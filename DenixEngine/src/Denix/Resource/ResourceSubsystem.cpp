#include "ResourceSubsystem.h"
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Scene/MeshData.h"
#include "Denix/Video/Renderer/RenderComponent.h"
#include "Denix/Video/GL/Shader.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Material.h"
#include "Denix/Video/GL/Mesh.h"
#include "Denix/Video/GL/Model.h"
#include "Denix/Resource/Asset.h"
#include "Denix/Audio/AudioClip.h"
#include "yaml-cpp/yaml.h"

namespace fs = std::filesystem;

namespace Denix
{
	ResourceSubsystem* ResourceSubsystem::s_ResourceSubsystem = nullptr;

	Ref<Asset> ResourceSubsystem::GetAsset(const std::string& _path)
	{
		for (const auto& asset : s_ResourceSubsystem->m_AssetStore)
		{
			if (asset->GetAssetPath() == _path)
			{
				return asset;
			}
		}

		return nullptr;
	}

	void ResourceSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogResource, Warn, "Resource Subsystem Initializing")

		// Iniatlize Default Assets
		std::vector<ShaderSource> defaultShaders;
		defaultShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\Vertex.glsl)");
		defaultShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\Fragment.glsl)");
		if (LoadShader(defaultShaders, "DefaultShader")) m_DefaultShader = GetShader("DefaultShader");
		else throw std::runtime_error("Default Shader not loaded");
		
		std::vector<ShaderSource> viewportShaders;
		viewportShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\FBVertex.glsl)");
		viewportShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\FBFragment.glsl)");
		if (LoadShader(viewportShaders, "ViewportShader")) m_ViewportShader = GetShader("FBShader");
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
						m_AudioClipStore[asset->m_AssetName] = MakeRef<AudioClip>(asset);
					}
				}
				
				m_AssetStore.push_back(asset);
			}
		}

	    DE_LOG(LogResource, Info, "Resource Subsystem Initialized")
	}

	void ResourceSubsystem::Deinitialize()
	{
		Subsystem::Deinitialize();

		// Free all resources
		m_TextureStore.clear();

		DE_LOG(LogResource, Trace, "Resource Subsystem Deinitialized")
	}
	

	Ref<Asset> ResourceSubsystem::GetSceneAsset(const std::string& _path)
	{
		// We Should validate the path first

		// Check registered assets
		for (const auto& asset : s_ResourceSubsystem->m_SceneStore)
		{
			if (asset->GetAssetPath() == _path)
			{
				return asset;
			}
		}

		return nullptr;
	}

	////////////////////////  SHADERS ///////////////////////////////
	Ref<Shader> ResourceSubsystem::LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _name)
	{
		// Check if the shader already exists
		if (s_ResourceSubsystem->m_ShaderStore.contains(_name))
		{
			DE_LOG(LogShader, Error, "GLShader already exists: {}", _name)
			return nullptr;
		}

		// Create a new shader
		if (const Ref<Shader> program = MakeRef<Shader>(ObjectInit(_name)))
		{
			if (!program->m_GL_ID) return nullptr;

			program->m_ShaderSources = _shaders;

			if (!program->CompileProgram()) return nullptr;

			s_ResourceSubsystem->m_ShaderStore[_name] = program;

			DE_LOG(LogShader, Trace, "Shader Loaded: {}", _name)
			
			return program;
		}

		DE_LOG(LogShader, Error, "Failed to load shader: {}", _name)
		return nullptr;
	}

	bool ResourceSubsystem::ReloadShader(Ref<Shader>& _shader)
	{
		// Create a new shader to check if it compiles
		const Ref<Shader> testShader = MakeRef<Shader>(ObjectInit(_shader->GetName() + "_temp"));
		
		if (!testShader->GetGL_ID()) return false;

		testShader->m_ShaderSources = _shader->GetShaderSources();

		// Compile the new shader
		if (!testShader->CompileProgram())
		{
			DE_LOG(LogResource, Error, "Shader Recompile failed: {}", _shader->GetName())
			return false;
		}

		// Save the shader sources to disk
		for(const auto& shader : testShader->GetShaderSources())
		{
			if(!FileSubsystem::WriteFile(shader.Path, shader.Source))
			{
				DE_LOG(LogResource, Error, "Failed to write shader source to disk: {}", shader.Path)
				return false;
			}
		}

		// Reassign the new shader ID to the old shader
		_shader->m_GL_ID = testShader->m_GL_ID;
		DE_LOG(LogResource, Info, "Shader Recompiled successfully: {}", _shader->GetName())
		
		return true;
	}

	Ref<Shader> ResourceSubsystem::GetShader(const std::string& _name)
	{
		if (s_ResourceSubsystem->m_ShaderStore.contains(_name))
		{
			return s_ResourceSubsystem->m_ShaderStore[_name];
		}

		DE_LOG(LogShader, Error, "GLShader not found: {}", _name)
			return nullptr;
	}

	

	////////////////////////  TEXTURES ///////////////////////////////
	Ref<Texture> ResourceSubsystem::LoadTexture(const std::string& _path)
	{
		// Check it isn't already loaded
		if (s_ResourceSubsystem->m_TextureStore.contains(_path))
		{
			DE_LOG(LogResource, Error, "Load Texture: A texture name: {} is already loaded", _path)
				return nullptr;
		}

		Ref<Texture> texture = MakeRef<Texture>(_path);

		if (!texture->LoadTexture()) return nullptr;

		s_ResourceSubsystem->m_TextureStore[_path] = texture;
		DE_LOG(LogResource, Trace, "Texture loaded: {}", _path)

		return texture;
	}
	
	Ref<Texture> ResourceSubsystem::GetTexture(const std::string& _path)
	{
		if (s_ResourceSubsystem->m_TextureStore.contains(_path))
		{
			return s_ResourceSubsystem->m_TextureStore[_path];
		}

		return nullptr;
	}

	////////////////////////  MATERIALS ///////////////////////////////
	Ref<Material> ResourceSubsystem::LoadMaterial(const Ref<Asset>& _matAsset)
	{
		if (s_ResourceSubsystem->m_MaterialStore.contains(_matAsset->m_AssetName))
		{
			DE_LOG(LogResource, Error, "Load Material: A material name: {} is already loaded", _matAsset->m_AssetName)
			return s_ResourceSubsystem->m_MaterialStore[_matAsset->m_AssetName];
		}

		if (Ref<Material> material = MakeRef<Material>(_matAsset))
		{
			s_ResourceSubsystem->m_MaterialStore[_matAsset->m_AssetName] = material;
			DE_LOG(LogResource, Trace, "Material Loaded: {}", _matAsset->m_AssetName)
			return material;
		}
		
		DE_LOG(LogResource, Error, "Failed to load material: {}", _matAsset->m_AssetName)
		return nullptr;
	}

	Ref<Material> ResourceSubsystem::GetMaterial(const std::string& _path)
	{
		for (const auto& material : s_ResourceSubsystem->m_MaterialStore)
		{
			if (material.second->GetAsset()->GetAssetPath() == _path)
			{
				return material.second;
			}
		}

		return nullptr;
	}

	

	////////////////////////  MESHES ///////////////////////////////
	bool ResourceSubsystem::AddMesh(const Ref<Mesh>& _mesh)
	{
		if (s_ResourceSubsystem->m_MeshStore.contains(_mesh->GetName()))
		{
			DE_LOG(LogResource, Error, "Load Mesh: A Mesh name: {} is already loaded", _mesh->GetName())
				return false;
		}

		DE_LOG(LogResource, Trace, "Mesh Loaded: {}", _mesh->GetName())
			s_ResourceSubsystem->m_MeshStore[_mesh->GetName()] = _mesh;

		return true;
	}

	bool ResourceSubsystem::LoadMesh(const std::string& _name, const float* _vertices, const unsigned int* _indices,
	                                 const unsigned int _verticesCount, const unsigned int _numOfIndices)
	{
		if (s_ResourceSubsystem->m_MeshStore.contains(_name))
		{
			DE_LOG(LogResource, Error, "Load Mesh: A Mesh name: {} is already loaded",_name)
				return false;
		}

		const Ref<Mesh> mesh = MakeRef<Mesh>(_vertices, _indices, _verticesCount, _numOfIndices, ObjectInit(_name));
		DE_LOG(LogResource, Trace, "Mesh Loaded: {}", mesh->GetName())
		s_ResourceSubsystem->m_MeshStore[_name] = mesh;

		return true;
	}

	Ref<Mesh> ResourceSubsystem::GetMesh(const std::string& _name)
	{
		if (s_ResourceSubsystem->m_MeshStore.contains(_name))
		{
			return s_ResourceSubsystem->m_MeshStore[_name];
		}

		return nullptr;
	}

	bool ResourceSubsystem::AddModel(const Ref<Model>& _mesh)
	{
		return false;
	}

	bool ResourceSubsystem::LoadModel(const std::string& _name, const std::string& _path)
	{
		if (s_ResourceSubsystem->m_ModelStore.contains(_name))
		{
			DE_LOG(LogResource, Error, "Load Mesh: A Model name: {} is already loaded", _name)
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

		DE_LOG(LogResource, Trace, "Model Loaded: {}", model->GetName())
			s_ResourceSubsystem->m_ModelStore[_name] = model;

		return true;
	}

	Ref<Model> ResourceSubsystem::GetModel(const std::string& _name)
	{
		if (s_ResourceSubsystem->m_ModelStore.contains(_name))
		{
			return s_ResourceSubsystem->m_ModelStore[_name];
		}

		return nullptr;
	}
}