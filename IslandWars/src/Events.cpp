/*
 *  Events.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/17/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

//---------------------------------------------------------------
//Note on serialization
//Events are serialized in the same order as their arguments
//with the Event Type at the beginning
//---------------------------------------------------------------

#include "GraphicsCommon.h"
#include "Events.h"

#include <sstream>

namespace IslandWars {
	/*
		LEGEND:
		For GameEngine handling:
		GEH_A : Dealt with and tested (completed)
		GEH_B : Dealt with but needs to be tested
		GEH_C : Placed but not completed nor tested
		GEH_D : Not Dealt with (not even placed)
		For GameEngine creating:
		GEC_A : Dealt with and tested (completed)
		GEC_B :	Dealt with but needs to be tested
		GEC_C : Placed but not completed nor tested
		GEC_D : Not Dealt with (not even placed)

		When all events are grade A from all perspectives, our game is done.
		Note that some events won't be handled or created by the GameEngine in any case (mostly all deniedEvent). e.g.moveDeniedEvent won't be handled by the GameEngine
	*/
	const EventType AcquiredStructureEvent::sk_EventType( "acquired_structure" ); //  GEC_D
	const EventType AttackEvent::sk_EventType( "attack" );					//GEH_B / GEC_B
	const EventType AttackDeniedEvent::sk_EventType( "attack_denied" );		//        GEC_B
	const EventType BombRequestEvent::sk_EventType( "bomb_request" );	    //GEH_B
	const EventType BombEvent::sk_EventType( "bomb" );				        //GEH_B / GEC_B
	const EventType BombDeniedEvent::sk_EventType( "bomb_denied" );	        //GEH_B / GEC_B
	const EventType BuildRequestEvent::sk_EventType( "build_request" );		//GEH_B
	const EventType BuildCancelledEvent::sk_EventType( "build_cancelled" );	//GEH_B
	const EventType BuildEvent::sk_EventType( "build" );					//GEH_B / GEH_C
	const EventType BuildDeniedEvent::sk_EventType( "build_denied" );		//GEH_B
	const EventType CreateObjectEvent::sk_EventType( "create_unit" );		        //GEH_B
	const EventType FortifyRequestEvent::sk_EventType( "fortify_request" );	//GEH_B
	const EventType FortifyEvent::sk_EventType( "fortify" );				//GEH_B / GEC_B
	const EventType FortifyDeniedEvent::sk_EventType( "fortify_denied" );	//GEH_B / GEC_B
	const EventType EndTurnEvent::sk_EventType( "end_turn" );				//GEH_B / GEH_C
	const EventType BeginTurnEvent::sk_EventType( "begin_turn" );			//        GEH_C
	const EventType MoveRequestEvent::sk_EventType( "move_request" );		//GEH_B / GEH_C
	const EventType MoveUnitEvent::sk_EventType( "move_unit" );				//GEH_B / GEH_C
	const EventType MoveDeniedEvent::sk_EventType( "move_denied" );			//GEH_B
	const EventType ConquerEvent::sk_EventType( "conquer" );				//        GEH_C
	const EventType UnitDestroyedEvent::sk_EventType( "unit_destroyed" );	//        GEC_B
	const EventType ShellRequestEvent::sk_EventType( "shell_request" );		//GEH_B
	const EventType ShellEvent::sk_EventType( "shell" );					//GEH_B / GEC_B
	const EventType ShellDeniedEvent::sk_EventType( "shell_denied" );		//        GEC_B
	const EventType RepairRequestEvent::sk_EventType( "repair_request" );	//GEH_B
	const EventType RepairEvent::sk_EventType( "repair" );					//GEH_B / GEC_B
	const EventType RepairDeniedEvent::sk_EventType( "repair_denied" );		//
	const EventType EndGameEvent::sk_EventType( "end_game" );				//        GEH_C
	const EventType GameOverEvent::sk_EventType( "game_over" );				//        GEH_C
	const EventType SaveGameEvent::sk_EventType( "save_game" );				//        GEH_C

	IActorParams & IActorParams::CreateFromStream(std::istringstream & in)
	{
		int actorType;
		in >> actorType;

		IActorParams * actor = NULL;
		if (! actor -> VInit(in))
		{
			if (actor)
				free(actor);
		}

		return *actor;
	}

	// --------------------------------------------------------
	// Acquired Structure Event
	// --------------------------------------------------------
	
	std::string AcquiredStructureEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
        str << sk_EventType.getStr() << " ";
		str << m_StructureId << " ";
		str << m_NewPlayerOwnerId << " ";
		return str.str();
	}
	
	// --------------------------------------------------------
	// Attack Event
	// --------------------------------------------------------

	AttackEvent::AttackEvent( std::stringstream & data ) {
		data >> m_AttackerId;
		m_PositionAttacker = Vector2( data );
		data >> m_DefenderId;
		m_PositionDefender = Vector2( data );
	}

	std::string AttackEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
        str << sk_EventType.getStr() << " ";
		str << m_AttackerId << " ";
		m_PositionAttacker.serialize( str );
		str << m_DefenderId << " ";
		m_PositionDefender.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Attack Denied Event
	// --------------------------------------------------------

	std::string AttackDeniedEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_AttackerId << " ";
		m_Position.serialize( str );
		return str.str();
	}


	// --------------------------------------------------------
	// Bomb Request Event
	// --------------------------------------------------------

	std::string BombRequestEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_PositionUnit.serialize( str );
		str << m_StructureId << " ";
		m_PositionStructure.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Bomb Event
	// --------------------------------------------------------

	BombEvent::BombEvent( std::stringstream & data ) {
		data >> m_UnitId;
		m_PositionUnit = Vector2( data );
		data >> m_StructureId;
		m_PositionStructure = Vector2( data );
	}

	std::string BombEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_PositionUnit.serialize( str );
		str << m_StructureId << " ";
		m_PositionStructure.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Bomb Denied Event
	// --------------------------------------------------------

	std::string BombDeniedEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_PositionUnit.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Build Request Event
	// --------------------------------------------------------
	
	std::string BuildRequestEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_StructureId << " ";
		m_Position.serialize( str );
		str << m_Type << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Build Cancelled Event
	// --------------------------------------------------------

	BuildCancelledEvent::BuildCancelledEvent( std::stringstream & data ) {
		data >> m_StructureId;
		m_Position = Vector2( data );
		data >> m_UnitId;
	}

	std::string BuildCancelledEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_StructureId << " ";
		m_Position.serialize( str );
		str << m_UnitId << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Build Event
	// --------------------------------------------------------

	BuildEvent::BuildEvent( std::stringstream & data ) {
		data >> m_StructureId;
		m_Position = Vector2( data );
		data >> m_UnitId;
		int type;
		data >> type;
		m_Type = (ObjectType) type;
	}

	std::string BuildEvent::VSerialize() {
		
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " " << m_StructureId << " ";
		m_Position.serialize( str );
		str << m_UnitId << " ";
		str << m_Type << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Build Denied Event
	// --------------------------------------------------------
	
	std::string BuildDeniedEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_StructureId << " ";
		str << m_SpecialCase << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Create Event
	// --------------------------------------------------------

	std::string CreateObjectEvent::VSerialize() {
		std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Fortify Request Event
	// --------------------------------------------------------

	std::string FortifyRequestEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		str << m_Fortify << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Fortify Event
	// --------------------------------------------------------

	FortifyEvent::FortifyEvent( std::stringstream & data ) {
		data >> m_UnitId;
		m_Position = Vector2( data );
		data >> m_Fortify;
	}

	std::string FortifyEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		str << m_Fortify << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Fortify Denied Event
	// --------------------------------------------------------

	std::string FortifyDeniedEvent::VSerialize() {
	    std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		str << m_Fortify << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// End Turn Event
	// --------------------------------------------------------

	EndTurnEvent::EndTurnEvent( std::stringstream & data ) {
		data >> m_PlayerId;
	}
	
	std::string EndTurnEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_PlayerId << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Begin Turn Event
	// --------------------------------------------------------

	BeginTurnEvent::BeginTurnEvent( std::stringstream & data ) {
		data >> m_PlayerId;
	}
	
	std::string BeginTurnEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_PlayerId << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Move Request Event
	// --------------------------------------------------------

	std::string MoveRequestEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		m_Destination.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Move Unit Event
	// --------------------------------------------------------

	MoveUnitEvent::MoveUnitEvent( std::stringstream & data ) {
		data >> m_PlayerId;
		data >> m_UnitId;
		m_Position = Vector2( data );
		m_Destination = Vector2( data );
		data >> m_MergeCase;
	}

	std::string MoveUnitEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_PlayerId << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		m_Destination.serialize( str );
		str << m_MergeCase << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Move Denied Event
	// --------------------------------------------------------

	std::string MoveDeniedEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
	    str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Conquer Event
	// --------------------------------------------------------

	ConquerEvent::ConquerEvent( std::stringstream & data ) {
		data >> m_UnitId;
		m_PositionUnit = Vector2( data );
		data >> m_StructureId;
		m_PositionStructure = Vector2( data );
	}

	std::string ConquerEvent::VSerialize() {
		std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_PositionUnit.serialize( str );
		str << m_StructureId << " ";
		m_PositionStructure.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Unit Destroyed Event
	// --------------------------------------------------------

	UnitDestroyedEvent::UnitDestroyedEvent( std::stringstream & data ) {
		data >> m_UnitId;
	}

	std::string UnitDestroyedEvent::VSerialize() {
		std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Shell Request Event
	// --------------------------------------------------------

	std::string ShellRequestEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_ShellerId << " ";
		m_PositionSheller.serialize( str );
		str << m_ShelleyId << " ";
		m_PositionShelley.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Shell Event
	// --------------------------------------------------------

	ShellEvent::ShellEvent( std::stringstream & data ) {
		data >> m_ShellerId;
		m_PositionSheller = Vector2( data );
		data >> m_ShelleyId;
		m_PositionShelley = Vector2( data );
	}

	std::string ShellEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_ShellerId << " ";
		m_PositionSheller.serialize( str );
		str << m_ShelleyId << " ";
		m_PositionShelley.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Shell Denied Event
	// --------------------------------------------------------

	std::string ShellDeniedEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_ShellerId << " ";
		m_PositionShelley.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Repair Request Event
	// --------------------------------------------------------

	std::string RepairRequestEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Repair Event
	// --------------------------------------------------------

	RepairEvent::RepairEvent( std::stringstream & data ) {
		data >> m_UnitId;
		m_Position = Vector2( data );
	}

	std::string RepairEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		return str.str();
	}

	// --------------------------------------------------------
	// Repair Denied Event
	// --------------------------------------------------------

	std::string RepairDeniedEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_UnitId << " ";
		m_Position.serialize( str );
		str << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// End Game Event
	// --------------------------------------------------------

	EndGameEvent::EndGameEvent( std::stringstream & data ) {
		data >> m_PlayerId;
	}

	std::string EndGameEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_PlayerId << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Game Over Event
	// --------------------------------------------------------

	GameOverEvent::GameOverEvent( std::stringstream & data ) {
		data >> m_PlayerId;
	}

	std::string GameOverEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_PlayerId << " ";
		return str.str();
	}

	// --------------------------------------------------------
	// Save Game Event
	// --------------------------------------------------------

	SaveGameEvent::SaveGameEvent( std::stringstream & data ) {
		data >> m_PlayerId;
		data >> m_Filename;
	}
	
	std::string SaveGameEvent::VSerialize() {
        std::stringstream str (std::stringstream::in | std::stringstream::out);
		str << sk_EventType.getStr() << " ";
		str << m_PlayerId << " ";
		str << m_Filename.c_str() << " ";
		return str.str();
	}

}
