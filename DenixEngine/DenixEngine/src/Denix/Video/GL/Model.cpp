#include "Model.h"

void Denix::Model::RenderModel()
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
	{
		unsigned int materialIndex = m_MeshToTex[i];

		/*if (materialIndex < m_Textures.size() && m_Textures[materialIndex])
		{
		m_Textures[materialIndex]->Bind();
		}*/

		/*m_Meshes[i]->GetVertexArray()->Bind();
		m_Meshes[i]->GetIndexBuffer()->Bind();
		glDrawElements(GL_TRIANGLES, m_Meshes[i]->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);*/

		//RendererSubsystem::DrawImmediate();
	}
}
