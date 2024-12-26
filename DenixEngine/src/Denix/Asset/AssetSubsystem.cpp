#include "AssetSubsystem.h"
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Scene/MeshData.h"
#include "Denix/Video/Renderer/RenderComponent.h"
#include "Denix/Video/GL/Shader.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Material.h"
#include "Denix/Video/GL/Mesh.h"
#include "Denix/Video/GL/Model.h"
#include "Denix/Asset/Asset.h"
#include "Denix/Audio/AudioClip.h"
#include "yaml-cpp/yaml.h"

namespace fs = std::filesystem;

namespace Denix
{
	AssetSubsystem* AssetSubsystem::s_AssetSubsystem = nullptr;

	Ref<Asset> AssetSubsystem::GetAsset(const std::string& _path)
	{
		for (const auto& asset : s_AssetSubsystem->m_AssetStore)
		{
			if (asset->GetAssetPath() == _path)
			{
				return asset;
			}
		}

		return nullptr;
	}

	Ref<AudioClip> AssetSubsystem::LoadAudioClip(const Ref<Asset>& _audioClipAsset)
	{
		if (s_AssetSubsystem->m_AudioClipStore.contains(_audioClipAsset->GetAssetPath()))
		{
			DE_LOG(LogAsset, Error, "Load AudioClip: An audio clip name: {} is already loaded", _audioClipAsset->m_AssetName)
			return s_AssetSubsystem->m_AudioClipStore[_audioClipAsset->GetAssetPath()];
		}

		if (Ref<AudioClip> audioClip = MakeRef<AudioClip>(AssetInit(_audioClipAsset->GetAssetPath())))
		{
			if (!audioClip->Load()) return nullptr;
			
			s_AssetSubsystem->m_AudioClipStore[_audioClipAsset->GetAssetPath()] = audioClip;
			DE_LOG(LogAsset, Trace, "Audio Clip Loaded: {}", _audioClipAsset->m_AssetName)
			return audioClip;
		}

		return nullptr;
	}

	Ref<AudioClip> AssetSubsystem::GetAudioClip(const std::string& _path)
	{
		if (s_AssetSubsystem->m_AudioClipStore.contains(_path))
		{
			return s_AssetSubsystem->m_AudioClipStore[_path];
		}

		DE_LOG(LogAsset, Error, "Audio Clip not found: {}", _path)
		
		return nullptr;
	}

