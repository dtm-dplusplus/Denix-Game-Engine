#pragma once

#include "Denix/Core/Subsystem.h"
#include "Denix/Scene/Component/PhysicsComponent.h"

#include "PxPhysicsAPI.h"

#define PVD_HOST "127.0.0.1"

namespace Denix
{
    class Scene;

    class PhysicsSubsystem : public Subsystem<PhysicsSubsystem>
    {
    public:
        PhysicsSubsystem() = default;
        ~PhysicsSubsystem() override = default;

        PhysicsSubsystem(const PhysicsSubsystem& _other) = delete;
        PhysicsSubsystem(PhysicsSubsystem&& _other) noexcept = delete;
        PhysicsSubsystem& operator=(const PhysicsSubsystem& _other) = delete;
        PhysicsSubsystem& operator=(PhysicsSubsystem&& _other) noexcept = delete;

        static void RegisterComponent(const Ref<PhysicsComponent>& _comp);
        static void UnregisterComponent(const Ref<PhysicsComponent>& _comp);

        static bool RayCast(const glm::vec3& _origin, const glm::vec3& _direction, float _distance,
                            physx::PxRaycastBuffer& _hit);

        // @TODO This all needs to be private
        inline static physx::PxDefaultAllocator m_PxAllocator;
        inline static physx::PxDefaultErrorCallback m_PxErrorCallback;
        inline static physx::PxFoundation* m_PxFoundation = nullptr;
        inline static physx::PxPhysics* m_PxPhysics = nullptr;
        inline static physx::PxPvd* m_PxPvd = nullptr;
        inline static physx::PxDefaultCpuDispatcher* m_PxDispatcher = nullptr;
        inline static physx::PxMaterial* m_PxMaterial = nullptr;

        static physx::PxScene* CreatePxScene(const physx::PxSceneDesc* _sceneDesc);

        static void PhysicsLoggingUpdate();
        inline static bool m_PhysicsLogging = false;

    private:
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
        void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        void Initialize() override;
        void Deinitialize() override;

        static void SetActiveScene(const Ref<Scene>& _scene) { s_Instance->m_ActiveScene = _scene; }

        WRef<Scene> m_ActiveScene;
        std::vector<Ref<PhysicsComponent>> m_PhysicsComponents;

        friend class PhysicsComponent;
        friend class SceneSubsystem;
        friend class Engine;
    };
}
