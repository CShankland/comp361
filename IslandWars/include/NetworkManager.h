/*
 *  NetworkManager.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/3/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

// Local includes
#include "EventManager.h"
#include "Events.h"
#include "network.h"

namespace IslandWars
{
	
	class NetworkManager : public IEventListener
	{
	public:
		explicit NetworkManager();
		~NetworkManager();
		
		bool Initialize();
		
		bool Poll();
		
		//************************//
		// IEventListener Methods //
		//************************//
		char const * GetName( void );
		
		bool HandleEvent( IEventData & event );
		
		static NetworkManager * get();
		void deserializeEvent(std::string eventType);
	};
	
}

#endif