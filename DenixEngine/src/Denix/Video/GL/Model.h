#pragma once

#include "Denix/Core.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Denix/Asset/Asset.h"
#include "Denix/Video/GL/Mesh.h"

namespace Denix
{
	class Mesh;
	class Texture;

	class Model : public Asset
	{
	public:
		Model(const AssetInit& _assetInit);

		~Model() override;

		bool LoadModel();
		void RenderModel();
		
	//private:

		void LoadNode(aiNode* _node, const aiScene* _scene);
		void LoadMesh(aiMesh* _mesh, const aiScene* _scene);

		void LoadMaterials(const aiScene* _scene);

		bool m_IsLoaded = false;
		std::vector<Ref<Mesh>> m_Meshes;
		std::vector<Ref<Texture>> m_Textures;
		std::vector<unsigned int> m_MeshToTex;
	};
}

