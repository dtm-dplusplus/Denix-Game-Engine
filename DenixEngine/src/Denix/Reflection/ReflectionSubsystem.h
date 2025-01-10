#pragma once

#include <functional>

#include <map>
#include "Denix/Scene/BaseObject.h"
#include "Denix/System/SubSystem.h"
#include "Denix/Reflection/ReflectionHelper.h"

namespace Denix
{
    
    class ReflectionSubsystem: public Subsystem<ReflectionSubsystem>
    {
    public:
        ReflectionSubsystem() = default;
        
        ~ReflectionSubsystem() override = default;

        using CreateFunc = std::function<Ref<Object>()>;
       
        template<typename T>
        static void Register()
        {
            const std::string className = ReflectionHelper::GetClassNameDE<T>();

            static_assert(IsBase<Object, T>(), "Class must be derived from Actor");

            const CreateFunc _createFunc = [] { return MakeRef<T>(); };
            s_Instance->m_CreateFuncs[className] = _createFunc;
        }

        template <typename T = Object>
        static Ref<T> Create(const std::string& _className)
        {
            if (const auto it = s_Instance->m_CreateFuncs.find(_className); it != s_Instance->m_CreateFuncs.end())
            {
               if(Ref<Object> obj = it->second())
               {
                   obj->m_ClassName = _className;
                   return CastRef<T>(obj);
               }
            }
            return nullptr;
        }

       static Ref<Object> GetType(const std::string& _className)
        {
            if (const auto it = s_Instance->m_CreateFuncs.find(_className); it != s_Instance->m_CreateFuncs.end()) {
                return it->second();
            }
            return nullptr;
        }

        static bool ClassExists(const std::string& _className)
        {
            return s_Instance->m_CreateFuncs.contains(_className);
        }
        
        static std::map<std::string, CreateFunc>& GetCreateFuncs() { return s_Instance->m_CreateFuncs; }

    private:
        void Initialize() override;
        void Deinitialize() override;
        
        std::map<std::string, CreateFunc> m_CreateFuncs;

        friend class Engine;
    };
};