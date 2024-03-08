#pragma once
#include <string>

class Project
{
public:
	Project() = default;
	virtual ~Project() = default;


protected:
	std::string m_ProjectName;

};