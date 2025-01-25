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
     *  BaseObject defines an object that can be serialized, deserialized, and reflected via th Reflection Subsystem
     *  
     */
    class BaseObject: public Object
    {
    public:
        BaseObject() = default;
        BaseObject(const ObjectInit& _objInit): Object(_objInit){}
        /*~BaseObject() override = default;*/

        
        // Called each frame if the game is playing
        virtual void BeginPlay() {}
        virtual void EndPlay() {}

        virtual void PreUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) {}
        virtual void Update(float _deltaTime, const Ref<Counter>& _waitCounter) {}
        virtual void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) {}
        
        // Called when the scene is set as active
        virtual void BeginScene() {}
        virtual void EndScene() {}

        
        virtual void Serialize(YAML::Emitter& _out);
        virtual void Deserialize(const YAML::Node& _in);
    };
}
