#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "JobDecleration.h"

#include "JobProfile.h"

Denix::JobDeclaration::JobDeclaration(const std::string& _name, const Priority _priority, Ref<Counter> _waitCounter,
                                      const Ref<JobProfile>& _profile, std::function<void()> _entryPoint): m_ID(s_IDCounter++), m_Name(_name), m_EntryPoint(std::move(_entryPoint)),
                                                                                                           m_Priority(_priority),
                                                                                                           m_WaitCounter(std::move(_waitCounter)), m_JobProfile(_profile)
{
}
