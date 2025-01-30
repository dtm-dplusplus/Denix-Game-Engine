#include "PhysicsComponent.h"

#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Core/Reflection/YAMLHelper.h"
#include "Denix/Scene/Actor.h"

namespace Denix
{
    PhysicsComponent::PhysicsComponent(): Component(ObjectInit("Physics Component"))
    {
        m_ClassName = "PhysicsComponent";
    }

    void PhysicsComponent::BeginScene()
    {
        Component::BeginScene();

        // Register the physics component with the physics subsystem
        RegisterComponent();
    }

    void PhysicsComponent::RegisterComponent()
    {
        Component::RegisterComponent();

        if (IsEnabled()) PhysicsSubsystem::RegisterComponent(GetRef<PhysicsComponent>());
    }

    void PhysicsComponent::UnregisterComponent()
    {
        Component::UnregisterComponent();

        if (IsEnabled()) PhysicsSubsystem::UnregisterComponent(GetRef<PhysicsComponent>());
    }

    void PhysicsComponent::Serialize(YAML::Emitter& _out)
    {
        Component::Serialize(_out);

        _out << YAML::Key << "m_SimulatePhysics" << YAML::Value << m_SimulatePhysics;
        _out << YAML::Key << "m_CollisionDetectionEnabled" << YAML::Value << m_CollisionDetectionEnabled;
        _out << YAML::Key << "m_ImpulseEnabled" << YAML::Value << m_ImpulseEnabled;
        _out << YAML::Key << "m_Mass" << YAML::Value << m_Mass;
        _out << YAML::Key << "m_LinearDrag" << YAML::Value << m_LinearDrag;
        _out << YAML::Key << "m_AngularDrag" << YAML::Value << m_AngularDrag;
        _out << YAML::Key << "m_Elasticity" << YAML::Value << m_Elasticity;
        _out << YAML::Key << "m_StaticFriction" << YAML::Value << m_StaticFriction;
        _out << YAML::Key << "m_DynamicFriction" << YAML::Value << m_DynamicFriction;
    }

    void PhysicsComponent::Deserialize(const YAML::Node& _in)
    {
        Component::Deserialize(_in);

        if (const YAML::Node& simulate = _in["m_SimulatePhysics"]; simulate.IsDefined())
            m_SimulatePhysics = simulate.as<bool>();

        if (const YAML::Node& collision = _in["m_CollisionDetectionEnabled"];
            collision.IsDefined())
            m_CollisionDetectionEnabled = collision.as<bool>();

        if (const YAML::Node& impulse = _in["m_ImpulseEnabled"]; impulse.IsDefined())
            m_ImpulseEnabled = impulse.as<bool>();

        if (const YAML::Node& mass = _in["m_Mass"]; mass.IsDefined())
            m_Mass = mass.as<float>();

        if (const YAML::Node& linearDrag = _in["m_LinearDrag"]; linearDrag.IsDefined())
            m_LinearDrag = linearDrag.as<float>();

        if (const YAML::Node& angularDrag = _in["m_AngularDrag"]; angularDrag.IsDefined())
            m_AngularDrag = angularDrag.as<float>();

        if (const YAML::Node& elasticity = _in["m_Elasticity"]; elasticity.IsDefined())
            m_Elasticity = elasticity.as<float>();

        if (const YAML::Node& staticFriction = _in["m_StaticFriction"]; staticFriction.IsDefined())
            m_StaticFriction = staticFriction.as<float>();

        if (const YAML::Node& dynamicFriction = _in["m_DynamicFriction"]; dynamicFriction.IsDefined())
            m_DynamicFriction = dynamicFriction.as<float>();
    }

    void PhysicsComponent::EndScene()
    {
        UnregisterComponent();

        Component::EndScene();
    }

    void PhysicsComponent::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        Component::Update(_deltaTime, _waitCounter);

