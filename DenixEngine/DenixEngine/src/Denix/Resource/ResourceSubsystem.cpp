#include "ResourceSubsystem.h"
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Scene/MeshData.h"
#include "Denix/Video/Renderer/RenderComponent.h"
#include "Denix/Video/GL/Shader.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Material.h"
#include "Denix/Video/GL/Mesh.h"
#include "Denix/Video/GL/Model.h"


namespace Denix
{
	ResourceSubsystem* ResourceSubsystem::s_ResourceSubsystem = nullptr;

	void ResourceSubsystem::Initialize()
	{
		Subsystem::Initialize();

		DE_LOG_CREATE(LogResource)

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
		// TEXTURES
		LoadTexture(FileSubsystem::GetEngineContentRoot() + R"(textures\DefaultTexture.png)", "DefaultTexture");

		// MATERIALS
		{
			Ref<Material> defaultMaterial = MakeRef<Material>(ObjectInitializer("MAT_Default"));
			defaultMaterial->SetShader(GetShader("DefaultShader"));
			defaultMaterial->GetBaseParam().IsTexture = true;
			defaultMaterial->GetBaseParam().Texture = GetTexture("DefaultTexture");
			AddMaterial(defaultMaterial);

			Ref<Material> unlitMaterial = MakeRef<Material>(ObjectInitializer("MAT_Unlit"));
			unlitMaterial->SetShader(GetShader("UnlitShader"));
			AddMaterial(unlitMaterial);
		}
		m_MaterialStore["MAT_Default"] = MakeRef<Material>();

		// Models
		LoadModel("SM_Plane", FileSubsystem::GetEngineContentRoot() + R"(models\Plane.obj)");
		LoadModel("SM_Cube", FileSubsystem::GetEngineContentRoot() + R"(models\Cube.obj)");
		LoadModel("SM_Sphere", FileSubsystem::GetEngineContentRoot() + R"(models\Sphere.obj)");
		LoadModel("SM_Cone", FileSubsystem::GetEngineContentRoot() + R"(models\Cone.obj)");
		LoadModel("SM_Cylinder", FileSubsystem::GetEngineContentRoot() + R"(models\Cylinder.obj)");

	    DE_LOG(LogResource, Trace, "Resource Subsystem Initialized")
	}

	void ResourceSubsystem::Deinitialize()
	{
		Subsystem::Deinitialize();

		// Free all resources
		m_TextureStore.clear();

		DE_LOG(LogResource, Trace, "Resource Subsystem Deinitialized")
	}


	////////////////////////  SHADERS ///////////////////////////////
	void ResourceSubsystem::AddShader(const Ref<Shader>& _shader)
	{
		if (s_ResourceSubsystem->ShaderExists(_shader->GetFriendlyName()))
		{
			DE_LOG(LogShader, Error, "GLShader already exists: {}", _shader->GetFriendlyName())
				return;
		}

		s_ResourceSubsystem->m_ShaderStore[_shader->GetFriendlyName()] = _shader;
	}

	bool ResourceSubsystem::LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _name)
	{
		if (const Ref<Shader> program = MakeRef<Shader>(ObjectInitializer(_name)))
		{
			if (!program->GetGL_ID()) return false;

			program->SetShaderSources(_shaders);

			if (!program->CompileProgram()) return false;

			AddShader(program);

			return true;
		}

		return false;
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
	void ResourceSubsystem::AddTexture(const Ref<Texture>& _texture)
	{
	}

	Ref<Texture> ResourceSubsystem::LoadTexture(const std::string& _path, const std::string& _name)
	{
		// Check it isn't already loaded
		if (s_ResourceSubsystem->m_TextureStore.contains(_path))
		{
			DE_LOG(LogResource, Error, "Load Texture: A texture name: {} is already loaded", _path)
				return nullptr;
		}

		Ref<Texture> texture = MakeRef<Texture>(_path, _name);

		if (!texture->LoadTexture()) return nullptr;

		s_ResourceSubsystem->m_TextureStore[_name] = texture;
		DE_LOG(LogSceneSubSystem, Trace, "Texture loaded: ", _name)

			return GetTexture(_name);
	}
	
	Ref<Texture> ResourceSubsystem::GetTexture(const std::string& _name)
	{
		if (s_ResourceSubsystem->m_TextureStore.contains(_name))
		{
			return s_ResourceSubsystem->m_TextureStore[_name];
		}

		return nullptr;
	}

	////////////////////////  MATERIALS ///////////////////////////////
	void ResourceSubsystem::AddMaterial(const Ref<Material>& _ref)
	{
		if (s_ResourceSubsystem->m_MaterialStore.contains(_ref->GetFriendlyName()))
		{
			DE_LOG(LogResource, Error, "Add Material: A material name: {} is already loaded", _ref->GetName())
			return;
		}

		s_ResourceSubsystem->m_MaterialStore[_ref->GetFriendlyName()] = _ref;
		DE_LOG(LogResource, Trace, "Material Loaded: {}", _ref->GetFriendlyName())
	}

	Ref<Material> ResourceSubsystem::GetMaterial(const std::string& _name)
	{
		if (s_ResourceSubsystem->m_MaterialStore.contains(_name))
		{
			return s_ResourceSubsystem->m_MaterialStore[_name];
		}

		return nullptr;
	}

	

	////////////////////////  MESHES ///////////////////////////////
	bool ResourceSubsystem::AddMesh(const Ref<Mesh>& _mesh)
	{
		if (s_ResourceSubsystem->m_MeshStore.contains(_mesh->GetFriendlyName()))
		{
			DE_LOG(LogResource, Error, "Load Mesh: A Mesh name: {} is already loaded", _mesh->GetName())
				return false;
		}

		DE_LOG(LogResource, Trace, "Mesh Loaded: {}", _mesh->GetFriendlyName())
			s_ResourceSubsystem->m_MeshStore[_mesh->GetFriendlyName()] = _mesh;

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

		const Ref<Mesh> mesh = MakeRef<Mesh>(_vertices, _indices, _verticesCount, _numOfIndices, ObjectInitializer(_name));
		DE_LOG(LogResource, Trace, "Mesh Loaded: {}", mesh->GetFriendlyName())
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

		DE_LOG(LogResource, Trace, "Model Loaded: {}", model->GetFriendlyName())
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