//
// PropertySystem.cpp
//
 
#include "PropertySystem.h"

#include "RTTI.h"

list<CAbstractProperty*> CPropertySystem::ms_Properties;

list<CRTTI*> CPropertySystem::ms_Classes;

CPropertySystem::CPropertySystem()
{
}

CPropertySystem::~CPropertySystem()
{
	// Destroy properties
	for ( list<CAbstractProperty*>::iterator it = ms_Properties.begin(); it != ms_Properties.end(); ++it )
		delete *it;
	ms_Properties.clear();
}

CRTTI* CPropertySystem::GetClassByName(const char* _className)
{
	for (const auto& ms_Classe : ms_Classes)
	{
		if ( strcmp(ms_Classe->GetClassName(), _className ) == 0 )
			return ms_Classe;
	}
	return nullptr;
}


void CPropertySystem::PrintClasses()
{
	for ( list<CRTTI*>::iterator it = ms_Classes.begin(); it != ms_Classes.end(); ++it )
	{
		printf( "Class: %s\n", (*it)->GetClassName() );
	}
}
