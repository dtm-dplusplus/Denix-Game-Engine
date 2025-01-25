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
 struct Transform
 {
  Transform(): Position(glm::vec3(0.0f)), Rotation(glm::vec3(0.0f)), Scale(glm::vec3(1.0f)) {}
  Transform(const glm::vec3& _position, const glm::vec3& _rotation, const glm::vec3& _scale)
   : Position(_position), Rotation(_rotation), Scale(_scale) {}
  
  glm::vec3 Position;
  glm::vec3 Rotation;
  glm::vec3 Scale;
 };

 class TransformComponent : public Component, public std::enable_shared_from_this<TransformComponent>
 {
 public:
  // Constructors
  TransformComponent();

  // Destructors
  ~TransformComponent() override = default;

  void BeginScene() override;
  void EndScene() override;

  void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

  void UpdateRotationVectorFromMatrix();
  Moveability GetMoveability() const { return static_cast<Moveability>(m_Moveability); }
  int& GetMoveabilityI() { return m_Moveability; }
  void SetMoveability(const Moveability _moveability)
  {
   m_Moveability = static_cast<int>(_moveability);
   m_MoveabilityChanged = true;
  }

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
  Transform m_Transform;

  glm::vec3 m_Forward;
  glm::vec3 m_Right;
  glm::vec3 m_Up;
  
  glm::mat4 m_Model;
  int m_Moveability = static_cast<int>(Moveability::Static);

  // Will be a flag in future
  bool m_MoveabilityChanged = false;

  friend class Actor;
  friend class Scene;
  friend class SceneSubsystem;
  friend class PhysicsComponent;
  friend class PhysicsSubsystem;
  friend class RendererSubsystem;
 };
}