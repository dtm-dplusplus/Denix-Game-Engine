#include "MeshComponent.h"

namespace Denix
{
	void MeshComponent::DrawModel()
	{
		if (m_Model)
		{
			for (unsigned int i = 0; i < m_Model->m_Meshes.size(); i++)
			{
				if (m_Model->m_Meshes[i]->m_VAO && m_Model->m_Meshes[i]->m_IBO)
				{
					m_Model->m_Meshes[i]->m_VAO->Bind();
					m_Model->m_Meshes[i]->m_IBO->Bind();
					glDrawElements(GL_TRIANGLES, m_Model->m_Meshes[i]->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
				}
			}
		}
	}
}

