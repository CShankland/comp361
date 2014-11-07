/*
 *  NetworkManager.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/3/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "NetworkManager.h"
#include "WillsGlobals.h"
#include "GameEngine.h"

extern int peer;
namespace IslandWars
{
	std::string msg0 = "Your my bitch: We're going to Vegas!";
	std::string msg1 = "Im your bitch: (input racist remark here)";
	std::string msg2 = "Change turns";
	std::string msg3 = "End";
	
	GameEngine *gePtr;
	
	static NetworkManager * me;
	
	NetworkManager * NetworkManager::get() {
		return me;
	}
	
	NetworkManager::NetworkManager()
	{
		me = this;
		// Any constructor information here
	}
	
	NetworkManager::~NetworkManager()
	{
        shutdown(peer, 2);
        close(peer);
	}
	
	bool NetworkManager::Initialize()
	{
		EventListenerPtr networkListenerPtr( this );
		
		safeAddListener( networkListenerPtr, AttackEvent::sk_EventType);
		safeAddListener( networkListenerPtr, BombEvent::sk_EventType);
		safeAddListener( networkListenerPtr, BuildCancelledEvent::sk_EventType);
		safeAddListener( networkListenerPtr, BuildEvent::sk_EventType);
		safeAddListener( networkListenerPtr, FortifyEvent::sk_EventType);
		safeAddListener( networkListenerPtr, EndTurnEvent::sk_EventType);
		safeAddListener( networkListenerPtr, BeginTurnEvent::sk_EventType);
		safeAddListener( networkListenerPtr, MoveUnitEvent::sk_EventType);
		safeAddListener( networkListenerPtr, ConquerEvent::sk_EventType);
		safeAddListener( networkListenerPtr, UnitDestroyedEvent::sk_EventType);
		safeAddListener( networkListenerPtr, ShellEvent::sk_EventType);
		safeAddListener( networkListenerPtr, RepairEvent::sk_EventType);
		safeAddListener( networkListenerPtr, EndGameEvent::sk_EventType);
		safeAddListener( networkListenerPtr, GameOverEvent::sk_EventType);
		safeAddListener( networkListenerPtr, SaveGameEvent::sk_EventType);
		
		gePtr = GameEngine::get();
		return true;
	}
	
	bool NetworkManager::HandleEvent( IEventData & event )
	{
			if (event.net_send && !loadMode) {
				if (!userServer) {
					//std::cout << "Trying to send as client: "<< event.VSerialize();
					send(peer, event.VSerialize().c_str(), 50, 0);
					//receivedCheck(peer);
				} else {
					//std::cout << "Trying to send as the server: "<< event.VSerialize();
					sendPeerServer(event.VSerialize());
					if (event.VGetEventType() == EndTurnEvent::sk_EventType) {
						nextPeer();
					}
				}
			}
			
            return false;
	}
	
	char const * NetworkManager::GetName()
	{
		return "Network";
	}
	
	bool NetworkManager::Poll()
	{
		if (!userServer) {
			std::string reception = checkPeer(peer);
			//std::cout << reception << ", ";
			
			if (reception != "NIL") {
				deserializeEvent(reception);
			}		
			
			//usleep(50);
			
			std::string dummyMessage("NIL");
			sendPeer(peer,dummyMessage);
		} else {
			sendPeerServer(std::string("NIL"));
			int currentPeer = nextPeer();
			
			while (currentPeer != -1) {
				std::string reception = checkPeer(currentPeer);
				
				if (reception != "NIL") {
					deserializeEvent(reception);
					
					//usleep(50);
					
					sendPeerServer(reception);
					//deserializeEvent(reception);
				} 
				
				currentPeer = nextPeer();
			}
		}
		return true;
	}
	
	void NetworkManager::deserializeEvent(std::string listener) {
		
		std::stringstream ss (listener, (std::stringstream::in | std::stringstream::out));
		
		std::string evtType;
		ss >> evtType;
		
		EventType type( evtType );
		IEventData * evt = NULL;
		
		if (type == AttackEvent::sk_EventType) {
			evt = new AttackEvent( ss );
		} else if (type == BombEvent::sk_EventType) {
			evt = new BombEvent( ss );
		} else if (type == BuildCancelledEvent::sk_EventType) {
			evt = new BuildCancelledEvent( ss );
		} else if (type == BuildEvent::sk_EventType) {
			evt = new BuildEvent( ss );
		} else if (type == FortifyEvent::sk_EventType) {
			evt = new FortifyEvent( ss );
		} else if (type == EndTurnEvent::sk_EventType) {
			evt = new EndTurnEvent( ss );
		} else if (type == BeginTurnEvent::sk_EventType) {
			evt = new BeginTurnEvent( ss );
		} else if (type == MoveUnitEvent::sk_EventType) {
			evt = new MoveUnitEvent( ss );
		} else if (type == ConquerEvent::sk_EventType) {
			evt = new ConquerEvent( ss );
		} else if (type == UnitDestroyedEvent::sk_EventType) {
			evt = new UnitDestroyedEvent( ss );
		} else if (type == ShellEvent::sk_EventType) {
			evt = new ShellEvent( ss );
		} else if (type == RepairEvent::sk_EventType) {
			evt = new RepairEvent( ss );
		} else if (type == EndGameEvent::sk_EventType) {
			evt = new EndGameEvent( ss );
		} else if (type == GameOverEvent::sk_EventType) {
			evt = new GameOverEvent( ss );
		} else if (type == SaveGameEvent::sk_EventType) {
			evt = new SaveGameEvent( ss );
		} else {
			//std::cout << "Unrecognized event" << std::endl;
		}
		
		if (evt != NULL) {
			evt->net_send = 0;
			IEventDataPtr ptr(evt);
			safeQueueEvent(ptr);
		}
	}
}
