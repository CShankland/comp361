/*
 *  GraphicsEngine.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/18/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __GRAPHICS_ENGINE_H__
#define __GRAPHICS_ENGINE_H__

#include "GraphicsCommon.h"
#include "Utils.h"
#include "Actor.h"
#include "EventManager.h"
#include "ActorManager.h"
#include "WorldManager.h"
#include "WillsGlobals.h"
#include "UIElements.h"
#include "GameEngine.h"

#include <ctime>

namespace IslandWars
{
	// Specifies what the next selection should be interpreted as
	enum SelectionMode {
		SM_NONE, SM_MOVE, SM_ATTACK, SM_SPECIAL
	};
	
	enum CameraMoveState {
		CMS_P_L, CMS_P_R, CMS_P_U, CMS_P_D, CMS_NONE
	};
	
	class CameraData {
	public:
		
		explicit CameraData() : 
			r( 0.0f ), theta( 0 ), phi( 0 ), lookAt( 0.0f, 0.0f, 0.0f, 0.0f ),
			panLR( 0.0f, 0.0f, 0.0f, 0.0f ), panUD( 0.0f, 0.0f, 0.0f, 0.0f ), moveState( CMS_NONE ) {};
		
		float r;
		int theta;
		int phi;
		Vector4 lookAt;
		Vector4 panLR;
		Vector4 panUD;
		
		CameraMoveState moveState;
	};
	
	class MouseData {
	public:
		explicit MouseData() : state( -1 ), x( 0 ), y( 0 ), lastPress( 0 ) { };
		
		int state;
		int x;
		int y;
		clock_t lastPress;
	};
	
	class BaseGraphicsEngine : public IEventListener
	{
	public:
		
		virtual ~BaseGraphicsEngine();
		
		// IGraphicsEngine implementation
		ActorPtr getActor( int actorId );
		void addActor(  ObjectParams * a_Params  );
		void removeActor( int actorId );
		bool loadGame( std::string gameName, GameMap & a_Map, int a_PlayerId );
		void moveActor( int actorId, Vector2 & position );
		void startGame( bool doLoad, std::list<IEventDataPtr> events );
		
		// IEventListener implementation
		char const * GetName( void );
		bool HandleEvent( IEventData & a_Event );
		
		// Functions to deal with the top level GLUT context
		void initializeAppWindow( std::string name );
		void reshapeAppWindow( int width, int height );
		void drawAppWindow( void );
		void idleAppWindow( void );
		void mainMouse( int a_Button, int a_State, int a_X, int a_Y );
		void mainMouseMotion( int a_X, int a_Y );
		void mainKeyboard( unsigned char a_Key, int a_X, int a_Y );

		// Functions for the game window context
		void initializeGameWindow( void );
		void drawGameWindow( void );
		void reshapeGameWindow( int a_Width, int a_Height );
		void gamePassiveMouseMotion( int a_X, int a_Y );
		void gameMouse( int a_Button, int a_State, int a_X, int a_Y );
		void gameMouseMotion( int a_X, int a_Y );
		void gameKeyboard( unsigned char a_Key, int a_X, int a_Y );

		// Functions for the map window context
		void initializeMapWindow( void );
		void drawMapWindow( void );
		void reshapeMapWindow( int a_Width, int a_Height );
		
		// Functions for the stats window context
		void initializeStatsWindow( void );
		void drawStatsWindow( void );
		void reshapeStatsWindow( int a_Width, int a_Height );
		void statsMouse( int a_Button, int a_State, int a_X, int a_Y );
		void statsKeyboard( unsigned char a_Key, int a_X, int a_Y );

		explicit BaseGraphicsEngine(GameEngine * a_GameEngine);
		static BaseGraphicsEngine * getGraphicsEngine();
		
		void serializeGameTiles( std::stringstream & ss );
		void deserializeGameTiles( std::stringstream & ss );
	private:
		
		GameEngine * m_GameEngine;
		
		// --------------------------------------------------
		// Rendering variables and typedefs
		// --------------------------------------------------

		ActorManager * m_ActorManager;
		WorldManager * m_WorldManager;
		
		// --------------------------------------------------
		// GLUT state and control
		// --------------------------------------------------
		
		// Window handles
		int m_AppHandle;
		int m_WindowWidth, m_WindowHeight;
		MouseData * m_Mouse;
		
		int m_GameHandle;
		int m_GameWidth, m_GameHeight;
		MouseData * m_GameMouse;
		Vector3 eyeVec, lookVec;
		
		int m_MapHandle;
		
		int m_StatsHandle;
		int m_StatsWidth, m_StatsHeight;
		char * m_FramerateBuffer;
		MouseData * m_StatsMouse;
		ButtonList m_Buttons;
		Button * m_Selection;
		
		// Framerate and animations
		clock_t m_LastTick, m_LastTime;
		int m_LastFrameTicks, m_FrameTicks;
		int m_DeltaMilliseconds;
		
		// Picking
		const static int PICK_BUFFER_SIZE = 512;
		const static int CLICK_THRESHOLD = 200 * CLOCK_TO_MILLIS;
		GLuint * m_PickBuffer;
		int doPick( int a_ViewId, int a_X, int a_Y );
		Button * doButtonPick( int a_X, int a_Y );
		
		// --------------------------------------------------
		// Gameplay variables
		// --------------------------------------------------
		int m_PlayerId;
		bool m_OpenMode;
		
		// --------------------------------------------------
		// Saving and Loading
		// --------------------------------------------------
		std::string m_SaveName;
		std::string m_FileName;
		bool m_GetSaveFile;
		std::list<Visibility> m_VisibilityStatus;
		bool m_DoLoad;
		std::list<IEventDataPtr> m_Events;
		
		// --------------------------------------------------
		// Selection Related Functions and Data
		// --------------------------------------------------
		void setSelectedActor( ActorPtr a_Actor );
		void setSelectedActors( ActorList a_List );
		
		void buildStructureBuildMenu( ActorPtr a_Actor );
		void buildStructureMainMenu( ActorPtr a_Actor );
		
		void buildStructureStats( ActorPtr a_Actor );
		void buildUnitStats( ActorPtr a_Actor );
		
		DrawablePtr m_DrawInfo;
		ActorPtr m_pSelectedActor;
		SelectionMode m_SelectionMode;
		
		int m_StructureMenuId;
		
		int m_USelectedIndex;
		int m_SSelectedIndex;
		
		const static int BUTTON_END_TURN = -1000;
		const static int BUTTON_NEXT_UNIT = -1001;
		const static int BUTTON_NEXT_STRUCTURE = -1002;
		const static int BUTTON_STRUCTURE_BUILD_MENU = -1003;
		Button * nextUButton, * nextSButton, * endTurnButton;
		const char * nextUText, * nextSText, * endTurnText;
		
		// --------------------------------------------------
		// Menu Data
		// --------------------------------------------------
		const static int MENU_NONE = 0;
		const static int MENU_FILE = 1;
		const static int MENU_ACTION = 2;
		const static int MENU_HELP = 3;
		
		bool m_OverlayStatus;
		
		void drawMenu( void );
		
		int m_SubmenuWidth;
		int m_MenuStatus;
		
		char * m_FileMenu;
		char * m_ActionMenu;
		char * m_HelpMenu;
		
		Color * m_FileColor;
		Color * m_ActionColor;
		Color * m_HelpColor;
		
		int m_FileHandle, m_ActionHandle, m_HelpHandle;
		int m_FileStart, m_FileLength;
		int m_ActionStart, m_ActionLength;
		int m_HelpStart, m_HelpLength;
		
		CameraData * m_Camera;
	};
};  // Namespace IslandWars

#endif