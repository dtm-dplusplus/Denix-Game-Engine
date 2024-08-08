#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/Object.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace Denix
{
	class Mesh;
	class Texture;

	/*struct aiNode;
	struct aiMesh;
	struct aiScene;*/

	class Model : public Object
	{
	public:
		Model(const ObjectInitializer& _objInit = {"Model"}): Object(_objInit) {}

		Model(const std::string& _name, const std::string& _path) :
			Object(ObjectInitializer(_name)), m_Path(_path) 
		{
			LoadModel(_path);
		}

		~Model(){}

		bool LoadModel(const std::string& _path);
		void RenderModel();
		void ClearModel()
		{
			//for (unsigned int i = 0; i < m_Meshes.size(); i++)
			//{
			//	if (m_Meshes[i])
			//	{
			//		//m_Meshes[i]->ClearMesh();
			//	}
			//}
			//
			//for (const auto& m_Texture : m_Textures)
			//{
			//	if (m_Texture)
			//	{
			//		m_Texture->ClearTexture();
			//	}
			//}
		}
	//private:

		void LoadNode(aiNode* _node, const aiScene* _scene);
		void LoadMesh(aiMesh* _mesh, const aiScene* _scene);

		void LoadMaterials(const aiScene* _scene);

		bool m_IsLoaded = false;
		std::string m_Path;
		std::vector<Ref<Mesh>> m_Meshes;
		std::vector<Ref<Texture>> m_Textures;
		std::vector<unsigned int> m_MeshToTex;
	};
}

