#include "ResourceSubsystem.h"
#include "ShaderSubsystem.h"

#include "Denix/Scene/Component/RenderComponent.h"

namespace Denix
{
	ResourceSubsystem* ResourceSubsystem::s_ResourceSubsystem = nullptr;

	void ResourceSubsystem::Initialize()
	{
		Subsystem::Initialize();

		// Iniatlize Default Assets

		DE_LOG_CREATE(LogResourceSubsystem)

		DE_LOG(LogResourceSubsystem, Trace, "Resource Subsystem Initialized")
	}

	void ResourceSubsystem::Deinitialize()
	{
		Subsystem::Deinitialize();

		// Free all resources
		m_TextureStore.clear();

		DE_LOG(LogResourceSubsystem, Trace, "Resource Subsystem Deinitialized")
	}

	Ref<Texture> ResourceSubsystem::LoadTexture(const std::string& _path, const std::string& _name)
	{
		// Check it isn't already loaded
		if (s_ResourceSubsystem->m_TextureStore.contains(_path))
		{
			DE_LOG(LogResourceSubsystem, Error, "Load Texture: A texture name: {} is already loaded", _path)
			return nullptr;
		}

		const Ref<Texture> texture = MakeRef<Texture>(_path, _name);
		texture->LoadTexture();

		if (!texture->GetTextureID()) return nullptr;

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
}