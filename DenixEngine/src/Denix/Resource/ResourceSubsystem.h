#pragma once

#include <yaml-cpp/node/node.h>

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"

namespace Denix
{
    class Asset;
    class Mesh;
    class Model;
    struct ShaderSource;
	class Shader;
	class Material;
	class Texture;
    class Scene;

    class ResourceSubsystem : public Subsystem
    {
    public:
        ResourceSubsystem()
        {
            s_ResourceSubsystem = this;
			DE_LOG_CREATE(LogResource)
        }

        ~ResourceSubsystem() override
        {
            s_ResourceSubsystem = nullptr;
        }


        // Scenes
        static std::vector<Ref<Asset>>  GetSceneStore() { return s_ResourceSubsystem->m_SceneStore; }
        static Ref<Asset> GetSceneAsset(const std::string& _path);
        
        // Shaders
        static std::unordered_map<std::string, Ref<Shader>> GetShaderStore() 
        {
            return s_ResourceSubsystem->m_ShaderStore;
        }

        static void AddShader(const Ref<Shader>& _shader);

        static bool LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _name);
        static bool ReloadShader(Ref<Shader>& _shader);
        
        static Ref<Shader> GetShader(const std::string& _name);

        static bool ShaderExists(const std::string& _name)
        {
            return s_ResourceSubsystem->m_ShaderStore.contains(_name);
        }
       
        // Materials
        static void AddMaterial(const Ref<Material>& _ref);

        static Ref<Material> GetMaterial(const std::string& _path);

        static std::unordered_map<std::string, Ref<Material>>& GetMaterialStore() { return s_ResourceSubsystem->m_MaterialStore; }

        // Textures
        static void AddTexture(const Ref<Texture>& _texture);
        static Ref<Texture> LoadTexture(const std::string& _path);

        static Ref<Texture> GetTexture(const std::string& _path);
        static Ref<Texture> GetTextureByPath(const std::string& _path);

        static std::unordered_map<std::string, Ref<Texture>>& GetTextureStore() { return s_ResourceSubsystem->m_TextureStore; }

        // Meshes
        static bool AddMesh(const Ref<Mesh>& _mesh);
        static bool LoadMesh(const std::string& _name, const float* _vertices, const unsigned int* _indices, unsigned int _verticesCount, unsigned int _numOfIndices);

        static Ref<Mesh> GetMesh(const std::string& _name);

        // Models
        static bool AddModel(const Ref<Model>& _mesh);
        static bool LoadModel(const std::string& _name, const std::string& _path);
        static Ref<Model> GetModel(const std::string& _name);
		static std::unordered_map<std::string, Ref<Model>>& GetModelStore() { return s_ResourceSubsystem->m_ModelStore; }

        static std::unordered_map<std::string, Ref<Mesh>>& GetMeshStore() { return s_ResourceSubsystem->m_MeshStore; }

        static std::vector<Ref<Asset>>& GetAssetStore() { return s_ResourceSubsystem->m_AssetStore; }
        static Ref<Asset> GetAsset(const std::string& _path);
    public:
        void Initialize() override;

        void Deinitialize() override;

        static ResourceSubsystem* Get() { return s_ResourceSubsystem; }

    private:
        static ResourceSubsystem* s_ResourceSubsystem;

		std::unordered_map<std::string, Ref<Shader>> m_ShaderStore;


        std::unordered_map<std::string, Ref<Material>> m_MaterialStore;

        std::unordered_map<std::string, Ref<Mesh>> m_MeshStore;

		std::unordered_map<std::string, Ref<Model>> m_ModelStore;

        std::unordered_map<std::string, Ref<Texture>> m_TextureStore;
        
        std::vector<Ref<Asset>> m_AssetStore;

        std::vector<Ref<Asset>> m_SceneStore;

        // TODO: Implement this
        /*Ref<Asset> GetShaderType(const std::string& _source) const
        {
            if (const auto keywordIt = _source.find_first_of(g_SHADER_KEYWORD))
            {
                std::string type;

                for (auto typeIt = keywordIt + g_SHADER_KEYWORD_OFFSET;
                    _source[typeIt] != ' ' && _source[typeIt] != '\n'; typeIt++)
                {
                    type += _source[typeIt];
                }

                // Return the type if we find it
                for (const auto& [StringType, EnumType] : g_SHADER_TYPES)
                    if (type == StringType) return EnumType;

            }

            DE_LOG(LogShader, Error, "Shader Keyword DE_SHADER not found or invalid type")

            return GL_FALSE;
        }*/
        
    };
}