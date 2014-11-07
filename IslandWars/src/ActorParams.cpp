/*
 *  ActorParams.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/1/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "ActorParams.h"

namespace IslandWars
{
	BaseActorParams::BaseActorParams( IActorParams & params) 
	: m_PlayerId( params.VGetPlayerId() ), m_Id( params.VGetActorId() ), m_Type( params.VGetActorType() ), m_MapPosition( params.VGetMapPosition() ) { }
	
	BaseActorParams::BaseActorParams( int playerId, ActorId id, ObjectType type, Vector2 & mapPosition )
	: m_PlayerId( playerId ), m_Id( id ), m_Type( type ), m_MapPosition( mapPosition ) { }
	
	int BaseActorParams::VGetPlayerId()
	{
		return m_PlayerId;
	}
	
	ActorId BaseActorParams::VGetActorId() 
	{
		return m_Id;
	}
	
	ObjectType BaseActorParams::VGetActorType()
	{
		return m_Type;
	}
	
	Vector2 & BaseActorParams::VGetMapPosition()
	{
		return m_MapPosition;
	}
	
	bool BaseActorParams::VInit( std::istringstream & in )
	{
		return false;
	}
	
	bool BaseActorParams::VSerialize( std::ostringstream & out) const
	{
		return false;
	}
}