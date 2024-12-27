#pragma once

#include <yaml-cpp/node/node.h>

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"

namespace Denix
{
    class AudioClip;
    class Asset;
    class Model;
    struct ShaderSource;
	class Shader;
	class Material;
	class Texture;
    class Scene;

    class AssetSubsystem : public Subsystem
    {
    public:
        AssetSubsystem()
        {
            s_AssetSubsystem = this;
			DE_LOG_CREATE(LogAsset)
        }

        ~AssetSubsystem() override
        {
            s_AssetSubsystem = nullptr;
        }

        // Scenes
        static std::vector<Ref<Asset>>  GetSceneStore() { return s_AssetSubsystem->m_SceneStore; }
        static Ref<Asset> GetSceneAsset(const std::string& _path);
        
        // Shaders
        static std::unordered_map<std::string, Ref<Shader>> GetShaderStore() 
        {
            return s_AssetSubsystem->m_ShaderStore;
        }

        Ref<Shader> LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _path);

        static bool ReloadShader(const Ref<Shader>& _shader);

        
        static Ref<Shader> GetShader(const std::string& _name);
        static Ref<Shader> GetDefaultShader() { return s_AssetSubsystem->m_DefaultShader; }
        static Ref<Shader> GetFrameBufferShader() { return s_AssetSubsystem->m_FramebufferShader; }
        Ref<Shader> m_DefaultShader;
        Ref<Shader> m_FramebufferShader;
        
        // Materials
        static Ref<Material> LoadMaterial(const Ref<Asset>& _matAsset);

        static Ref<Material> GetMaterial(const std::string& _path);
        static Ref<Material> GetDefaultMaterial() { return s_AssetSubsystem->m_DefaultMaterial; }
        Ref<Material> m_DefaultMaterial;
        
        static std::unordered_map<std::string, Ref<Material>>& GetMaterialStore() { return s_AssetSubsystem->m_MaterialStore; }

        // Textures
        static Ref<Texture> LoadTexture(const std::string& _path);
        static Ref<Texture> GetTexture(const std::string& _path);
        static Ref<Texture> GetDefaultTexture() { return s_AssetSubsystem->m_DefaultTexture; }
        Ref<Texture> m_DefaultTexture;
        
        static std::unordered_map<std::string, Ref<Texture>>& GetTextureStore() { return s_AssetSubsystem->m_TextureStore; }

        // Models
        static bool LoadModel(const std::string& _path);
        static Ref<Model> GetModel(const std::string& _name);
		static std::unordered_map<std::string, Ref<Model>>& GetModelStore() { return s_AssetSubsystem->m_ModelStore; }

        static std::vector<Ref<Asset>>& GetAssetStore() { return s_AssetSubsystem->m_AssetStore; }
        static Ref<Asset> GetAsset(const std::string& _path);

        // Audio
        static Ref<AudioClip> LoadAudioClip(const Ref<Asset>& _audioClipAsset);
        static Ref<AudioClip> GetAudioClip(const std::string& _path);
        
        std::unordered_map<std::string, Ref<AudioClip>> m_AudioClipStore;
        
        static AssetSubsystem* Get() { return s_AssetSubsystem; }

    private:
        void Initialize() override;

        void Deinitialize() override;
        
        static AssetSubsystem* s_AssetSubsystem;

		std::unordered_map<std::string, Ref<Shader>> m_ShaderStore;

        std::unordered_map<std::string, Ref<Material>> m_MaterialStore;

		std::unordered_map<std::string, Ref<Model>> m_ModelStore;

        std::unordered_map<std::string, Ref<Texture>> m_TextureStore;
        
        std::vector<Ref<Asset>> m_AssetStore;

        std::vector<Ref<Asset>> m_SceneStore;

        friend class Engine;
    };
}