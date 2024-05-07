#include "CollisionDetection.h"

#include "Denix/Physics/PhysicsComponent.h"
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
			switch (otherCol->GetColliderType())
			{
			case ColliderType::Cube:
			{
				// Cube to Cube Collision Detection
				break;
			}
			case ColliderType::Plane:
			{
				// Cube to Plane Collision Detection
				break;
			}
			case ColliderType::Sphere:
			{
				// Cube to Sphere Collision Detection
				break;
			}
			}
		} break;

		case ColliderType::Sphere:
		{
			switch (otherCol->GetColliderType())
			{
			case ColliderType::Cube:
			{
				// Sphere to Cube Collision Detection
				break;
			}
			case ColliderType::Plane:
			{
				// Sphere to Plane Collision Detection
				break;
			}
			case ColliderType::Sphere:
			{
				// Sphere to Sphere Collision Detection
				break;
			}
			}

		} break;

		case ColliderType::Plane:
		{
			switch (otherCol->GetColliderType())
			{
			case ColliderType::Cube:
			{
				// Plane to Cube Collision Detection
				break;
			}
			case ColliderType::Plane:
			{
				// Plane to Plane Collision Detection
				break;
			}
			case ColliderType::Sphere:
			{
				// Plane to Sphere Collision Detection
				break;
			}
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