/*
 *  Actor.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/28/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "Utils.h"
#include "tga.h"
#include "GraphicsCommon.h"
#include "ActorParams.h"
#include "Hexagon.h"
#include "Structure.h"

#include <map>
#include <vector>

#define ACTOR_MOVE_SPEED 0.000005
#define EPSILON 0.05
#define MAX_BOUNCE 1.5

namespace IslandWars
{	
	
	class DrawData {
	public:
		int dl2D;
		int dl3D;
	};
	
	// Defines something that will be drawable in the game
	// This means that it can be displayed in the main game window,
	// the map window and the stats window.
	class Drawable {
	public:
		// Render a 3D representation of the object
		//
		// deltaMilliseconds - Number of milliseconds since the last update
		// viewId - The GLUT window id of the calling view
		virtual void draw3D( int a_ViewId, int a_DeltaMilliseconds ) = 0;
		
		// Render a 2D representation of the object ( in the XY plane )
		//
		// deltaMilliseconds - Number of milliseconds since the last update
		// viewId - The GLUT window id of the calling view
		virtual void draw2D( int a_ViewId, int a_DeltaMilliseconds ) = 0;
		
		// Register a view with the Drawable
		//
		// pre - The calling view is the current GLUT context
		// post - draw3D and draw2D are valid given this viewId
		virtual void attachView( int a_ViewId, DrawData * a_DrawData ) = 0;
	};
	
	typedef boost::shared_ptr<Drawable> DrawablePtr;
	
	// Visibility constants
	static const char VISIBILITY_NONE = 0;
	static const char VISIBILITY_FOG = 1;
	static const char VISIBILITY_OPEN = 2;
	
	typedef unsigned char Visibility;
	
	class Actor;
	
	typedef boost::shared_ptr<Actor> ActorPtr;
	static ActorPtr NullActorPtr;
	typedef std::list<ActorPtr> ActorList;
	
	typedef std::map< int, DrawData * > DrawTable;
	
	// A GameTile is a representation of the smallest division of the
	// game world.  In this case, it is represented as a hexagon.
	class GameTile : public Drawable {
	public:
		explicit GameTile( Vector2 a_Position, Terrain a_Type );
		virtual ~GameTile();
		
		static int getHashCode(GameTile & a_GameTile) { return (a_GameTile.m_Position.x << 16) | a_GameTile.m_Position.y; }
		static int getHashCode(GameTile * a_GameTile) { return (a_GameTile->m_Position.x << 16) | a_GameTile->m_Position.y; }
		
		// Accessors
		Terrain getType() { return m_Type; }
		Vector2 & getPosition() { return m_Position; }
		Vector3 & getDrawPos() { return m_DrawPosition; }
		Visibility getVisibility() { return m_Visibility; }
		ActorPtr getTopActor() { return (m_Actors.empty()) ? NullActorPtr : m_Actors.front(); }
		ActorPtr getActor( int a_Id );
		ActorList getActors() { return m_Actors; }
		float * getHeightMap() { return m_HeightMap; }
		Vector3 & getDrawPosition() { return m_DrawPosition; }
		
		// Mutators
		void initialize( Vector3 & a_Position, float * a_HeightMap );
		void setVisibility( Visibility a_Visibility ) { m_Visibility = a_Visibility; }
		void addActor( ActorPtr a_Actor );
		ActorPtr removeActor( int a_Id );
		
		// Drawable implementation
		void draw3D( int a_ViewId, int a_DeltaMilliseconds );
		void draw2D( int a_ViewId, int a_DeltaMilliseconds );
		void attachView( int a_ViewId, DrawData * a_DrawTable );
		
	private:
		Terrain m_Type;
		Vector2 m_Position;
		Vector3 m_DrawPosition;
		float * m_HeightMap;
		Visibility m_Visibility;
		ActorList m_Actors;
		DrawTable m_DrawTable;
	};
	
	typedef boost::shared_ptr<GameTile> GameTilePtr;
	
	// An Actor is an interactable object within the game
	class Actor : public Drawable {
	public:
		explicit Actor( int a_PlayerId, int a_ActorId, ObjectType a_ObjectType );
		virtual ~Actor();
		
		// Accessors
		int getPlayerId() { return m_PlayerId; }
		int getActorId() { return m_ActorId; }
		bool isSelected() { return m_Selected; }
		Vector2 & getMapPos() { return m_MapPos; }
		Vector3 & getDrawPos() { return m_DrawPosition; }
		ObjectType getType() { return m_Type; }
		bool isVisible() { return m_Visible; }
		
		// Mutators
		void initialize( Vector2 & a_MapPos, Vector3 & a_DrawPosition );
		void setMapPos( Vector2 & a_MapPos ) { m_MapPos = a_MapPos; }
		void setSelected( bool a_Selected ) { m_Selected = a_Selected; }
		void setVisible( bool a_Visible ) { m_Visible = a_Visible; }
		virtual void setDrawPosition( Vector3 & a_Vector ) = 0;
		
		// Drawable implementation
		void draw3D( int a_ViewId, int a_DeltaMilliseconds );
		void draw2D( int a_ViewId, int a_DeltaMilliseconds );
		void attachView( int viewId, DrawData * a_DrawTable );
		
		static Color getColor( int a_playerId );
		
	protected:
		int m_PlayerId;
		int m_ActorId;
		bool m_Selected;
		bool m_Visible;
		Vector2 m_MapPos;
		ObjectType m_Type;
		Color m_Color;
		Vector3 m_DrawPosition;
		DrawTable m_DrawTable;
		
		
	};
	
	// A Unit is a moveable actor within the game
	class UnitActor : public Actor {
	public:
		explicit UnitActor( int a_PlayerId, int a_ActorId, ObjectType a_ObjectType );
		virtual ~UnitActor();
		
		// Accessors
		bool isFortified() { return m_Fortified; }
		
		// Mutators
		void setFortified( bool a_Fortified ) { m_Fortified = a_Fortified; }
		void setDrawPosition( Vector3 & a_DrawPos ) { m_DrawPosition = a_DrawPos; }
		
		// Animation implementations
		void attack( Vector3 & a_DefenderPosition ) {};
		
		// Actor implementation
		//virtual void draw3D( int a_ViewId, int a_DeltaMilliseconds ) = 0;
		
	private:
		bool m_Fortified;
	};
	
	// A Structure is an actor capable of producing units
	class StructureActor : public Actor {
	public:
		explicit StructureActor( ProductionCapabilities a_BuildOptions, int a_PlayerId, int a_ActorId, ObjectType a_ObjectType );
		virtual ~StructureActor();
		
		void setPlayerId( int playerId ) { m_PlayerId = playerId; m_Color = getColor(playerId); }
		void setDrawPosition( Vector3 & a_Pos ) { };
		// Accessors
		ProductionCapabilities getBuildOptions() { return m_BuildOptions; }
		
		// Actor implementation
		//virtual void draw3D( int a_ViewId, int a_DeltaMilliseconds ) = 0;
		
	private:
		ProductionCapabilities m_BuildOptions;
	};
	
	typedef boost::shared_ptr<UnitActor> UnitActorPtr;
	typedef boost::shared_ptr<StructureActor> StructureActorPtr;
	
	static StructureActorPtr NullStructureActorPtr;
	static UnitActorPtr NullUnitActorPtr;
	
}; // Namespace IslandWars
#endif