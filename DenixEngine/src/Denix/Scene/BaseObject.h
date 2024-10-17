#pragma once

#include "Object.h"
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/emitter.h>
#include "Denix/Core/YAMLHelper.h"

namespace Denix
{
    class BaseObject: public Object
    {
    public:
        BaseObject(const ObjectInitializer& _objInit = ObjectInitializer::Get()){}
        ~BaseObject() override = default;

        BaseObject(const BaseObject& _other)
            : Object(_other)
        {
        }

        BaseObject(BaseObject&& _other) noexcept
            : Object(_other)
        {
        }

        BaseObject& operator=(const BaseObject& _other)
        {
            if (this == &_other)
                return *this;
            Object::operator =(_other);
            return *this;
        }

        BaseObject& operator=(BaseObject&& _other) noexcept
        {
            if (this == &_other)
                return *this;
            Object::operator =(_other);
            return *this;
        }

      
        // Called each frame if the game is playing
        virtual void BeginPlay() {}
        virtual void EndPlay() {}
        
        virtual void Update(float _deltaTime)  {}

        // Called when the scene is set as active
        virtual void BeginScene() {}
        virtual void EndScene() {}

        virtual void Serialize(YAML::Emitter& _out);
        virtual void Deserialize(const YAML::Node& _in);
    
    };
}
