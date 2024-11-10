#pragma once
#include <string>

namespace Denix
{
    class ReflectionHelper
    {
    public:

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
    };
    
}
