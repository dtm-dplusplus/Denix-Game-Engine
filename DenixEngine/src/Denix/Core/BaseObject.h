/**
 * @file BaseObject.h
 * @brief Defines an object that can be serialized, deserialized, and reflected via the Reflection Subsystem.
 */
#pragma once

#include "Object.h"
#include "Reflection/ReflectionHelper.h"
#include "Thread/ThreadPrimitive.h"

namespace YAML
{
    class Node;
    class Emitter;
}

namespace Denix
{
  /**
     * @class BaseObject
     * @brief Defines an object that can be serialized, deserialized, and reflected via the Reflection Subsystem.
     */
    class BaseObject : public Object, public std::enable_shared_from_this<BaseObject>
    {
    public:
        /**
         * @brief Default constructor.
         */
        BaseObject() = default;

        /**
         * @brief Constructor initializing the object with an initializer.
         * @param _objInit Object initialization parameters.
         */
        BaseObject(const ObjectInit& _objInit): Object(_objInit) {}

        /**
         * @brief Destructor.
         */
        ~BaseObject() override = default;

        /**
         * @brief Gets a shared reference to the object with type casting.
         * @tparam T Target type.
         * @return Shared reference of type T, or nullptr if cast fails.
         */
        template <typename T>
        Ref<T> GetRef()
        {
            if (Ref<BaseObject> ref = shared_from_this())
                if (Ref<T> castRef = std::dynamic_pointer_cast<T>(ref)) return castRef;

            DE_LOG(LogCore, Error, "Failed to cast object to type: {}", ReflectionHelper::GetClassNameDE<T>());
            return nullptr;
        }

        /** @brief Called when the game starts playing. */
        virtual void BeginPlay() {}

        /** @brief Called when the game stops playing. */
        virtual void EndPlay() {}

        /** @brief Pre-update logic before the main update.
         * @param _deltaTime Time since last update.
         * @param _waitCounter Synchronization counter.
         */
        virtual void PreUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) {}

        /** @brief Main update function.
         * @param _deltaTime Time since last update.
         * @param _waitCounter Synchronization counter.
         */
        virtual void Update(float _deltaTime, const Ref<Counter>& _waitCounter) {}

        /** @brief Post-update logic after the main update.
         * @param _deltaTime Time since last update.
         * @param _waitCounter Synchronization counter.
         */
        virtual void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) {}

        /** @brief Called when the scene is set as active. */
        virtual void BeginScene() {}

        /** @brief Called when the scene is no longer active. */
        virtual void EndScene() {}

        /** @brief Marks the object for destruction. */
        virtual void Destroy() { MarkRubbish(); }

        /** @brief Serializes the object to YAML.
         * @param _out YAML emitter for serialization.
         */
        virtual void Serialize(YAML::Emitter& _out);

        /** @brief Deserializes the object from YAML.
         * @param _in YAML node for deserialization.
         */
        virtual void Deserialize(const YAML::Node& _in);
    };
}
