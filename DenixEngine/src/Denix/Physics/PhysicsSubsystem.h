/**
 * @file PhysicsSubsystem.h
 * @brief Declaration of the PhysicsSubsystem class, responsible for handling physics simulation.
 */

#pragma once

#include "Denix/Core/Subsystem.h"
#include "Denix/Scene/Component/PhysicsComponent.h"

#include "PxPhysicsAPI.h"

#define PVD_HOST "127.0.0.1"

namespace Denix
{
    class Scene;

    /**
     * @class PhysicsSubsystem
     * @brief Manages physics simulations using PhysX.
     */
    class PhysicsSubsystem : public Subsystem<PhysicsSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        PhysicsSubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~PhysicsSubsystem() override = default;

        PhysicsSubsystem(const PhysicsSubsystem& _other) = delete;
        PhysicsSubsystem(PhysicsSubsystem&& _other) noexcept = delete;
        PhysicsSubsystem& operator=(const PhysicsSubsystem& _other) = delete;
        PhysicsSubsystem& operator=(PhysicsSubsystem&& _other) noexcept = delete;

        /**
         * @brief Registers a physics component.
         * @param _comp Reference to the physics component.
         */
        static void RegisterComponent(const Ref<PhysicsComponent>& _comp);

        /**
         * @brief Unregisters a physics component.
         * @param _comp Reference to the physics component.
         */
        static void UnregisterComponent(const Ref<PhysicsComponent>& _comp);

        /**
         * @brief Performs a raycast in the physics world.
         * @param _origin Start position of the ray.
         * @param _direction Direction of the ray.
         * @param _distance Maximum distance of the ray.
         * @param _hit PhysX raycast result buffer.
         * @return True if the ray hits an object, false otherwise.
         */
        static bool RayCast(const glm::vec3& _origin, const glm::vec3& _direction, float _distance,
            physx::PxRaycastBuffer& _hit);

        /**
         * @brief Creates a PhysX scene.
         * @param _sceneDesc Description of the scene.
         * @return Pointer to the created PhysX scene.
         */
        static physx::PxScene* CreatePxScene(const physx::PxSceneDesc* _sceneDesc);

        /**
         * @brief Updates physics logging.
         */
        static void PhysicsLoggingUpdate();

        inline static bool m_PhysicsLogging = false; /**< Flag for enabling physics logging. */

    private:
        /**
         * @brief Updates the physics subsystem.
         * @param _deltaTime Time step for update.
         * @param _waitCounter Counter for multithreading synchronization.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        /**
         * @brief Performs post-update physics operations.
         * @param _deltaTime Time step for update.
         * @param _waitCounter Counter for multithreading synchronization.
         */
        void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        /**
         * @brief Initializes the physics subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the physics subsystem.
         */
        void Deinitialize() override;

        /**
         * @brief Sets the active scene for physics simulation.
         * @param _scene Reference to the active scene.
         */
        static void SetActiveScene(const Ref<Scene>& _scene) { s_Instance->m_ActiveScene = _scene; }

        WRef<Scene> m_ActiveScene; /**< Weak reference to the active scene. */
        std::vector<Ref<PhysicsComponent>> m_PhysicsComponents; /**< List of registered physics components. */

        /**
         * @brief PhysX objects for physics simulation.
         */
        inline static physx::PxDefaultAllocator m_PxAllocator;
        inline static physx::PxDefaultErrorCallback m_PxErrorCallback;
        inline static physx::PxFoundation* m_PxFoundation = nullptr;
        inline static physx::PxPhysics* m_PxPhysics = nullptr;
        inline static physx::PxPvd* m_PxPvd = nullptr;
        inline static physx::PxDefaultCpuDispatcher* m_PxDispatcher = nullptr;
        inline static physx::PxMaterial* m_PxMaterial = nullptr;

        friend class PhysicsComponent;
        friend class SceneSubsystem;
        friend class Scene;
        friend class Engine;
    };
}
