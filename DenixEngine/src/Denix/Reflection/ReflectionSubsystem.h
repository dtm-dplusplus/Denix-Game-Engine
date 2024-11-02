#pragma once

#include <functional>

#include <map>
#include "Denix/Scene/BaseObject.h"
#include "Denix/System/SubSystem.h"
#include "Denix/Reflection/ReflectionHelper.h"

namespace Denix
{
    
    class ReflectionSubsystem: public Subsystem
    {
    public:
        ReflectionSubsystem()
        {
            s_ReflectionSubsystem = this;
            DE_LOG_CREATE(LogReflection)
        }
        
        ~ReflectionSubsystem() override = default;

        static ReflectionSubsystem* Get()
        {
            return s_ReflectionSubsystem;
        }
        using CreateFunc = std::function<Ref<BaseObject>()>;
       

       
        
        template<typename T>
        static void Register()
        {
            const CreateFunc _createFunc = [] { return MakeRef<T>(); };
            const std::string className = ReflectionHelper::GetDEClassName<T>();
            s_ReflectionSubsystem->m_CreateFuncs[className] = _createFunc;
            DE_LOG(LogScene, Info, "Registered class: {}", className)
        }

        static Ref<BaseObject> Create(const std::string& className)
        {
            if (const auto it = s_ReflectionSubsystem->m_CreateFuncs.find(className); it != s_ReflectionSubsystem->m_CreateFuncs.end()) {
               if(Ref<BaseObject> obj = it->second())
               {
                   obj->m_ClassName = className;
                   return obj;
               }
            }
            return nullptr;
        }

        Ref<BaseObject> GetType(const std::string& _className)
        {
            if (const auto it = m_CreateFuncs.find(_className); it != m_CreateFuncs.end()) {
                return it->second();
            }
            return nullptr;
        }
        
        static std::map<std::string, CreateFunc>& GetCreateFuncs() { return s_ReflectionSubsystem->m_CreateFuncs; }
        
        void Initialize() override;
        void Deinitialize() override;

    private:
        static ReflectionSubsystem* s_ReflectionSubsystem;
        
        std::map<std::string, CreateFunc> m_CreateFuncs;
    };
};