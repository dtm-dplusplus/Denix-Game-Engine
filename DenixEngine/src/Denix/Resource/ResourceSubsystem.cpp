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
		// SHADERS
		{
			std::vector<ShaderSource> debugShaders;
			debugShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\DebugVertex.glsl)");
			debugShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\DebugFragment.glsl)");
			LoadShader(debugShaders, "DebugShader");
		}

		{
			std::vector<ShaderSource> defaultShaders;
			defaultShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\Vertex.glsl)");
			defaultShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\Fragment.glsl)");
			LoadShader(defaultShaders, "DefaultShader");
		}

		{
			std::vector<ShaderSource> unlitShaders;
			unlitShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\UnlitVertex.glsl)");
			unlitShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\UnlitFragment.glsl)");
			LoadShader(unlitShaders, "UnlitShader");
		}

		{
			std::vector<ShaderSource> wireframeShaders;
			wireframeShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\WireframeVertex.glsl)");
			wireframeShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\WireframeFragment.glsl)");
			LoadShader(wireframeShaders, "WireframeShader");
		}

		{
			std::vector<ShaderSource> viewportShaders;
			viewportShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\FBVertex.glsl)");
			viewportShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\FBFragment.glsl)");
			LoadShader(viewportShaders, "FBShader");
		}

		{
			std::vector<ShaderSource> textShaders;
			textShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\TextVertex.glsl)");
			textShaders.emplace_back(FileSubsystem::GetEngineContentRoot() + R"(shaders\TextFragment.glsl)");
			LoadShader(textShaders, "TextShader");
		}

		// Search Project directory for assets
		for (const auto& entry : std::filesystem::recursive_directory_iterator(FileSubsystem::GetContentRoot()))
		{
			const std::string& path = entry.path().string();
			if (entry.is_regular_file())
			{
				Ref<Asset> asset = MakeRef<Asset>(path);

				// Scene Check
				if (path.find(".asset") != std::string::npos)
				{
					const std::string sceneData = FileSubsystem::ReadFile(path);
					if (sceneData.find("DE_ASSET_SCENE") != std::string::npos)
					{
						m_SceneStore.push_back(asset);
					}
				}
				// Material Check - Not a safe check
				if(path.find("MAT") != std::string::npos)
				{
					// We should check the material file to see if it is a valid asset - Skip for now
					YAML::Node matNode = YAML::LoadFile(path);
					Ref<Material> material = MakeRef<Material>(asset);
					// Check Texture
					m_MaterialStore[asset->GetAssetName()] = material; // Friendly name is redunant as we are checking by asset path now
					
					// We should do validation to check if the file is a valid asset - Skip for now
				}
				// Texture Check
				if (path.find(".png") != std::string::npos)
				{
					// We should do validation to check if the file is a valid asset - Skip for now
					LoadTexture(path);
				}
				// Shader Check
				// Model Check

				// We should do validation to check if the file is a valid asset - Skip for now
				m_AssetStore.push_back(asset);
			}
		}
		
		// TEXTURES
		//LoadTexture(FileSubsystem::GetEngineContentRoot() + R"(textures\DefaultTexture.png)");

		// MATERIALS
		if(!GetMaterial(FileSubsystem::GetEngineContentRoot() + "Material\\MAT_Default.asset"))
			throw std::runtime_error("Default Material Asset not loaded");

		// Models
		LoadModel("SM_Plane", FileSubsystem::GetEngineContentRoot() + R"(models\Plane.obj)");
		LoadModel("SM_Cube", FileSubsystem::GetEngineContentRoot() + R"(models\Cube.obj)");
		LoadModel("SM_Sphere", FileSubsystem::GetEngineContentRoot() + R"(models\Sphere.obj)");
		LoadModel("SM_Cone", FileSubsystem::GetEngineContentRoot() + R"(models\Cone.obj)");
		LoadModel("SM_Cylinder", FileSubsystem::GetEngineContentRoot() + R"(models\Cylinder.obj)");

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
	void ResourceSubsystem::AddShader(const Ref<Shader>& _shader)
	{
		if (s_ResourceSubsystem->ShaderExists(_shader->GetName()))
		{
			DE_LOG(LogShader, Error, "GLShader already exists: {}", _shader->GetName())
				return;
		}

		s_ResourceSubsystem->m_ShaderStore[_shader->GetName()] = _shader;
	}

	bool ResourceSubsystem::LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _name)
	{
		if (const Ref<Shader> program = MakeRef<Shader>(ObjectInit(_name)))
		{
			if (!program->GetGL_ID()) return false;

			program->SetShaderSources(_shaders);

			if (!program->CompileProgram()) return false;

			AddShader(program);

			DE_LOG(LogShader, Trace, "Shader Loaded: {}", _name)
			
			return true;
		}

		DE_LOG(LogShader, Error, "Failed to load shader: {}", _name)
		return false;
	}

	bool ResourceSubsystem::ReloadShader(Ref<Shader>& _shader)
	{
		// Create a new shader to check if it compiles
		const Ref<Shader> testShader = MakeRef<Shader>(ObjectInit(_shader->GetName() + "_temp"));
		
		if (!testShader->GetGL_ID()) return false;

		testShader->SetShaderSources(_shader->GetShaderSources());

		// Compile the new shader
		if (!testShader->CompileProgram(false))
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
		if (ShaderExists(_name))
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
	void ResourceSubsystem::AddMaterial(const Ref<Material>& _ref)
	{
		if (s_ResourceSubsystem->m_MaterialStore.contains(_ref->GetName()))
		{
			DE_LOG(LogResource, Error, "Add Material: A material name: {} is already loaded", _ref->GetName())
			return;
		}

		s_ResourceSubsystem->m_MaterialStore[_ref->GetName()] = _ref;
		DE_LOG(LogResource, Trace, "Material Loaded: {}", _ref->GetName())
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