        if (m_PxActor && m_SimulatePhysics && SceneSubsystem::GetSceneState() == SceneState::Playing)
        {
            physx::PxTransform tform = m_PxActor->getGlobalPose();

            auto parent = m_Parent.lock();
            parent->m_TransformComponent->m_Transform.Position = {tform.p.x, tform.p.y, tform.p.z};
            parent->m_TransformComponent->m_Transform.Rotation = Math::Degrees(
                eulerAngles(glm::quat(tform.q.w, tform.q.x, tform.q.y, tform.q.z)));
        }
    }

    void PhysicsComponent::PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        Component::PostUpdate(_deltaTime, _waitCounter);

        if (!m_PxActor) return;

        // Update physx gravity - This only disables gravity, collisions are still detected
        if (m_AttributeFlags & PHYSICS_SIMULATE)
        {
            if (SimulatePhysics())
            {
                m_PxActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
                if (PhysicsSubsystem::m_PhysicsLogging)
                    DE_LOG(LogPhysics, Trace, "Simulating Physics for {}", GetParent()->GetName())
            }
            else
            {
                m_PxActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
                if (PhysicsSubsystem::m_PhysicsLogging)
                    DE_LOG(LogPhysics, Trace, "Stopping Physics Simulation for {}", GetParent()->GetName())
            }

            // Check Moveability
            if (Ref<TransformComponent> transform = m_Parent.lock()->GetComponent<TransformComponent>())
            {
                if (!m_SimulatePhysics && m_PxActor->is<physx::PxRigidDynamic>())
                {
                    if (physx::PxScene* scene = GetParent()->GetScene()->m_PxScene)
                    {
                        scene->removeActor(*m_PxActor);
                        m_PxActor->release();

                        m_PxActor = PhysicsSubsystem::m_PxPhysics->createRigidStatic(
                            physx::PxTransform(transform->GetPosition().x, transform->GetPosition().y,
                                transform->GetPosition().z));
                        m_PxActor->attachShape(*m_PxShape);
                        scene->addActor(*m_PxActor);
                        if (PhysicsSubsystem::m_PhysicsLogging)
                            DE_LOG(LogPhysics, Trace, "Actor type changed to Static for {}", GetParent()->GetName())
                    }
                }
                else if (m_SimulatePhysics && m_PxActor->is<physx::PxRigidStatic>())
                {
                    if (Ref<Actor> parent = GetParent())
                    {
                        if (Ref<Scene> scene = parent->m_SceneRef.lock())
                        {
                            scene->m_PxScene->removeActor(*m_PxActor);
                            m_PxActor->release();
                            m_PxActor = PhysicsSubsystem::m_PxPhysics->createRigidDynamic(
                                physx::PxTransform(transform->GetPosition().x, transform->GetPosition().y,
                                                   transform->GetPosition().z));
                            m_PxActor->attachShape(*m_PxShape);
                            scene->m_PxScene->addActor(*m_PxActor);
                        }
                    }


                    if (PhysicsSubsystem::m_PhysicsLogging)
                        DE_LOG(LogPhysics, Trace, "Actor type changed to Dynamic for {}", GetParent()->GetName())
                }
            }
            m_AttributeFlags &= ~PHYSICS_SIMULATE;
        }

        // Update physx collision detection
        if (m_AttributeFlags & PHYSICS_COLLISION)
        {
            if (CollisionDetectionEnabled())
            {
                if (m_PxShape)
                {
                    physx::PxShape* shape;
                    m_PxActor->getShapes(&shape, 1);
                    m_PxActor->attachShape(*m_PxShape);
                }


                if (PhysicsSubsystem::m_PhysicsLogging)
                    DE_LOG(LogPhysics, Trace, "Collision Detection Enabled for {}", GetParent()->GetName())
            }
            else
            {
                physx::PxShape* shape;
                m_PxActor->getShapes(&shape, 1);
                m_PxActor->detachShape(*shape);
                if (PhysicsSubsystem::m_PhysicsLogging)
                    DE_LOG(LogPhysics, Trace, "Collision Detection Disabled for {}", GetParent()->GetName())
            }

            m_AttributeFlags &= ~PHYSICS_COLLISION;
        }

        // Update physx mass
        if (m_AttributeFlags & PHYSICS_MASS)
        {
            SetInertia();
            if (PhysicsSubsystem::m_PhysicsLogging)
                DE_LOG(LogPhysics, Trace, "Mass set to {} for {}", GetMass(), GetParent()->GetName())
            m_AttributeFlags &= ~PHYSICS_MASS;
        }

        // Update physx Material
        if (m_AttributeFlags & PHYSICS_MATERIAL)
        {
            if (m_PxMaterial)
            {
                m_PxMaterial->setStaticFriction(m_StaticFriction);
                m_PxMaterial->setDynamicFriction(m_DynamicFriction);
                m_PxMaterial->setRestitution(m_Elasticity);

                physx::PxShape* shape;
                m_PxActor->getShapes(&shape, 1);
                shape->setMaterials(&m_PxMaterial, 1);
            }
            m_AttributeFlags &= ~PHYSICS_MATERIAL;

            if (PhysicsSubsystem::m_PhysicsLogging)
                DE_LOG(LogPhysics, Trace, "Material updated for {}", GetParent()->GetName())
        }

        // Update physx transform & Shape
        if (m_AttributeFlags & PHYSICS_SHAPE)
        {
            if (Ref<TransformComponent> transform = m_Parent.lock()->GetComponent<TransformComponent>())
            {
                physx::PxVec3 scale = {transform->GetScale().x, transform->GetScale().y, transform->GetScale().z};
                physx::PxVec3 scaleHalf = scale * 0.5f;

                // Check Transform
                //physx::PxVec3 rot = {transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z};
                //Math::Degrees(glm::eulerAngles(glm::quat(tform.q.w, tform.q.x, tform.q.y, tform.q.z)));


                const physx::PxGeometryHolder geometry = m_PxShape->getGeometry();

                switch (m_ColliderType)
                {
                case ColliderType::Plane:
                    {
                        physx::PxBoxGeometry box = geometry.box();
                        box.halfExtents = scale;
                        m_PxActor->detachShape(*m_PxShape);
                        m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(box, *m_PxMaterial);
                        m_PxActor->attachShape(*m_PxShape);
                    }
                    break;

                case ColliderType::Cube:
                    {
                        physx::PxBoxGeometry box = geometry.box();
                        box.halfExtents = scaleHalf;
                        m_PxActor->detachShape(*m_PxShape);
                        m_PxShape->release();
                        m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(box, *m_PxMaterial);
                        m_PxActor->attachShape(*m_PxShape);
                    }
                    break;

                case ColliderType::Sphere:
                    {
                        physx::PxSphereGeometry sphere = geometry.sphere();
                        sphere.radius = scaleHalf.x;
                        m_PxActor->detachShape(*m_PxShape);
                        m_PxShape->release();
                        m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(sphere, *m_PxMaterial);
                        m_PxActor->attachShape(*m_PxShape);
                    }
                    break;
                }

                m_PxActor->setGlobalPose(physx::PxTransform(transform->GetPosition().x, transform->GetPosition().y,
                                                            transform->GetPosition().z));
                m_AttributeFlags &= ~PHYSICS_SHAPE;
                if (PhysicsSubsystem::m_PhysicsLogging)
                    DE_LOG(LogPhysics, Trace, "Shape updated for {}", GetParent()->GetName())
            }
        }

        // Update physx transfrom from parent if transform is changed during scene & editor update
        if (SimulatePhysics())
        {
            if (physx::PxRigidDynamic* pxActor = m_PxActor->is<physx::PxRigidDynamic>())
            {
                const glm::vec3& pos = GetParent()->GetTransformComponent()->GetPosition();
                pxActor->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z));
            }
        }
    }

    void PhysicsComponent::AddImpulse(const glm::vec3& _impulse) const
    {
        if (!m_PxActor) return;

        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addForce({_impulse.x, _impulse.y, _impulse.z}, physx::PxForceMode::eIMPULSE);
            return;
        }

        if (PhysicsSubsystem::m_PhysicsLogging)
            DE_LOG(LogPhysics, Error, "Cannot add impulse to a static actor {}", m_Parent.lock()->GetName())
    }

    void PhysicsComponent::AddTorque(const glm::vec3& _torque) const
    {
        if (!m_PxActor) return;

        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addTorque({_torque.x, _torque.y, _torque.z}, physx::PxForceMode::eFORCE);
            return;
        }

        if (PhysicsSubsystem::m_PhysicsLogging)
            DE_LOG(LogPhysics, Error, "Cannot add torque to a static actor {}", m_Parent.lock()->GetName());
    }

    void PhysicsComponent::AddForce(const glm::vec3& _force) const
    {
        if (!m_PxActor) return;

        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addForce({_force.x, _force.y, _force.z}, physx::PxForceMode::eFORCE);
            return;
        }

        if (PhysicsSubsystem::m_PhysicsLogging)
            DE_LOG(LogPhysics, Error, "Cannot add force to a static actor {}", m_Parent.lock()->GetName());
    }

    void PhysicsComponent::AddAcceleration(const glm::vec3& _acceleration) const
    {
        if (!m_PxActor) return;

        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addForce({_acceleration.x, _acceleration.y, _acceleration.z},
                                   physx::PxForceMode::eACCELERATION);
            return;
        }

        if (PhysicsSubsystem::m_PhysicsLogging)
            DE_LOG(LogPhysics, Error, "Cannot add acceleration to a static actor {}", m_Parent.lock()->GetName());
    }

    void PhysicsComponent::SetInertia()
    {
        if (!m_PxShape || !m_PxActor) return;

        const physx::PxGeometryHolder geometry = m_PxShape->getGeometry();

        switch (m_ColliderType)
        {
        case ColliderType::Plane:
            {
                physx::PxBoxGeometry box = geometry.box();

                // Compute inertia tensor for a solid box
                float width = box.halfExtents.x * 2.0f;
                float height = box.halfExtents.y * 2.0f;
                float depth = box.halfExtents.z * 2.0f;

                m_InertiaTensor = {
                    (1.0f / 12.0f) * m_Mass * (height * height + depth * depth),
                    (1.0f / 12.0f) * m_Mass * (width * width + depth * depth),
                    (1.0f / 12.0f) * m_Mass * (width * width + height * height)
                };
            }
            break;

        case ColliderType::Cube:
            {
                physx::PxBoxGeometry box = geometry.box();

                // Compute inertia tensor for a solid box
                float width = box.halfExtents.x * 2.0f;
                float height = box.halfExtents.y * 2.0f;
                float depth = box.halfExtents.z * 2.0f;

                m_InertiaTensor = {
                    (1.0f / 12.0f) * m_Mass * (height * height + depth * depth),
                    (1.0f / 12.0f) * m_Mass * (width * width + depth * depth),
                    (1.0f / 12.0f) * m_Mass * (width * width + height * height)
                };
            }
            break;

        case ColliderType::Sphere:
            {
                physx::PxSphereGeometry sphere = geometry.sphere();

                // Compute inertia tensor for a solid sphere
                float I = (2.0f / 5.0f) * m_Mass * (sphere.radius * sphere.radius);
                m_InertiaTensor = {I, I, I};
            }
            break;
        }

        // Apply m_Mass and inertia tensor
        if (physx::PxRigidDynamic* actor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            actor->setMass(m_Mass);
            actor->setMassSpaceInertiaTensor({m_InertiaTensor.x, m_InertiaTensor.y, m_InertiaTensor.z});
            physx::PxRigidBodyExt::updateMassAndInertia(*actor, m_Mass);
        }
    }
}
