#pragma once

#include "Denix/Core.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/MeshComponent.h"
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"

namespace Denix
{

	class Model
	{
	public:
		Model(){}
		~Model(){}

		bool LoadModel(const std::string& _path)
		{
			DE_LOG(Log, Trace, "Loading Model: {0}", _path)
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				DE_LOG(Log, Error, "ERROR::ASSIMP::{0}", importer.GetErrorString())
				return false;
			}

			LoadNode(scene->mRootNode, scene);
			LoadMaterials(scene);

			return true;
		}
		void RenderModel()
		{
			for (unsigned int i = 0; i < m_Meshes.size(); i++)
			{
				unsigned int materialIndex = m_MeshToTex[i];

				if (materialIndex < m_Textures.size() && m_Textures[materialIndex])
				{
					m_Textures[materialIndex]->Bind();
				}

				m_Meshes[i]->GetVertexArray()->Bind();
				m_Meshes[i]->GetIndexBuffer()->Bind();
				glDrawElements(GL_TRIANGLES, m_Meshes[i]->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
		}
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

		void LoadNode(aiNode* _node, const aiScene* _scene)
		{
			for (unsigned int i = 0; i < _node->mNumMeshes; i++)
			{
				LoadMesh(_scene->mMeshes[_node->mMeshes[i]], _scene);
			}

			for (unsigned int i = 0; i < _node->mNumChildren; i++)
			{
				LoadNode(_node->mChildren[i], _scene);
			}
		}
		void LoadMesh(aiMesh* _mesh, const aiScene* _scene)
		{
			std::vector<float> vertices;
			std::vector<unsigned int> indices;

			for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
			{
				vertices.insert(vertices.end(), { _mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z });
				if (_mesh->mTextureCoords[0]) vertices.insert(vertices.end(), { _mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y });
				else vertices.insert(vertices.end(), { 0.0f, 0.0f });
				vertices.insert(vertices.end(), { -_mesh->mNormals[i].x, -_mesh->mNormals[i].y, -_mesh->mNormals[i].z });
			}

			for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
			{
				aiFace face = _mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			Ref<MeshComponent> mesh = MakeRef<MeshComponent>();
			mesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
			m_Meshes.push_back(mesh);
			m_MeshToTex.push_back(_mesh->mMaterialIndex);
		}

		void LoadMaterials(const aiScene* _scene)
		{
			m_Textures.resize(_scene->mNumMaterials);

			for (unsigned int i = 0; i < _scene->mNumMaterials; i++)
			{
				aiMaterial* material = _scene->mMaterials[i];
				m_Textures[i] = nullptr;

				if (material->GetTextureCount(aiTextureType_DIFFUSE))
				{
					aiString path;
					if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
					{
						int idx = std::string(path.data).rfind("\\");
						std::string filename = std::string(path.data).substr(idx + 1);
						std::string texPath = FileSubsystem::GetContentRoot() + R"(Textures\)" + filename;

						m_Textures[i] = MakeRef<Texture>(texPath, filename);
						DE_LOG(Log, Trace, "Loaded Texture: {}", texPath)
						if (!m_Textures[i]->LoadTexture())
						{
							m_Textures[i] = nullptr;
							m_MeshToTex[i] = -1;
						}
					}
				}

				if (!m_Textures[i])
				{
					DE_LOG(Log, Error, "Texture failed to load")
					m_Textures[i] = ResourceSubsystem::GetTexture("DefaultTexture");
					m_Textures[i]->LoadTexture();
				}
			}	
		}

		std::vector<Ref<MeshComponent>> m_Meshes;
		std::vector<Ref<Texture>> m_Textures;
		std::vector<unsigned int> m_MeshToTex;
	};
}

