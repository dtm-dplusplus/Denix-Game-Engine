#include "ResourceSubsystem.h"
#include "ShaderSubsystem.h"
#include "FileSubsystem.h"
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

	Ref<GLShader> ResourceSubsystem::GetShader(const std::string& _name)
	{
		return ShaderSubsystem::Get()->GetShader(_name);
	}

	Ref<Material> ResourceSubsystem::GetMaterial(const std::string& _name)
	{
		if (s_ResourceSubsystem->m_MaterialStore.contains(_name))
		{
			return s_ResourceSubsystem->m_MaterialStore[_name];
		}

		return nullptr;
	}
}
