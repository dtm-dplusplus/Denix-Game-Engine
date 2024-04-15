#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/Mesh.h"

namespace Denix
{
	class
	MeshComponent final : public Component
	{
	public:
		MeshComponent() : Component(ObjectInitializer("Mesh Component")),
			m_Mesh{ nullptr } {}

		MeshComponent(const std::string& _parentName) : Component(_parentName, ObjectInitializer("Mesh Component")),
			m_Mesh{nullptr} {}

		MeshComponent(const std::string& _parentName, const Ref<Mesh>& _mesh) : Component(_parentName, ObjectInitializer("Mesh Component")),
			m_Mesh{ _mesh } {}

		Ref<Mesh> GetMesh() const { return m_Mesh; }
		Ref<Mesh>& GetMesh() { return m_Mesh; }
		void SetMesh(const Ref<Mesh>& _mesh) { m_Mesh = _mesh; }

	private:

		Ref<Mesh> m_Mesh;

		friend class RendererSubsystem;
	};
}
