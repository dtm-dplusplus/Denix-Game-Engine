#include "CollisionDetection.h"

#include "Denix/Physics/PhysicsComponent.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Physics/Collider.h"

namespace Denix
{
	bool CollisionDetection::BroadCollisionDetection(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent)
	{
		// Create a bounding sphere for the physics component

		return false;
	}

	CollisionEvent CollisionDetection::NarrowCollisionDetection(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent)
	{
		// Narrow Collision Detection
		Ref<Collider> col = _component->GetCollider();
		Ref<Collider> otherCol = _otherComponent->GetCollider();
		CollisionEvent collisionEvent;

		if (!col || !otherCol) return collisionEvent;


		switch (col->GetColliderType())
		{
		case ColliderType::Cube:
		{
			Ref<CubeCollider> cubeCol = std::dynamic_pointer_cast<CubeCollider>(col);
			glm::vec3 minA = cubeCol->GetMin();
			glm::vec3 maxA = cubeCol->GetMax();
			

			switch (otherCol->GetColliderType())
			{
			case ColliderType::Cube:
			{
				// Cube to Cube Collision Detection
				Ref<CubeCollider> otherCubeCol = std::dynamic_pointer_cast<CubeCollider>(otherCol);
				glm::vec3 minB = otherCubeCol->GetMin();
				glm::vec3 maxB = otherCubeCol->GetMax();


				

				if (minA.x <= maxB.x && maxA.x >= minB.x &&
					minA.y <= maxB.y && maxA.y >= minB.y &&
					minA.z <= maxB.z && maxA.z >= minB.z)
				{
					collisionEvent.IsCollision = true;
					collisionEvent.Actor = SceneSubsystem::GetActiveScene()->GetGameObject(_component->GetParentObjectName());
					collisionEvent.Other = SceneSubsystem::GetActiveScene()->GetGameObject(_otherComponent->GetParentObjectName());
				}
				
			} break;
			
			case ColliderType::Sphere:
			{
				// Cube to Sphere Collision Detection
			}  break;
			}
		} break;

		case ColliderType::Sphere:
		{
			Ref<SphereCollider> sphereCol = std::dynamic_pointer_cast<SphereCollider>(col);

			switch (otherCol->GetColliderType())
			{
			case ColliderType::Cube:
			{
				// Sphere to Cube Collision Detection
				Ref<CubeCollider> otherCubeCol = std::dynamic_pointer_cast<CubeCollider>(otherCol);

				glm::vec3 minB = otherCubeCol->GetMin();
				glm::vec3 maxB = otherCubeCol->GetMax();

				// Check left, right, front, back, top
				/*if (maxB.x < sphereCol->GetTransformComponent()->GetPosition().x - sphereCol->GetRadius() ||
					minB.x > sphereCol->GetTransformComponent()->GetPosition().x + sphereCol->GetRadius() ||
					maxB.z < sphereCol->GetTransformComponent()->GetPosition().z - sphereCol->GetRadius() ||
					minB.z > sphereCol->GetTransformComponent()->GetPosition().z + sphereCol->GetRadius() ||
					maxB.y < sphereCol->GetTransformComponent()->GetPosition().y - sphereCol->GetRadius() ||
					minB.y > sphereCol->GetTransformComponent()->GetPosition().y + sphereCol->GetRadius())
				{
					collisionEvent.IsCollision = true;
					collisionEvent.Actor = SceneSubsystem::GetActiveScene()->GetGameObject(_component->GetParentObjectName());
					collisionEvent.Other = SceneSubsystem::GetActiveScene()->GetGameObject(_otherComponent->GetParentObjectName());
				}*/
				break;
			}
			
			case ColliderType::Sphere:
			{
				// Sphere to Sphere Collision Detection
				Ref<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(otherCol);

				collisionEvent.IsCollision = SphereToSphereCollision(sphereCol->GetTransformComponent()->GetPosition(),
					otherSphereCol->GetTransformComponent()->GetPosition(), otherSphereCol->GetRadius(), otherSphereCol->GetRadius(), collisionEvent.ColData.ContactPoint);
				
				if (collisionEvent.IsCollision)
				{
					collisionEvent.Actor = SceneSubsystem::GetActiveScene()->GetGameObject(_component->GetParentObjectName());
					collisionEvent.Other = SceneSubsystem::GetActiveScene()->GetGameObject(_otherComponent->GetParentObjectName());
				}
	
				break;
			}

			default: break;
			}

		} break;
		}

		return collisionEvent;
	}

	float CollisionDetection::DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q)
	{
		float d = glm::dot((p - q), n);
		return d;
	}

	bool CollisionDetection::MovingSphereToPlaneCollision(const glm::vec3& n, const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& q, float r, glm::vec3& ci)
	{
		float t;

		float d0 = DistanceToPlane(n, c0, q);
		float d1 = DistanceToPlane(n, c1, q);

		if (glm::abs(d0) <= r)
		{
			ci = c0;
			t = 0.0f;
			return true;
		}
		if (d0 > r && d1 < r)
		{
			t = (d0 - r) / (d0 - d1);
			ci = (1 - t) * c0 + t * c1;
			return true;
		}
		return false;
	}


	bool CollisionDetection::SphereToSphereCollision(const glm::vec3& c0, const glm::vec3 c1, float r1, float r2, glm::vec3& cp)
	{
		float d = glm::length(c0 - c1);
		glm::vec3 n;

		if (d <= (r1 + r2))
		{
			n = glm::normalize(c0 - c1);
			cp = r1 * n;
			return true;
		}
		return false;
	}



}