	void AssetSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogAsset, Warn, "Asset Subsystem Initializing")

		// Iniatlize Default Assets
		std::vector<ShaderSource> defaultShaders;
		defaultShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\Vertex.glsl)");
		defaultShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\Fragment.glsl)");
		if (Ref<Shader> defShader = LoadShader(defaultShaders, "DefaultShader")) m_DefaultShader = defShader;
		else throw std::runtime_error("Default Shader not loaded");
		
		std::vector<ShaderSource> viewportShaders;
		viewportShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\FBVertex.glsl)");
		viewportShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\FBFragment.glsl)");
		if (Ref<Shader> fbShader = LoadShader(viewportShaders, "FBShader")) m_FramebufferShader = fbShader;
		else throw std::runtime_error("Viewport Shader not loaded");


		std::string defMatPath = FileSubsystem::GetEngineContentRoot() + R"(materials\MAT_Default.asset)";
		if (Ref<Material> defMat = LoadMaterial(MakeRef<Asset>(defMatPath))) m_DefaultMaterial = defMat;
		else throw std::runtime_error("Default Material not loaded");
			
		std::string defTexPath = FileSubsystem::GetEngineContentRoot() + R"(textures\DefaultTexture.png)";
		if (Ref<Texture> defTex = LoadTexture(defTexPath)) m_DefaultTexture = defTex;
		else throw std::runtime_error("Default Texture not loaded");
			
		// Search Project directory for assets
		for (const auto& entry : std::filesystem::recursive_directory_iterator(FileSubsystem::GetContentRoot()))
		{
			const std::string& path = entry.path().string();
			if (entry.is_regular_file())
			{
				Ref<Asset> asset = MakeRef<Asset>(path);

				// DE_ASSET Validation
				if (asset->m_AssetExtension == ".asset")
				{
					const std::string assetData = FileSubsystem::ReadFile(path);

					// Scene Asset
					if (assetData.find("DE_ASSET_SCENE") != std::string::npos)
					{
						// We don't need to load the scene asset here
						m_SceneStore.push_back(asset);
					}

					// Material Asset
					else if (assetData.find("DE_ASSET_MATERIAL") != std::string::npos)
					{
						m_MaterialStore[asset->GetAssetName()] = MakeRef<Material>(asset);
					}
				}

				// NON DE_ASSET Validation
				else
				{
					// Texture Asset
					if (asset->m_AssetExtension == ".png" || asset->m_AssetExtension == ".jpg")
					{
						// Probably not a good idea to load all textures at startup
						LoadTexture(path);
					}
				
					// Shader Check
					
					// Model Check
					else if (asset->m_AssetExtension == ".obj")
					{
						LoadModel(asset->m_AssetName, path);
					}
					// Audio Asset
					else if (asset->m_AssetExtension == ".wav")
					{
						LoadAudioClip(asset);
					}
				}
				
				m_AssetStore.push_back(asset);
			}
		}

	    DE_LOG(LogAsset, Info, "Asset Subsystem Initialized")
	}

	void AssetSubsystem::Deinitialize()
	{
		Subsystem::Deinitialize();

		// Free all Asset
		m_MeshStore.clear();
		m_ModelStore.clear();
		
		m_SceneStore.clear();
		m_AssetStore.clear();
		m_TextureStore.clear();
		
		m_ShaderStore.clear();
		m_DefaultShader = nullptr;
		m_FramebufferShader = nullptr;

		m_MaterialStore.clear();
		m_DefaultMaterial = nullptr;

		m_AudioClipStore.clear();
		
		DE_LOG(LogAsset, Trace, "Resource Subsystem Deinitialized")
	}
	

	Ref<Asset> AssetSubsystem::GetSceneAsset(const std::string& _path)
	{
		// We Should validate the path first

		// Check registered assets
		for (const auto& asset : s_AssetSubsystem->m_SceneStore)
		{
			if (asset->GetAssetPath() == _path)
			{
				return asset;
			}
		}

		return nullptr;
	}

	////////////////////////  SHADERS ///////////////////////////////
	Ref<Shader> AssetSubsystem::LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _name)
	{
		// Check if the shader already exists
		if (s_AssetSubsystem->m_ShaderStore.contains(_name))
		{
			DE_LOG(LogShader, Error, "GLShader already exists: {}", _name)
			return nullptr;
		}

		// Create a new shader
		if (const Ref<Shader> shader = MakeRef<Shader>(ObjectInit(_name)))
		{

			shader->m_ShaderSources = _shaders;

			if (!shader->CompileProgram()) return nullptr;

			s_AssetSubsystem->m_ShaderStore[_name] = shader;

			DE_LOG(LogShader, Trace, "Shader Loaded: {}", _name)
			
			return shader;
		}

		DE_LOG(LogShader, Error, "Failed to load shader: {}", _name)
		return nullptr;
	}

	bool AssetSubsystem::ReloadShader(Ref<Shader>& _shader)
	{
		// Create a new shader to check if it compiles
		const Ref<Shader> testShader = MakeRef<Shader>(ObjectInit(_shader->GetName() + "_temp"));
		
		if (!testShader->GetGL_ID()) return false;

		testShader->m_ShaderSources = _shader->GetShaderSources();

		// Compile the new shader
		if (!testShader->CompileProgram())
		{
			DE_LOG(LogAsset, Error, "Shader Recompile failed: {}", _shader->GetName())
			return false;
		}

		// Save the shader sources to disk
		for(const auto& shader : testShader->GetShaderSources())
		{
			if(!FileSubsystem::WriteFile(shader.Path, shader.Source))
			{
				DE_LOG(LogAsset, Error, "Failed to write shader source to disk: {}", shader.Path)
				return false;
			}
		}

		// Reassign the new shader ID to the old shader
		_shader->m_GL_ID = testShader->m_GL_ID;
		DE_LOG(LogAsset, Info, "Shader Recompiled successfully: {}", _shader->GetName())
		
		return true;
	}

	Ref<Shader> AssetSubsystem::GetShader(const std::string& _name)
	{
		if (s_AssetSubsystem->m_ShaderStore.contains(_name))
		{
			return s_AssetSubsystem->m_ShaderStore[_name];
		}

		DE_LOG(LogShader, Error, "GLShader not found: {}", _name)
			return nullptr;
	}

	

	////////////////////////  TEXTURES ///////////////////////////////
	Ref<Texture> AssetSubsystem::LoadTexture(const std::string& _path)
	{
		// Check it isn't already loaded
		if (s_AssetSubsystem->m_TextureStore.contains(_path))
		{
			DE_LOG(LogAsset, Warn, "Load Texture: A texture name: {} is already loaded", _path)
				return s_AssetSubsystem->m_TextureStore[_path];
		}

		Ref<Texture> texture = MakeRef<Texture>(_path);

		if (!texture->LoadTexture()) return nullptr;

		s_AssetSubsystem->m_TextureStore[_path] = texture;
		DE_LOG(LogAsset, Trace, "Texture loaded: {}", _path)

		return texture;
	}
	
	Ref<Texture> AssetSubsystem::GetTexture(const std::string& _path)
	{
		if (s_AssetSubsystem->m_TextureStore.contains(_path))
		{
			return s_AssetSubsystem->m_TextureStore[_path];
		}

		return nullptr;
	}

	////////////////////////  MATERIALS ///////////////////////////////
	Ref<Material> AssetSubsystem::LoadMaterial(const Ref<Asset>& _matAsset)
	{
		if (s_AssetSubsystem->m_MaterialStore.contains(_matAsset->m_AssetName))
		{
			DE_LOG(LogAsset, Error, "Load Material: A material name: {} is already loaded", _matAsset->m_AssetName)
			return s_AssetSubsystem->m_MaterialStore[_matAsset->m_AssetName];
		}

		if (Ref<Material> material = MakeRef<Material>(_matAsset))
		{
			s_AssetSubsystem->m_MaterialStore[_matAsset->m_AssetName] = material;
			DE_LOG(LogAsset, Trace, "Material Loaded: {}", _matAsset->m_AssetName)
			return material;
		}
		
		DE_LOG(LogAsset, Error, "Failed to load material: {}", _matAsset->m_AssetName)
		return nullptr;
	}

	Ref<Material> AssetSubsystem::GetMaterial(const std::string& _path)
	{
		for (const auto& material : s_AssetSubsystem->m_MaterialStore)
		{
			if (material.second->GetAsset()->GetAssetPath() == _path)
			{
				return material.second;
			}
		}

		return nullptr;
	}

	

	////////////////////////  MESHES ///////////////////////////////
	bool AssetSubsystem::AddMesh(const Ref<Mesh>& _mesh)
	{
		if (s_AssetSubsystem->m_MeshStore.contains(_mesh->GetName()))
		{
			DE_LOG(LogAsset, Error, "Load Mesh: A Mesh name: {} is already loaded", _mesh->GetName())
				return false;
		}

		DE_LOG(LogAsset, Trace, "Mesh Loaded: {}", _mesh->GetName())
			s_AssetSubsystem->m_MeshStore[_mesh->GetName()] = _mesh;

		return true;
	}

	bool AssetSubsystem::LoadMesh(const std::string& _name, const float* _vertices, const unsigned int* _indices,
	                                 const unsigned int _verticesCount, const unsigned int _numOfIndices)
	{
		if (s_AssetSubsystem->m_MeshStore.contains(_name))
		{
			DE_LOG(LogAsset, Error, "Load Mesh: A Mesh name: {} is already loaded",_name)
				return false;
		}

		const Ref<Mesh> mesh = MakeRef<Mesh>(_vertices, _indices, _verticesCount, _numOfIndices, ObjectInit(_name));
		DE_LOG(LogAsset, Trace, "Mesh Loaded: {}", mesh->GetName())
		s_AssetSubsystem->m_MeshStore[_name] = mesh;

		return true;
	}

	Ref<Mesh> AssetSubsystem::GetMesh(const std::string& _name)
	{
		if (s_AssetSubsystem->m_MeshStore.contains(_name))
		{
			return s_AssetSubsystem->m_MeshStore[_name];
		}

		return nullptr;
	}

	bool AssetSubsystem::AddModel(const Ref<Model>& _mesh)
	{
		return false;
	}

	bool AssetSubsystem::LoadModel(const std::string& _name, const std::string& _path)
	{
		if (s_AssetSubsystem->m_ModelStore.contains(_name))
		{
			DE_LOG(LogAsset, Error, "Load Mesh: A Model name: {} is already loaded", _name)
				return false;
		}

		const Ref<Model> model = MakeRef<Model>(_name, _path);
		if (!model->m_IsLoaded) return false;

		for (auto texture : model->m_Textures)
		{
			if (texture)
			{
				if (!texture->GetTextureID())
				{
					texture = GetTexture("DefaultTexture");
				}
			}
		}

		DE_LOG(LogAsset, Trace, "Model Loaded: {}", model->GetName())
			s_AssetSubsystem->m_ModelStore[_name] = model;

		return true;
	}

	Ref<Model> AssetSubsystem::GetModel(const std::string& _name)
	{
		if (s_AssetSubsystem->m_ModelStore.contains(_name))
		{
			return s_AssetSubsystem->m_ModelStore[_name];
		}

		return nullptr;
	}
}