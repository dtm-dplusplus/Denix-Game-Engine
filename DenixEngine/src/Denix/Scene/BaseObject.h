#pragma once

#include "Object.h"
#include "Denix/Reflection/ReflectionHelper.h"

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
        BaseObject() : Object(ObjectInitializer::Get()), m_ClassName("BaseObject")
        {
        }

        BaseObject(const ObjectInitializer& _objInit): Object(_objInit), m_ClassName("BaseObject") {}
        ~BaseObject() override = default;

        
        // Called each frame if the game is playing
        virtual void BeginPlay() {}
        virtual void EndPlay() {}
        
        virtual void Update(float _deltaTime)  {}

        // Called when the scene is set as active
        virtual void BeginScene() {}
        virtual void EndScene() {}

        
        virtual void Serialize(YAML::Emitter& _out);
        virtual void Deserialize(const YAML::Node& _in);

    protected:
        std::string m_ClassName;

        friend class ReflectionSubsystem;
    };
}
