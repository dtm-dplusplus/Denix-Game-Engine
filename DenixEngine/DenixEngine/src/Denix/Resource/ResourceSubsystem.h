#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"

namespace Denix
{
    class Mesh;
    class Model;
    struct ShaderSource;
	class GLShader;
	class Material;
	class Texture;

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

        // Shaders

        static std::unordered_map<std::string, Ref<GLShader>> GetShaderStore() 
        {
            return s_ResourceSubsystem->m_ShaderStore;
        }

        static void AddShader(const Ref<GLShader>& _shader);

        static bool LoadShader(const std::vector<ShaderSource>& _shaders, const std::string& _name);
        
        static Ref<GLShader> GetShader(const std::string& _name);

        static bool ShaderExists(const std::string& _name)
        {
            return s_ResourceSubsystem->m_ShaderStore.contains(_name);
        }
       
        // Materials
        static void AddMaterial(const Ref<Material>& _ref);

        static Ref<Material> GetMaterial(const std::string& _name);

        static std::unordered_map<std::string, Ref<Material>>& GetMaterialStore() { return s_ResourceSubsystem->m_MaterialStore; }

        // Textures
        static void AddTexture(const Ref<Texture>& _texture);
        static Ref<Texture> LoadTexture(const std::string& _path, const std::string& _name);

        static Ref<Texture> GetTexture(const std::string& _name);

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
    public:
        void Initialize() override;

        void Deinitialize() override;

        static ResourceSubsystem* Get() { return s_ResourceSubsystem; }

    private:
        static ResourceSubsystem* s_ResourceSubsystem;

		std::unordered_map<std::string, Ref<GLShader>> m_ShaderStore;

        std::unordered_map<std::string, Ref<Texture>> m_TextureStore;

        std::unordered_map<std::string, Ref<Material>> m_MaterialStore;

        std::unordered_map<std::string, Ref<Mesh>> m_MeshStore;

		std::unordered_map<std::string, Ref<Model>> m_ModelStore;

    };
}