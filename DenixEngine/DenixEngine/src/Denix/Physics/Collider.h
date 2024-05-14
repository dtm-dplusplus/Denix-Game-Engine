#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/MeshComponent.h"
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
		}

		~Collider() override = default;

		void Update(float _deltaTime) override
		{
			m_TransformComponent->Update(_deltaTime);
		}

		ColliderType GetColliderType() { return (ColliderType)m_ColliderType; }
		ColliderType& GetColliderTypeRef() { return (ColliderType&)m_ColliderType; }
		void SetColliderType(ColliderType _type) { m_ColliderType = (int)_type; }

		Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }
		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }
	protected:
		int m_ColliderType;

		Ref<MeshComponent> m_MeshComponent;

		Ref<TransformComponent> m_TransformComponent;

		//Ref<TransformComponent> m_TransformComponent;
		/** Render component that is used to draw the collider */
		
		friend class PhysicsSubsystem;
		friend class GameObject;
		friend class PhysicsComponent;
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

		glm::vec3 GetMin() const { return m_Min; }
		glm::vec3 GetMax() const { return m_Max; }


		void Update(float _deltaTime) override
		{
			m_Min = m_TransformComponent->GetPosition() - m_Dimensions / 2.0f;
			m_Max = m_TransformComponent->GetPosition() + m_Dimensions / 2.0f;
			m_TransformComponent->SetScale(m_Dimensions);

			m_TransformComponent->Update(_deltaTime);
			m_MeshComponent->Update(_deltaTime);
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
		float m_Radius = 0.5f;
	};
}
