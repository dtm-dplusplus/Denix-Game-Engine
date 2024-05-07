#pragma once

#include <glm/vec4.hpp>

#include "Denix/Core.h"
#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/MeshComponent.h"
#include "Denix/Video/Renderer/RenderComponent.h"
//#include <Denix/Scene/Component/TransformComponent.h>

namespace Denix
{
	enum class ColliderType
	{
		Plane,
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
		int m_ColliderType;

		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;

		//Ref<TransformComponent> m_TransformComponent;
		/** Render component that is used to draw the collider */
		const static glm::vec4 m_NoCollisionColor;
		const static glm::vec4 m_CollisionColor;


		friend class PhysicsSubsystem;
		friend class GameObject;
		friend class PhysicsComponent;
	};

	class PlaneCollider : public Collider
	{
	public:
		PlaneCollider();

		~PlaneCollider() override = default;

		glm::vec3 GetNormal() const { return m_Normal; }
		glm::vec3& GetNormal() { return m_Normal; }

		float GetDistance() const { return m_Distance; }
		float& GetDistance() { return m_Distance; }

	private:
		glm::vec3 m_Normal = { 0.0f, 1.0f, 0.0f };
		float m_Distance = 0.0f;
	};
	class CubeCollider : public Collider
	{
	public:
		CubeCollider();
		

		~CubeCollider() override = default;

		glm::vec3 GetDimensions() const { return m_Dimensions; }
		glm::vec3& GetDimensions() { return m_Dimensions; }

		float GetWidth() const { return m_Dimensions[0]; }
		float& GetWidth() { return m_Dimensions[0]; }

		float GetHeight() const { return m_Dimensions[1]; }
		float& GetHeight() { return m_Dimensions[1]; }

		float GetDepth() const { return m_Dimensions[2]; }
		float& GetDepth() { return m_Dimensions[2]; }

	private:
		glm::vec3 m_Dimensions = {1.0f, 1.0f, 1.0f};
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider();

		~SphereCollider() override = default;

		float GetRadius() const { return m_Radius; }
		float& GetRadius() { return m_Radius; }

	private:
		float m_Radius = 1.0f;
	};
}
