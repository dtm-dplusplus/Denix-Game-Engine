//
// CPropertySystem.h
//

#ifndef _PROPERTY_SYSTEM_H
#define _PROPERTY_SYSTEM_H

#include <list>
#include "Property.h"

class CRTTI;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CPropertySystem
//
// Global manager of all properties. Used to release properties.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class CPropertySystem
{

public :

	//----------------------------------------------------------------------------------------------
	// Constructor
	CPropertySystem();
	//----------------------------------------------------------------------------------------------
	// Destructor
	virtual ~CPropertySystem();

	//----------------------------------------------------------------------------------------------
	// Returns global list of properties.
	static	list<CAbstractProperty*>*	GetProperties() { return &ms_Properties;}

	static list<CRTTI*>* GetClasses() { return &ms_Classes; }

	static CRTTI* GetClassByName( const char*_className );
	static void PrintClasses();
private :

	static	list<CAbstractProperty*>	ms_Properties;

	static list<CRTTI*> ms_Classes;
};

#endif	// _PROPERTY_SYSTEM_H