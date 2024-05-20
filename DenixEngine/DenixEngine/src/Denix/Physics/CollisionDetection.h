#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/GameObject.h"
#include "Denix/Physics/PhysicsComponent.h"

namespace Denix
{
	

	struct CollisionEvent
	{
		CollisionEvent() : IsCollision{false}, Actor(nullptr), Other(nullptr) {}
		
		bool IsCollision;
		
		Ref<GameObject> Actor;
		Ref<GameObject> Other;

		CollisionData ColData;
	};

	class CollisionDetection
	{
	public:
		static bool BroadCollisionDetection(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent);

		static CollisionEvent NarrowCollisionDetection(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent);

		/*
		A signed distance 'd' from a point in space 'p'
		to a plane is calculated by a dot product of the plane's normal vector 'n'
		with the vector '(p-q)', where 'q' is any point on the plane
		*/
		static float DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q);

		/*
		A sphere to a plane collision detection is calculated by finding the parameter between the centre of the sphere c0
		at time step t0 and predict its movement using 'v'. Then find the position of the centre of the sphere using
		the parameter 't'. This also returns new centre point
		*/
		static bool MovingSphereToPlaneCollision(const glm::vec3& n, const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& q, float r, glm::vec3& ci);
		static bool MovingSphereToPlaneCollision(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent);


		/*
		Sphere to sphere collision detection is calculated by finding the distance between the centre of the sphere c0
		and the centre of the sphere c1. This function also finds the contact point cp on the sphere
		*/
		static bool SphereToSphereCollision(const glm::vec3& c0, const glm::vec3 c1, float r1, float r2, glm::vec3& cp);
	private:
		/* Collison Tolerance used in broad phase collision detection when constructing Sphere Collider */
		float m_CollisonTolerance{ 1.0f };

		friend class PhysicsSubsystem;
		friend class EditorSubsystem;
	};

}
