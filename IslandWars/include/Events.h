/*
 *  Events.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/17/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "EventManager.h"
#include "Actor.h"
#include "Object.h"

#include <sstream>

namespace IslandWars
{
//	class AttackRequestEvent : public BaseEventData
//	{
//	public:
//		static const EventType sk_EventType;
//		const EventType & VGetEventType( void ) const { return sk_EventType; }
//
//		explicit AttackRequestEvent( std::string data );
//		explicit AttackRequestEvent( int attackerId, Vector2 & positionAttacker, Vector2 & positionDefender ) :
//			m_AttackerId( attackerId ), m_PositionAttacker( positionDefender ), m_PositionDefender( positionDefender ) {};
//
//		std::string VSerialize();
//
//		int m_AttackerId;
//		Vector2 m_PositionAttacker;
//		Vector2 m_PositionDefender;
//	};

	class AcquiredStructureEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }
		
		explicit AcquiredStructureEvent( int structureId, int newPlayerOwnerId) :
		m_StructureId( structureId ), m_NewPlayerOwnerId( newPlayerOwnerId ) {};
		
		std::string VSerialize();
		
		int m_StructureId;
		int m_NewPlayerOwnerId;
	};
	
	class AttackEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit AttackEvent( std::stringstream & data );
		explicit AttackEvent( int attackerId, Vector2 & positionAttacker, int defenderId, Vector2 & positionDefender ) :
			m_AttackerId( attackerId ), m_PositionAttacker( positionAttacker ), m_DefenderId( defenderId ), m_PositionDefender( positionDefender ) {};

		std::string VSerialize();

		int m_AttackerId;
		Vector2 m_PositionAttacker;
		int m_DefenderId;
		Vector2 m_PositionDefender;
	};

	class AttackDeniedEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit AttackDeniedEvent( int attackerId, Vector2 & position ) : // Changed back, because the reference means that the object has to exist
			m_AttackerId( attackerId ), m_Position( position ) {};         // but is otherwise the same as the object itself.

		std::string VSerialize();

		int m_AttackerId;
		Vector2 m_Position;
	};

	class BombRequestEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit BombRequestEvent( int unitId, Vector2 & positionUnit, int structureId, Vector2 & positionStructure ) :
			m_UnitId( unitId ), m_PositionUnit( positionUnit ), m_StructureId( structureId ), m_PositionStructure( positionStructure ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_PositionUnit;
		int m_StructureId;
		Vector2 m_PositionStructure;

	};

	class BombEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit BombEvent( std::stringstream & data );
		explicit BombEvent( int unitId, Vector2 & positionUnit, int structureId, Vector2 & positionStructure ) :
			m_UnitId( unitId ), m_PositionUnit( positionUnit ), m_StructureId( structureId ), m_PositionStructure( positionStructure ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_PositionUnit;
		int m_StructureId;
		Vector2 m_PositionStructure;
	};

	class BombDeniedEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit BombDeniedEvent( int unitId, Vector2 & positionUnit) :
			m_UnitId( unitId ), m_PositionUnit( positionUnit ){};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_PositionUnit;

	};


	class BuildRequestEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit BuildRequestEvent( int structureId, Vector2 & position, ObjectType type ) :
			m_StructureId( structureId ), m_Position( position ), m_Type( type ) {};

		std::string VSerialize();

		int m_StructureId;
		Vector2 m_Position;
		ObjectType m_Type;
	};

	class BuildCancelledEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit BuildCancelledEvent( std::stringstream & data );
		explicit BuildCancelledEvent( int structureId, Vector2 &  position, int unitId ) :
			m_StructureId( structureId ), m_Position( position ), m_UnitId( unitId ) {};

		std::string VSerialize();

		int m_StructureId;
		Vector2 m_Position;
		int m_UnitId;

	};

	class BuildEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit BuildEvent( std::stringstream & data );
		explicit BuildEvent( int structureId, Vector2 & position, int unitId, ObjectType type ) :
			m_StructureId( structureId ), m_Position( position ), m_UnitId( unitId ), m_Type( type ) {};

		std::string VSerialize();

		int m_StructureId;
		Vector2 m_Position;
		int m_UnitId;
		ObjectType m_Type;
	};

	class BuildDeniedEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit BuildDeniedEvent( int structureId, int specialCase ) :
			m_StructureId( structureId ), m_SpecialCase( specialCase ) {};

		std::string VSerialize();

		int m_StructureId;
		int m_SpecialCase;
	};

	class CreateObjectEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit CreateObjectEvent( ObjectParams * a_Params ) : m_Params( a_Params ) {};

		std::string VSerialize();

		ObjectParams * m_Params;
	};

	class FortifyRequestEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit FortifyRequestEvent( std::string data );
		explicit FortifyRequestEvent( int unitId, Vector2 & position, bool fortify ) :
			m_UnitId( unitId ), m_Position( position ), m_Fortify( fortify ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_Position;
		bool m_Fortify;
	};

	class FortifyEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit FortifyEvent( std::stringstream & data );
		explicit FortifyEvent( int unitId, Vector2 & position, bool fortify ) :
			m_UnitId( unitId ), m_Position( position ), m_Fortify( fortify ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_Position;
		bool m_Fortify;
	};

	class FortifyDeniedEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit FortifyDeniedEvent( int unitId, Vector2 & position, bool fortify ) :
			m_UnitId( unitId ), m_Position( position ), m_Fortify( fortify ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_Position;
		bool m_Fortify;
	};

	class EndTurnEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit EndTurnEvent( std::stringstream & data );
		explicit EndTurnEvent(int playerId): m_PlayerId( playerId ) {};

		int m_PlayerId;

		std::string VSerialize();
	};

	class BeginTurnEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit BeginTurnEvent( std::stringstream & data );
		explicit BeginTurnEvent( int playerId ): m_PlayerId( playerId ) {};

		std::string VSerialize();

		int m_PlayerId;
	};

	class MoveRequestEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit MoveRequestEvent( int unitId, Vector2 & positionUnit, Vector2 & destination ) :
			m_UnitId( unitId ), m_Position( positionUnit ), m_Destination ( destination ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_Position;
		Vector2 m_Destination;
	};

	class MoveUnitEvent : public BaseEventData
	{
	public:

		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit MoveUnitEvent( std::stringstream & data );
		explicit MoveUnitEvent( int playerId, int unitId, Vector2 & position, Vector2 & destination, int mergeCase ) :
			m_PlayerId( playerId ), m_UnitId( unitId ), m_Position( position ), m_Destination( destination ), m_MergeCase( mergeCase ) {};

		std::string VSerialize();

		int m_PlayerId;
		int m_UnitId;
		Vector2 m_Position;
		Vector2 m_Destination;
		int m_MergeCase;
	};

	class MoveDeniedEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit MoveDeniedEvent( int unitId, Vector2 & position ) :
			m_UnitId( unitId ), m_Position( position ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_Position;
	};

	class ConquerEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit ConquerEvent( std::stringstream & data );
		explicit ConquerEvent( int unitId, Vector2 & positionUnit, int structureId, Vector2 & positionStructure ) :
			m_UnitId( unitId ), m_PositionUnit( positionUnit ), m_StructureId( structureId ), m_PositionStructure( positionStructure ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_PositionUnit;
		int m_StructureId;
		Vector2 m_PositionStructure;
	};

	class UnitDestroyedEvent : public BaseEventData
	{
	public:
		
		//New Implementation
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }
		
		explicit UnitDestroyedEvent( std::stringstream & data );
		explicit UnitDestroyedEvent( int unitId ) :
		m_UnitId( unitId ) {};
		
		std::string VSerialize();
		
		int m_UnitId;
	};

	class ShellRequestEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit ShellRequestEvent( int shellerId, Vector2 & positionOfSheller, int shelleyId, Vector2 & positionOfShelley ) :
			m_ShellerId( shellerId ), m_PositionSheller( positionOfSheller ), m_ShelleyId( shelleyId ), m_PositionShelley( positionOfShelley ) {};

		std::string VSerialize();

		int m_ShellerId;
		Vector2 m_PositionSheller;
		int m_ShelleyId;
		Vector2 m_PositionShelley;
	};

	class ShellEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit ShellEvent( std::stringstream & data );
		explicit ShellEvent( int shellerId, Vector2 & positionOfSheller, int shelleyId, Vector2 & positionOfShelley ) :
			m_ShellerId( shellerId ), m_PositionSheller( positionOfSheller ), m_ShelleyId( shelleyId ), m_PositionShelley( positionOfShelley ) {};

		std::string VSerialize();

		int m_ShellerId;
		Vector2 m_PositionSheller;
		int m_ShelleyId;
		Vector2 m_PositionShelley;
	};

	class ShellDeniedEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit ShellDeniedEvent( int shellerId, Vector2 & positionOfShelley ) :
			m_ShellerId( shellerId ), m_PositionShelley( positionOfShelley ) {};

		std::string VSerialize();

		int m_ShellerId;
		Vector2 m_PositionShelley;
	};

	class RepairRequestEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit RepairRequestEvent( int unitId, Vector2 & position ) :
			m_UnitId( unitId ), m_Position( position ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_Position;
	};

	class RepairEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit RepairEvent( std::stringstream & data );
		explicit RepairEvent( int unitId, Vector2 & position ) :
			m_UnitId( unitId ), m_Position( position ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_Position;
	};

	class RepairDeniedEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit RepairDeniedEvent( int unitId, Vector2 & position ) :
			m_UnitId( unitId ), m_Position( position ) {};

		std::string VSerialize();

		int m_UnitId;
		Vector2 m_Position;
	};

	class EndGameEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit EndGameEvent( std::stringstream & data );
		explicit EndGameEvent( int playerId ) :
			m_PlayerId( playerId ) {};

		std::string VSerialize();

		int m_PlayerId;
	};

	class GameOverEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit GameOverEvent( std::stringstream & data );
		explicit GameOverEvent( int playerId ) : m_PlayerId( playerId ) {};

		std::string VSerialize();

		int m_PlayerId;
	};

	class SaveGameEvent : public BaseEventData
	{
	public:
		static const EventType sk_EventType;
		const EventType & VGetEventType( void ) const { return sk_EventType; }

		explicit SaveGameEvent( std::stringstream & data );
		explicit SaveGameEvent( int playerId, std::string filename) : m_PlayerId( playerId ), m_Filename( filename ) {};

		std::string VSerialize();

		std::string m_Filename;
		int m_PlayerId;
	};
}

#endif
