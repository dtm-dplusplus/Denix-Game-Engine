#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/Mesh.h"
#include "Denix/Video/GL/Model.h"

namespace Denix
{
	class MeshComponent final : public Component
	{
	public:
		MeshComponent() : Component(ObjectInitializer("Mesh Component")),
			m_Model{ nullptr } {}

		MeshComponent(const std::string& _parentName) : Component(_parentName, ObjectInitializer("Mesh Component")),
			m_Model{ nullptr } {}

		MeshComponent(const std::string& _parentName, const Ref<Model>& _model) : Component(_parentName, ObjectInitializer("Mesh Component")),
			m_Model{ nullptr } {}

		void DrawModel();
		

		Ref<Model> GetModel() const { return m_Model; }
		Ref<Model>& GetModel() { return m_Model; }
		void SetModel(const Ref<Model>& _model) { m_Model = _model; }

	private:

		Ref<Model> m_Model;

		friend class RendererSubsystem;
	};
}
