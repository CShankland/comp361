/*
 *  WorldManager.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/28/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __WORLD_MANAGER_H__
#define __WORLD_MANAGER_H__

#include "Utils.h"
#include "Actor.h"
#include "WillsGlobals.h"

#include <map>

namespace IslandWars {
	
	typedef std::vector< GameTilePtr > TileVector;
	typedef std::vector< TileVector > TileMatrix;
	
	class WorldManager {
	public:
		WorldManager();
		~WorldManager();
		
		static WorldManager * get();
		
		void initialize( GameMap & a_Map );
		void registerView( int a_ViewId );
		void updateVisibility( ActorPtr a_Actor, Visibility a_Visibility );
		
		void draw2D( int a_ViewId, int a_DeltaMilliseconds );
		void draw3D( int a_ViewId, int a_DeltaMilliseconds );
		
		void setActorPosition( ActorPtr a_Actor, Vector2 & a_Position );
		void removeActor( ActorPtr a_Actor );
		GameTilePtr getTile( Vector2 & a_Position );
		TileMatrix getTiles() { return m_Tiles; }
		
		ActorList getContainedActors( Vector2 & a_Position );
	private:
		Vector2 m_Dimensions;
		
		TileMatrix m_Tiles;
		
		std::string getTileTexturePath( Terrain a_Terrain );
		
		Terrain lookupTerrain( int mapCode );
		
		float * m_HexCoords;
		float * m_HexTex;
	};
}; // Namespace IslandWars

#endif