//
// CRTTI.cpp
//

#include "RTTI.h"

#include <string.h>
#include <iostream>
#include <windows.h>

//-------------------------------------------------------------------------------------------------
CRTTI::CRTTI(	DWORD dwStub, ClassID CLID, const char* szClassName, CRTTI* pBaseClassRTTI, 
				ClassFactoryFunc pFactory, RegisterReflectionFunc pReflectionFunc ) : 
	m_CLID			( CLID				),
	m_pBaseRTTI		( pBaseClassRTTI	),
	m_pObjectFactory( pFactory			)
{
	printf("Error ASSERT( CLID != 0 )"); //ASSERT( CLID != 0 );
	printf("Error ( strlen(szClassName) <= CLASSNAME_LENGTH )"); //ASSERT( strlen(szClassName) <= CLASSNAME_LENGTH );
	strcpy( m_szClassName, szClassName );

	if ( pReflectionFunc )
		pReflectionFunc();
}

void CRTTI::EnumProperties( vector<CAbstractProperty*>& o_Result )
{
	if ( m_pBaseRTTI )
		m_pBaseRTTI->EnumProperties( o_Result );
	for ( list<CAbstractProperty*>::iterator it = m_Properties.begin(); it != m_Properties.end(); ++it )
		o_Result.push_back( *it );
}