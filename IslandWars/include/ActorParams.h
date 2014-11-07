/*
 *  ActorParams.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/1/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __ACTOR_PARAMS_H__
#define __ACTOR_PARAMS_H__

#include "GraphicsCommon.h"
#include "Utils.h"
#include "Object.h"

namespace IslandWars
{
	
	typedef unsigned int ActorId;
		
	class IActorParams
	{
	public:		
		virtual ~IActorParams() { }
		
		virtual bool VInit( std::istringstream & in ) = 0;
		virtual bool VSerialize( std::ostringstream & out ) const = 0;
		
		virtual int VGetPlayerId() = 0;
		virtual ActorId VGetActorId() = 0;
		virtual Vector2 & VGetMapPosition() = 0;
		virtual ObjectType VGetActorType() = 0;
		
		static IActorParams & CreateFromStream( std::istringstream & in );
	};
	
	class BaseActorParams : public IActorParams
	{
	public:
		
		BaseActorParams(IActorParams & params);
		BaseActorParams(int playerId, ActorId id, ObjectType type, Vector2 & mapPosition);
		
		int VGetPlayerId();
		ActorId VGetActorId();
		ObjectType VGetActorType();
		Vector2 & VGetMapPosition();
		
		bool VInit( std::istringstream & in );
		bool VSerialize( std::ostringstream & out ) const;
		
	protected:
		int m_PlayerId;
		ActorId m_Id;
		ObjectType m_Type;
		Vector2 m_MapPosition;
	};
}

#endif