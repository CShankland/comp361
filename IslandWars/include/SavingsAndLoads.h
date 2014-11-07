/*
 *  SavingsAndLoads.h
 *  IslandWars
 *
 *  Created by Seraphin on 10-03-24.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */
#pragma once
#ifndef _SL_
#define _SL_

#include "Events.h"
#include "EventManager.h"
#include "WillsGlobals.h"
#include <list>
#include <cstdio>

namespace IslandWars {
class SavingsAndLoads : public IEventListener {
private:
	std::list<std::string> relevantEvents;
public:
	SavingsAndLoads *mePtr;

	SavingsAndLoads( int mapVal ) {
		EventListenerPtr ptr(this);
		
		safeAddListener( ptr, AttackEvent::sk_EventType);
		safeAddListener( ptr, BombEvent::sk_EventType);
		safeAddListener( ptr, BuildCancelledEvent::sk_EventType);
		safeAddListener( ptr, BuildEvent::sk_EventType);
		safeAddListener( ptr, FortifyEvent::sk_EventType);
		safeAddListener( ptr, EndTurnEvent::sk_EventType);
		safeAddListener( ptr, BeginTurnEvent::sk_EventType);
		safeAddListener( ptr, MoveUnitEvent::sk_EventType);
		safeAddListener( ptr, ConquerEvent::sk_EventType);
		safeAddListener( ptr, UnitDestroyedEvent::sk_EventType);
		safeAddListener( ptr, ShellEvent::sk_EventType);
		safeAddListener( ptr, RepairEvent::sk_EventType);
		safeAddListener( ptr, EndGameEvent::sk_EventType);
		safeAddListener( ptr, GameOverEvent::sk_EventType);
		safeAddListener( ptr, SaveGameEvent::sk_EventType);

		if (loadMode) {
			loadGame(filename.c_str());
		} else {
			m_MapVal = mapVal;
		}
	}
	~SavingsAndLoads() {}

	void writeToFile(const char *filename);
	void loadGame(const char *filename);
	
	bool HandleEvent( IEventData & event );
	const char * GetName( void );
	
	int m_MapVal;
	std::list<IEventDataPtr> events;
};


}
#endif