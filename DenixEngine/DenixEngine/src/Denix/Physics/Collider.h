#pragma once

#include <glm/vec4.hpp>

#include "Denix/Core.h"
#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/MeshComponent.h"
#include "Denix/Video/Renderer/RenderComponent.h"

namespace Denix
{
	enum class ColliderType
	{
		Cube,
		Sphere
	};

	/** Colliders act as basic volumes which can be scaled about their origin */
	class Collider : public Object
	{
	public:

		Collider() : Object({ "Collider" }) 
		{
			m_MeshComponent = MakeRef<MeshComponent>();
			m_RenderComponent = MakeRef<RenderComponent>();
			m_RenderComponent->SetIsVisible(false);
			m_RenderComponent->SetAffectsLighting(false);
			m_RenderComponent->SetBaseColorAsTexture(true);
			m_RenderComponent->SetMaterial(MakeRef<Material>());
			m_RenderComponent->GetMaterial()->SetBaseColor(m_NoCollisionColor);
		}

		~Collider() override = default;

		ColliderType GetColliderType() { return (ColliderType)m_ColliderType; }
		ColliderType& GetColliderTypeRef() { return (ColliderType&)m_ColliderType; }
		void SetColliderType(ColliderType _type) { m_ColliderType = (int)_type; }
		
		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }
		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

	protected:
		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;

		/** Render component that is used to draw the collider */
		const static glm::vec4 m_NoCollisionColor;
		const static glm::vec4 m_CollisionColor;


		int m_ColliderType;
		friend class PhysicsSubsystem;
		friend class GameObject;
		friend class PhysicsComponent;
	};

	class CubeCollider : public Collider
	{
	public:
		CubeCollider();
		

		~CubeCollider() override = default;

	private:

	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider();

		~SphereCollider() override = default;

	private:
		float m_Radius;
	};
}
