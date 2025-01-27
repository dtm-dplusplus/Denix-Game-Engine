#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "TransformPrimitive.h"
#include "Denix/Scene/Component.h"

namespace Denix
{
 class TransformComponent : public Component
 {
 public:
  // Constructors
  TransformComponent();

  // Destructors
  ~TransformComponent() override = default;

  void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

  Transform GetTransform() const { return m_Transform; }
  Transform& GetTransform() { return m_Transform; }
  void SetTransform(const Transform& _transform) { m_Transform = _transform; }

  glm::vec3 GetPosition() const { return m_Transform.Position; }
  glm::vec3& GetPosition() { return m_Transform.Position; }
  void SetPosition(const float _x, const float _y, const float _z) { m_Transform.Position = { _x, _y, _z }; }
  void SetPosition(const glm::vec3& _position) { m_Transform.Position = _position; }
  
  glm::vec3 GetRotation() const { return m_Transform.Rotation; }
  glm::vec3& GetRotation() { return m_Transform.Rotation; }
  void SetRotation(const glm::vec3& _rotation) { m_Transform.Rotation = _rotation; }
  void SetRotation(const float _x, const float _y, const float _z) { m_Transform.Rotation = { _x, _y, _z }; }
  
  glm::vec3 GetScale() const { return m_Transform.Scale; }
  glm::vec3& GetScale() { return m_Transform.Scale; }
  void SetScale(const glm::vec3& _scale) { m_Transform.Scale = _scale; }
  void SetScale(const float _x, const float _y, const float _z) { m_Transform.Scale = { _x, _y, _z }; }

  glm::vec3 GetForward() const { return m_Forward; }
  glm::vec3& GetForward() { return m_Forward; }

  glm::vec3 GetRight() const { return m_Right; }
  glm::vec3& GetRight() { return m_Right; }

  glm::vec3 GetUp() const { return m_Up; }
  glm::vec3& GetUp() { return m_Up; }

  glm::mat4 GetModel() const { return m_Model; }
  glm::mat4& GetModel() { return m_Model; }

 private:
  void Serialize(YAML::Emitter& _out) override;
  void Deserialize(const YAML::Node& _in) override;
  
  Transform m_Transform;

  glm::vec3 m_Forward;
  glm::vec3 m_Right;
  glm::vec3 m_Up;
  
  glm::mat4 m_Model;

  friend class Actor;
  friend class Scene;
  friend class SceneSubsystem;
  friend class PhysicsComponent;
  friend class PhysicsSubsystem;
  friend class RendererSubsystem;
 };
}