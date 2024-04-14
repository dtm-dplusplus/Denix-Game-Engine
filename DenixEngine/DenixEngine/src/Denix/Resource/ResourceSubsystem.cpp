#include "ResourceSubsystem.h"
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Scene/Component/RenderComponent.h"
#include "Denix/Video/GL/GLShader.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Material.h"


namespace Denix
{
	ResourceSubsystem* ResourceSubsystem::s_ResourceSubsystem = nullptr;

	void ResourceSubsystem::Initialize()
	{
		Subsystem::Initialize();

		DE_LOG_CREATE(LogResourceSubsystem)

		// Iniatlize Default Assets

			// Create Debug Shader
		{
			std::vector<ShaderSource> shaders;
			shaders.emplace_back(FileSubsystem::GetEngineRoot() + R"(res\shaders\DebugVertex.glsl)");
			shaders.emplace_back(FileSubsystem::GetEngineRoot() + R"(res\shaders\DebugFragment.glsl)");
			LoadShader(shaders, "DebugShader");
		}

		// Create Texture Shader
		{
			std::vector<ShaderSource> shaders;
			shaders.emplace_back(FileSubsystem::GetEngineRoot() + R"(res\shaders\Vertex.glsl)");
			shaders.emplace_back(FileSubsystem::GetEngineRoot() + R"(res\shaders\Fragment.glsl)");
			LoadShader(shaders, "DefaultShader");
		}

		LoadTexture(FileSubsystem::GetEngineRoot() + R"(res\textures\DefaultTexture.png)", "DefaultTexture");

		m_MaterialStore["MAT_Default"] = MakeRef<Material>();

	    DE_LOG(LogResourceSubsystem, Trace, "Resource Subsystem Initialized")
	}

	void ResourceSubsystem::Deinitialize()
	{
		Subsystem::Deinitialize();

		// Free all resources
		m_TextureStore.clear();

		DE_LOG(LogResourceSubsystem, Trace, "Resource Subsystem Deinitialized")
	}

	void ResourceSubsystem::LoadMaterial(const Ref<Material>& ref)
	{
		if (s_ResourceSubsystem->m_MaterialStore.contains(ref->GetFriendlyName()))
		{
			DE_LOG(LogResourceSubsystem, Error, "Load Material: A material name: {} is already loaded", ref->GetName())
			return;
		}

		DE_LOG(LogResourceSubsystem, Trace, "Material Loaded: {}", ref->GetFriendlyName())
		s_ResourceSubsystem->m_MaterialStore[ref->GetFriendlyName()] = ref;
	}

	Ref<Texture> ResourceSubsystem::LoadTexture(const std::string& _path, const std::string& _name)
	{
		// Check it isn't already loaded
		if (s_ResourceSubsystem->m_TextureStore.contains(_path))
		{
			DE_LOG(LogResourceSubsystem, Error, "Load Texture: A texture name: {} is already loaded", _path)
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

	Ref<Material> ResourceSubsystem::GetMaterial(const std::string& _name)
	{
		if (s_ResourceSubsystem->m_MaterialStore.contains(_name))
		{
			return s_ResourceSubsystem->m_MaterialStore[_name];
		}

		return nullptr;
	}

	std::unordered_map<std::string, Ref<GLShader>> ResourceSubsystem::GetShaderStore()
	{
		return s_ResourceSubsystem->m_ShaderStore;
	}

	void ResourceSubsystem::AddShader(const Ref<GLShader>& _shader)
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
		if (const Ref<GLShader> program = MakeRef<GLShader>(ObjectInitializer(_name)))
		{
			if (!program->GetGL_ID()) return false;

			program->SetShaderSources(_shaders);

			if (!program->CompileProgram()) return false;

			AddShader(program);

			return true;
		}

		return false;
	}

	Ref<GLShader> ResourceSubsystem::GetShader(const std::string& _name)
	{
		if (ShaderExists(_name))
		{
			return s_ResourceSubsystem->m_ShaderStore[_name];
		}

		DE_LOG(LogShader, Error, "GLShader not found: {}", _name)
		return nullptr;
	}
}
