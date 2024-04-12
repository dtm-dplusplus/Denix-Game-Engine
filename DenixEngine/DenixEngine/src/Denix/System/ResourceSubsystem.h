#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"

namespace Denix
{
	class Material;
	class Texture;

    class ResourceSubsystem : public Subsystem
    {
    public:
        ResourceSubsystem()
        {
            s_ResourceSubsystem = this;
        }

        ~ResourceSubsystem() override
        {
            s_ResourceSubsystem = nullptr;
        }

        void Initialize() override;

        void Deinitialize() override;

        static void LoadMaterial(const Ref<Material>& ref);

        static Ref<Texture> LoadTexture(const std::string& _path, const std::string& _name);

        static ResourceSubsystem* Get() { return s_ResourceSubsystem; }

        static Ref<Texture> GetTexture(const std::string& _name);

        static Ref<class GLShader> GetShader(const std::string& _name);

        static Ref<class Material> GetMaterial(const std::string& _name);

        static std::unordered_map<std::string, Ref<Texture>>& GetTextureStore() { return s_ResourceSubsystem->m_TextureStore; }

        static std::unordered_map<std::string, Ref<Material>>& GetMaterialStore() { return s_ResourceSubsystem->m_MaterialStore; }

        static std::unordered_map<std::string, Ref<GLShader>> GetShaderStore();

    private:
        static ResourceSubsystem* s_ResourceSubsystem;

        std::unordered_map<std::string, Ref<Texture>> m_TextureStore;

        std::unordered_map<std::string, Ref<Material>> m_MaterialStore;

    };
}