#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Denix/Scene/Component.h"

namespace Denix
{
	class TransformComponent : public Component, public std::enable_shared_from_this<TransformComponent>
	{
	public:
		// Constructors
		TransformComponent() : Component(ObjectInitializer("Transform Component")) {}
		TransformComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Transform Component")) {}

		// Destructors
		~TransformComponent() override = default;

		void BeginScene() override;
		void EndScene() override;

		void RegisterComponent() override;
		void UnregisterComponent() override;
		// Getters
		glm::mat4 GetModel() const { return Model; }
		glm::mat4& GetModel() { return Model; }

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
		glm::vec3 m_Position = glm::vec3(0.f);
		glm::vec3 m_Rotation = glm::vec3(0.f);
		glm::vec3 m_Scale = glm::vec3(1.f);

		glm::mat4 Model = glm::mat4(1.f);
	};
}
