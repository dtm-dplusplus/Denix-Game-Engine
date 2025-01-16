#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/Mesh.h"
#include "Denix/Video/GL/Model.h"

namespace Denix
{
	class
	MeshComponent final : public Component
	{
	public:
		MeshComponent() : Component(ObjectInit("Mesh Component")),
			m_Model{ nullptr } {}

		Ref<Model> GetModel() const { return m_Model; }
		Ref<Model>& GetModel() { return m_Model; }
		void SetModel(const Ref<Model>& _model) { m_Model = _model; }

	private:

		Ref<Model> m_Model;

		friend class RendererSubsystem;
	};
}
