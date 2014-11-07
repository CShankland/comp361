/*
 *  ActorManager.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/28/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __ACTOR_MANAGER_H__
#define __ACTOR_MANAGER_H__

#include "Unit.h"
#include "Actor.h"

#include <map>
#include <queue>
#include <sstream>

namespace IslandWars {
	
	class ActorManager {
	public:
		// Basic constructor / destructor
		explicit ActorManager();
		~ActorManager();
		
		static ActorManager * get();
		
		// Register a view.  This should be done before any actors have been
		// added, as there is no cross-referencing when they have been already
		// added to create views.
		void registerView( int a_ViewId );
		
		// Add an actor, implicitely the actor is defined inside all the registered
		// views, and the actor's internal table is updated.  After this call, it will
		// be valid to draw the actor in all registered views.
		void addActor( ActorPtr a_Unit );
		
		// Removes an actor from the internal tables, does not remove the geometry from
		// memory.
		void removeActor( int a_ActorId );
		
		// Same as above, for cleanliness.
		void removeActor( ActorPtr a_Actor );
		
		// Retrieve an actor by its id
		ActorPtr getActor( int a_ActorId );
		
		// Draw all the visible actors in 2D
		void draw2D( int a_ViewId, int a_DeltaMilliseconds );
		
		// Draw all the visible actors in 3D
		void draw3D( int a_ViewId, int a_DeltaMilliseconds );
		
		// Set an actor's destination, animation to get there
		// will be handled internally
		void move( int a_ActorId, Vector2 & a_Destination );
		
		// Set the status of the open mode flag
		void setOpenMode( bool a_OpenMode );

	private:
		// Object Type to DrawTable information
		typedef std::map< ObjectType, DrawData * > ViewData;
		
		// View Id to View Data information
		typedef std::map< int, ViewData > ViewTable;
		
		// Actor Id to Actor Pointer
		typedef std::map< int, ActorPtr > ActorTable;
		
		// Processing queue of destinations
		typedef std::queue< Vector2 > Itinerary;
		
		// Table of Actor Id to Itinerary
		typedef std::map< int, Itinerary > AnimationRegistry;
		
		// Animation routine
		void updatePositions( int deltaMilliseconds );
		
		// Visibility checking (primitive culling)
		void updateVisibility();
		
		// Helper function to generate a new display list
		// given an object type and a view to bind it in
		DrawData * createDrawTable( int a_ViewId, ObjectType a_Type );
		
		// Geometry delegate functions
		void buildUnitSquare();
		int build3DActor( ObjectType a_Type );
		
		// Actual building functions
		//void buildCity();
		//void buildTower();
		//void buildAirFighter();
		//void buildBomber();
		//void buildBattleship();
		//void buildTransporter();
		//void buildCarrier();
		//void buildSubmarine();
		//void buildArtillery();
		//void buildFlag();
		//void buildTank();
		
		bool m_OpenMode;
		
		// Look up functions for creating textures
		static std::string get2DTexturePath( ObjectType a_Type );
		static const char * get3DTexturePath( ObjectType a_Type );
		
		// Member variables to keep track of everything
		ViewTable m_ViewTable;
		ActorTable m_ActorTable;
		AnimationRegistry m_Animations;
	};
	
}; // Namespace IslandWars

#endif