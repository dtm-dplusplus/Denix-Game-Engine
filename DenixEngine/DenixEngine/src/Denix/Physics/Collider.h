#pragma once

#include <glm/vec4.hpp>

#include "Denix/Core.h"
#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/MeshComponent.h"
#include "Denix/Video/Renderer/RenderComponent.h"
#include <Denix/Scene/Component/TransformComponent.h>

namespace Denix
{
	enum class ColliderType
	{
		// Plane,
		Cube,
		Sphere
	};

	/** Colliders act as basic volumes which can be scaled about their origin */
	class Collider : public Object
	{
	public:

		Collider() : Object({ "Collider" }) 
		{
			m_TransformComponent = MakeRef<TransformComponent>();

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

		Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }
		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }
		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

	protected:
		int m_ColliderType;

		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;

		Ref<TransformComponent> m_TransformComponent;

		//Ref<TransformComponent> m_TransformComponent;
		/** Render component that is used to draw the collider */
		const static glm::vec4 m_NoCollisionColor;
		const static glm::vec4 m_CollisionColor;

		friend class PhysicsSubsystem;
		friend class GameObject;
		friend class PhysicsComponent;
	};

	class CubeCollider : public Collider
	{
	public:
		CubeCollider();
		CubeCollider(Ref<RenderComponent> _renderComponent);

		~CubeCollider() override = default;

		glm::vec3 GetDimensions() const { return m_Dimensions; }
		glm::vec3& GetDimensions() { return m_Dimensions; }

		float GetWidth() const { return m_Dimensions[0]; }
		float& GetWidth() { return m_Dimensions[0]; }

		float GetHeight() const { return m_Dimensions[1]; }
		float& GetHeight() { return m_Dimensions[1]; }

		float GetDepth() const { return m_Dimensions[2]; }
		float& GetDepth() { return m_Dimensions[2]; }

		glm::vec3 GetMin() const { return m_Min; }
		glm::vec3 GetMax() const { return m_Max; }


		void Update(float _deltaTime) override
		{
			m_Min = m_TransformComponent->GetPosition() - m_Dimensions / 2.0f;
			m_Max = m_TransformComponent->GetPosition() + m_Dimensions / 2.0f;
			m_TransformComponent->SetScale(m_Dimensions);

			m_TransformComponent->Update(_deltaTime);
			m_MeshComponent->Update(_deltaTime);
			m_RenderComponent->Update(_deltaTime);
		}

	private:
		glm::vec3 m_Dimensions = {1.0f, 1.0f, 1.0f};

		glm::vec3 m_Min = { -0.5f, -0.5f, -0.5f };
		glm::vec3 m_Max = { 0.5f, 0.5f, 0.5f };

		friend class PhysicsComponent;
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
