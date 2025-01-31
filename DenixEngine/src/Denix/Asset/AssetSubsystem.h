/**
 * @file AssetSubsystem.h
 * @brief Declaration of the AssetSubsystem class for managing assets
 */

#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Subsystem.h"

namespace Denix
{
    class AudioClip;
    class Asset;
    class Model;
    struct ShaderSource;
    class Shader;
    class Texture;
    class Scene;

    /**
     * @class AssetSubsystem
     * @brief Manages assets such as scenes, shaders, textures, models, and audio clips.
     *
     * This subsystem provides static methods to load, retrieve, and manage various assets
     */
    class AssetSubsystem : public Subsystem<AssetSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        AssetSubsystem() = default;

        /**
         * @brief Destructor.
         */
        ~AssetSubsystem() override = default;

        // Delete copy and move operations
        AssetSubsystem(const AssetSubsystem& _other) = delete;
        AssetSubsystem(AssetSubsystem&& _other) noexcept = delete;
        AssetSubsystem& operator=(const AssetSubsystem& _other) = delete;
        AssetSubsystem& operator=(AssetSubsystem&& _other) noexcept = delete;

        /** @name Scene Management */
        ///@{
        /**
         * @brief Retrieves the list of stored scenes.
         * @return A vector of references to stored scene assets.
         */
        static std::vector<Ref<Asset>> GetSceneStore() { return s_Instance->m_SceneStore; }

        /**
         * @brief Retrieves a scene asset by path.
         * @param _path The path to the scene asset.
         * @return A reference to the scene asset.
         */
        static Ref<Asset> GetSceneAsset(const std::string& _path);

        /**
         * @brief Retrieves the startup scene.
         * @return A reference to the startup scene asset.
         */
        static Ref<Asset> GetStartupScene() { return s_Instance->m_StartupScene; }

        /**
         * @brief Sets the startup scene.
         * @param _scenePath The path of the scene to set as startup.
         */
        static void SetStartupScene(const std::string& _scenePath);
        ///@}

        /** @name Shader Management */
        ///@{
        /**
         * @brief Retrieves the shader store.
         * @return An unordered map of shader references.
         */
        static std::unordered_map<std::string, Ref<Shader>> GetShaderStore() { return s_Instance->m_ShaderStore; }

        /**
         * @brief Loads a shader from a given source.
         * @param _shaders A vector of shader sources.
         * @param _path The path of the shader.
         * @return A reference to the loaded shader.
         */
        static Ref<Shader> LoadShader(std::vector<ShaderSource>& _shaders, const std::string& _path);

        /**
         * @brief Reloads a shader.
         * @param _shader The shader to reload.
         * @return True if successful, false otherwise.
         */
        static bool ReloadShader(const Ref<Shader>& _shader);

        /**
         * @brief Retrieves a shader by name.
         * @param _name The name of the shader.
         * @return A reference to the shader.
         */
        static Ref<Shader> GetShader(const std::string& _name);

        /**
         * @brief Retrieves the default shader.
         * @return A reference to the default shader.
         */
        static Ref<Shader> GetDefaultShader() { return s_Instance->m_DefaultShader; }

        /**
         * @brief Retrieves the framebuffer shader.
         * @return A reference to the framebuffer shader.
         */
        static Ref<Shader> GetFrameBufferShader() { return s_Instance->m_FramebufferShader; }
        ///@}

        /** @name Texture Management */
        ///@{
        /**
         * @brief Loads a texture from a given path.
         * @param _path The path of the texture.
         * @return A reference to the loaded texture.
         */
        static Ref<Texture> LoadTexture(const std::string& _path);

        /**
         * @brief Retrieves a texture by path.
         * @param _path The path of the texture.
         * @return A reference to the texture.
         */
        static Ref<Texture> GetTexture(const std::string& _path);

        /**
         * @brief Retrieves the texture store.
         * @return An unordered map of texture references.
         */
        static std::unordered_map<std::string, Ref<Texture>>& GetTextureStore() { return s_Instance->m_TextureStore; }
        ///@}

        /** @name Model Management */
        ///@{
        /**
         * @brief Loads a model from a given path.
         * @param _path The path of the model.
         * @return True if the model was successfully loaded, false otherwise.
         */
        static bool LoadModel(const std::string& _path);

        /**
         * @brief Retrieves a model by path.
         * @param _path The path of the model.
         * @return A reference to the model.
         */
        static Ref<Model> GetModel(const std::string& _path);

        /**
         * @brief Retrieves the model store.
         * @return An unordered map of model references.
         */
        static std::unordered_map<std::string, Ref<Model>>& GetModelStore() { return s_Instance->m_ModelStore; }
        ///@}

        /** @name Asset Management */
        ///@{
        /**
         * @brief Retrieves the asset store.
         * @return A vector of references to stored assets.
         */
        static std::vector<Ref<Asset>>& GetAssetStore() { return s_Instance->m_AssetStore; }

        /**
         * @brief Retrieves an asset by path.
         * @param _path The path of the asset.
         * @return A reference to the asset.
         */
        static Ref<Asset> GetAsset(const std::string& _path);
        ///@}

        /** @name Audio Management */
        ///@{
        /**
         * @brief Loads an audio clip from a given asset reference.
         * @param _audioClipAsset The asset reference of the audio clip.
         * @return A reference to the loaded audio clip.
         */
        static Ref<AudioClip> LoadAudioClip(const Ref<Asset>& _audioClipAsset);

        /**
         * @brief Retrieves an audio clip by path.
         * @param _path The path of the audio clip.
         * @return A reference to the audio clip.
         */
        static Ref<AudioClip> GetAudioClip(const std::string& _path);
        ///@}

    private:
        /**
         * @brief Initializes the asset subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the asset subsystem.
         */
        void Deinitialize() override;

        std::unordered_map<std::string, Ref<AudioClip>> m_AudioClipStore; /**< Store for audio clips. */
        std::unordered_map<std::string, Ref<Shader>> m_ShaderStore; /**< Store for shaders. */
        Ref<Shader> m_DefaultShader; /**< Default shader reference. */
        Ref<Shader> m_FramebufferShader; /**< Framebuffer shader reference. */
        std::unordered_map<std::string, Ref<Model>> m_ModelStore; /**< Store for models. */
        std::unordered_map<std::string, Ref<Texture>> m_TextureStore; /**< Store for textures. */
        std::vector<Ref<Asset>> m_AssetStore; /**< Store for assets. */
        std::vector<Ref<Asset>> m_SceneStore; /**< Store for scenes. */
        Ref<Asset> m_StartupScene; /**< Reference to the startup scene. */

        /**
         * @brief Grants the Engine class access to private members.
         */
        friend class Engine;
    };
}
