#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"

namespace Denix
{
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

       static Ref<Texture> LoadTexture(const std::string& _path, const std::string& _name);

        static ResourceSubsystem* Get() { return s_ResourceSubsystem; }

        static Ref<Texture> GetTexture(const std::string& _name);

        static Ref<class GLShader> GetShader(const std::string& _name);

    private:
        static ResourceSubsystem* s_ResourceSubsystem;

        std::unordered_map<std::string, Ref<Texture>> m_TextureStore;
    };
}
