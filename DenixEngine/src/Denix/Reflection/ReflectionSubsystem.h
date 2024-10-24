#pragma once

#include <functional>

#include "Denix/Scene/BaseObject.h"
#include "Denix/System/SubSystem.h"

namespace Denix
{
    
    class ReflectionSubsystem: public Subsystem
    {
    public:
        ReflectionSubsystem(): m_CreateFuncs{}
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
       

        template <typename T>
        static std::string GetDEClassName()
        {
            // Remove the first 5 characters of the string "Class "
            std::string className = static_cast<std::string>(typeid(T).name()).substr(6);

            // Remove any namespaces
            while(className.find("::") != std::string::npos)
            {
                className = className.substr(className.find("::") + 2);
            }
            
            return className;
        }
        
        template<typename T>
        static void Register()
        {
            const CreateFunc _createFunc = [] { return MakeRef<T>(); };
            const std::string className = GetDEClassName<T>();
            s_ReflectionSubsystem->m_CreateFuncs[className] = _createFunc;
            DE_LOG(LogScene, Info, "Registered class: {}", className)
        }

        static Ref<BaseObject> Create(const std::string& className)
        {
            if (const auto it = s_ReflectionSubsystem->m_CreateFuncs.find(className); it != s_ReflectionSubsystem->m_CreateFuncs.end()) {
                return it->second();
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