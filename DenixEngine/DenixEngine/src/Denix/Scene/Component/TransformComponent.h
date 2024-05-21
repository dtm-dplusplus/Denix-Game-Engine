#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Denix/Scene/Component.h"

namespace Denix
{
	/** Moveability defines the complexity of logic performed on object data. It does not explicity determine the object's ability to move.
		* Static is the default value for all objects and uses the least amount of resources.
		* Dynamic is the most complex and uses the most resources.
		*
		*	@Physics
		*	Static: The object is a stationary collidable. There is no need to sweep or check for collisions
		*	Dynamic: The object is a collidable, affected by external forces and can move. It requires collision detection and resolution.
		*
		*	@Lighting - (Currently all light sources are dynamic in nature. Static lighting will be supported shortly)
		*	Static: The object is a stationary light source. It does not need to be updated every frame.
		*	Dynamic: The object is a light source that can move. It requires updating every frame.
		*/
	enum class Moveability
	{
		Static,
		Dynamic
	};

	class TransformComponent : public Component, public std::enable_shared_from_this<TransformComponent>
	{
	public:
		// Constructors
		TransformComponent() : Component(ObjectInitializer("Transform Component")) {}
		TransformComponent(const std::string& _parentName) : Component(_parentName, ObjectInitializer("Transform Component")) {}

		// Destructors
		~TransformComponent() override = default;

		void BeginScene() override;
		void EndScene() override;

		void RegisterComponent() override;
		void UnregisterComponent() override;


		void Update(float _deltaTime) override
		{
			// Update Rotation Matrix
			glm::mat4 rotationMatrix = glm::mat4(m_RotationMatrix);
			m_RotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
			m_RotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
			m_RotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));

			// Update the m_Model matrix
			m_Model = glm::translate(glm::mat4(1.0f), m_Position);
			m_Model *= rotationMatrix;
			m_Model = glm::scale(m_Model, m_Scale);
		}

		void UpdateRotationVectorFromMatrix()
		{
			// Update Rotation Vector
			m_Rotation.x = fmod(m_Rotation.x, 360.f);
			m_Rotation.y = fmod(m_Rotation.y, 360.f);
			m_Rotation.z = fmod(m_Rotation.z, 360.f);
		}
		Moveability GetMoveability() const { return static_cast<Moveability>(m_Moveability); }
		int& GetMoveabilityI() { return m_Moveability; }
		void SetMoveability(const Moveability _moveability)
		{
			m_Moveability = static_cast<int>(_moveability);
			m_MoveabilityChanged = true;
		}

		glm::mat4 GetModel() const { return m_Model; }
		glm::mat4& GetModel() { return m_Model; }

		glm::vec3 GetPosition() const { return m_Position; }
		glm::vec3& GetPosition() { return m_Position; }

		glm::vec3 GetRotation() const { return m_Rotation; }
		glm::vec3& GetRotation() { return m_Rotation; }

		glm::vec3 GetScale() const { return m_Scale; }
		glm::vec3& GetScale() { return m_Scale; }

		// Setters
		void SetPosition(const glm::vec3& _position) { m_Position = _position; }
		void SetRotation(const glm::vec3& _rotation) { m_Rotation = _rotation; }
		void SetScale(const glm::vec3& _scale) { m_Scale = _scale; }

	protected:
		
		int m_Moveability = static_cast<int>(Moveability::Static);

		// Will be a flag in future
		bool m_MoveabilityChanged = false;

		glm::vec3 m_Position = glm::vec3(0.f);
		glm::vec3 m_Rotation = glm::vec3(0.f);
		glm::vec3 m_Scale = glm::vec3(1.f);

		glm::mat4 m_Model = glm::mat4(1.f);
		glm::mat3 m_RotationMatrix = glm::mat3(1.f);
		friend class GameObject;
		friend class PhysicsComponent;
		friend class PhysicsSubsystem;
	};
}
