#pragma once

#include <yaml-cpp/node/node.h>

#include "Denix/Core.h"
#include "Denix/Core/Subsystem.h"

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

    class AssetSubsystem : public Subsystem<AssetSubsystem>
    {
    public:
        AssetSubsystem();
        

        ~AssetSubsystem() override = default;

        AssetSubsystem(const AssetSubsystem& _other) = delete;
        AssetSubsystem(AssetSubsystem&& _other) noexcept = delete;
        AssetSubsystem& operator=(const AssetSubsystem& _other) = delete;
        AssetSubsystem& operator=(AssetSubsystem&& _other) noexcept = delete;
        
        // Scenes
        static std::vector<Ref<Asset>>  GetSceneStore() { return s_Instance->m_SceneStore; }
        static Ref<Asset> GetSceneAsset(const std::string& _path);
        
        // Shaders
        static std::unordered_map<std::string, Ref<Shader>> GetShaderStore() 
        {
            return s_Instance->m_ShaderStore;
        }

        Ref<Shader> LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _path);

        static bool ReloadShader(const Ref<Shader>& _shader);

        
        static Ref<Shader> GetShader(const std::string& _name);
        static Ref<Shader> GetDefaultShader() { return s_Instance->m_DefaultShader; }
        static Ref<Shader> GetFrameBufferShader() { return s_Instance->m_FramebufferShader; }
       
        
        // Materials
        static Ref<Material> LoadMaterial(const Ref<Asset>& _matAsset);

        static Ref<Material> GetMaterial(const std::string& _path);
        static Ref<Material> GetDefaultMaterial() { return s_Instance->m_DefaultMaterial; }
        
        static std::unordered_map<std::string, Ref<Material>>& GetMaterialStore() { return s_Instance->m_MaterialStore; }

        // Textures
        static Ref<Texture> LoadTexture(const std::string& _path);
        static Ref<Texture> GetTexture(const std::string& _path);
        static Ref<Texture> GetDefaultTexture() { return s_Instance->m_DefaultTexture; }
        
        static std::unordered_map<std::string, Ref<Texture>>& GetTextureStore() { return s_Instance->m_TextureStore; }

        // Models
        static bool LoadModel(const std::string& _path);
        static Ref<Model> GetModel(const std::string& _name);
		static std::unordered_map<std::string, Ref<Model>>& GetModelStore() { return s_Instance->m_ModelStore; }

        static std::vector<Ref<Asset>>& GetAssetStore() { return s_Instance->m_AssetStore; }
        static Ref<Asset> GetAsset(const std::string& _path);

        // Audio
        static Ref<AudioClip> LoadAudioClip(const Ref<Asset>& _audioClipAsset);
        static Ref<AudioClip> GetAudioClip(const std::string& _path);
        
    private:
        void Initialize() override;

        void Deinitialize() override;

        std::unordered_map<std::string, Ref<AudioClip>> m_AudioClipStore;

		std::unordered_map<std::string, Ref<Shader>> m_ShaderStore;
        Ref<Shader> m_DefaultShader;
        Ref<Shader> m_FramebufferShader;
        
        std::unordered_map<std::string, Ref<Material>> m_MaterialStore;
        Ref<Material> m_DefaultMaterial;

		std::unordered_map<std::string, Ref<Model>> m_ModelStore;

        std::unordered_map<std::string, Ref<Texture>> m_TextureStore;
        Ref<Texture> m_DefaultTexture;

        std::vector<Ref<Asset>> m_AssetStore;

        std::vector<Ref<Asset>> m_SceneStore;

        friend class Engine;
    };
}