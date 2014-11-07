/*
 *  EventManager.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/10/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "EventManager.h"

#include <cstring>
#include <cstdlib>

namespace IslandWars
{

static IEventManager * g_pEventMgr = NULL;

IEventManager * IEventManager::Get()
{
	return g_pEventMgr;
}

IEventManager::IEventManager( char const * const pName, bool setAsGlobal )
{
	if (setAsGlobal )
		g_pEventMgr = this;
}

IEventManager::~IEventManager()
{
	if (g_pEventMgr == this)
		g_pEventMgr = NULL;
}

bool safeAddListener( EventListenerPtr const & inHandler, EventType const & inType )
{
	assert(IEventManager::Get());
	return IEventManager::Get() -> VAddListener( inHandler, inType );
}

bool safeDelListener( EventListenerPtr const & inHandler, EventType const & inType )
{
	assert(IEventManager::Get());
	return IEventManager::Get() -> VDelListener( inHandler, inType );
}

bool safeTriggerEvent( IEventData & inEvent )
{
	assert(IEventManager::Get());
	return IEventManager::Get() -> VTrigger( inEvent );
}

bool safeQueueEvent( IEventDataPtr & inEvent )
{
	assert(IEventManager::Get());
	return IEventManager::Get() -> VQueueEvent( inEvent );
}

bool safeAbortEvent( EventType const & inType, bool allOfType )
{
	assert(IEventManager::Get());
	return IEventManager::Get() -> VAbortEvent( inType, allOfType );
}

bool safeTickEventManager( unsigned long maxMillis )
{
	assert(IEventManager::Get());
	return IEventManager::Get() -> VTick( maxMillis );
}

bool safeValidateEventType( EventType const & inType )
{
	assert(IEventManager::Get());
	return IEventManager::Get() -> VValidateType( inType );
}
	
}
