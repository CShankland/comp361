/*
 *  GraphicsEngine.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/18/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsEngine.h"
#include "Events.h"
#include "GameEngine.h"
#include "NetworkManager.h"

#include <iostream>

#include <boost/pointer_cast.hpp>

namespace IslandWars
{
	static BaseGraphicsEngine * me;
	
	BaseGraphicsEngine * BaseGraphicsEngine::getGraphicsEngine() {
		return me;
	}
	
	// Major hack to make GLUT work with C++
	// Wraps the member functions into a C style function, which can be used in a callback
	void GE_Idle( void ) { BaseGraphicsEngine::getGraphicsEngine()->idleAppWindow(); }
	void GE_Display( void ) { BaseGraphicsEngine::getGraphicsEngine()->drawAppWindow(); }
	void GE_Reshape( int a_Width, int a_Height ) { BaseGraphicsEngine::getGraphicsEngine()->reshapeAppWindow( a_Width, a_Height ); }
	void GE_Mouse( int a_Button, int a_State, int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->mainMouse( a_Button, a_State, a_X, a_Y ); }
	void GE_MouseMotion( int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->mainMouseMotion( a_X, a_Y ); }
	void GE_Keyboard( unsigned char a_Key, int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->mainKeyboard( a_Key, a_X, a_Y ); }
	
	void GE_GameDisplay( void ) { BaseGraphicsEngine::getGraphicsEngine()->drawGameWindow(); }
	void GE_GameReshape( int a_Width, int a_Height ) { BaseGraphicsEngine::getGraphicsEngine()->reshapeGameWindow( a_Width, a_Height ); }
	void GE_GamePassiveMouseMotion( int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->gamePassiveMouseMotion( a_X, a_Y ); }
	void GE_GameMouse( int a_Button, int a_State, int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->gameMouse( a_Button, a_State, a_X, a_Y ); }
	void GE_GameMouseMotion( int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->gameMouseMotion( a_X, a_Y ); }
	void GE_GameKeyboard( unsigned char a_Key, int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->gameKeyboard( a_Key, a_X, a_Y ); }
	
	void GE_MapDisplay( void ) { BaseGraphicsEngine::getGraphicsEngine()->drawMapWindow(); }
	void GE_MapReshape( int a_Width, int a_Height ) { BaseGraphicsEngine::getGraphicsEngine()->reshapeMapWindow( a_Width, a_Height ); }
	
	void GE_StatsDisplay( void ) { BaseGraphicsEngine::getGraphicsEngine()->drawStatsWindow(); }
	void GE_StatsReshape( int a_Width, int a_Height ) { BaseGraphicsEngine::getGraphicsEngine()->reshapeStatsWindow( a_Width, a_Height ); }
	void GE_StatsMouse( int a_Button, int a_State, int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->statsMouse( a_Button, a_State, a_X, a_Y ); }
	void GE_StatsKeyboard( unsigned char a_Key, int a_X, int a_Y ) { BaseGraphicsEngine::getGraphicsEngine()->statsKeyboard( a_Key, a_X, a_Y ); }

	BaseGraphicsEngine::~BaseGraphicsEngine() {
		
	}
	
	// ----------------------------------------------
	// IGraphicsEngine implementation
	// ----------------------------------------------
	
	ActorPtr BaseGraphicsEngine::getActor( int actorId ) {
		return NullActorPtr;
	}
	
	void BaseGraphicsEngine::addActor( ObjectParams * a_Params ) {
		
		Vector2 mapPos = Vector2(a_Params->x_coord, a_Params->y_coord);
		ObjectType t = a_Params->type;
		
		// Handle structures differently
		if ( t == LAND_CITY || t == PORT_CITY || t == TOWER) {
			ProductionCapabilities prodCaps;
			prodCaps.push_back(AIRFIGHTER);
			prodCaps.push_back(BOMBER);
			prodCaps.push_back(ARTILLERY);
			prodCaps.push_back(FLAG);
			prodCaps.push_back(TANK);
			if (t == PORT_CITY) {
				prodCaps.push_back(BATTLESHIP);
				prodCaps.push_back(TRANSPORTER);
				prodCaps.push_back(CARRIER);
				prodCaps.push_back(SUBMARINE);
			}
			
			StructureActor * structure = new StructureActor( prodCaps, a_Params->playerId, a_Params->objectId, t);
			StructureActorPtr saPtr( structure );
			
			// Set the color and position of the structure
			
			Vector3 drawPos = m_WorldManager->getTile(mapPos)->getDrawPos();
			drawPos.y += 0.5f;
			saPtr->initialize( mapPos, drawPos);
			
			// Now register the new actor to the managers
			m_ActorManager->addActor(saPtr);
			m_WorldManager->setActorPosition( saPtr, mapPos );
		// Than we handle units
		} else {
			UnitActor * unit = new UnitActor( a_Params->playerId, a_Params->objectId, t );
			UnitActorPtr uaPtr( unit );
			
			// Set the color and position of the unit
			Vector3 drawPos = m_WorldManager->getTile(mapPos)->getDrawPos();
			drawPos.y += 0.5f;
			uaPtr->initialize( mapPos, drawPos );
			
			// Now register the new actor to the managers
			m_ActorManager->addActor(uaPtr);
			m_WorldManager->setActorPosition( uaPtr, mapPos );
			
			
			//printf("Adding Unit\n");
			
			if (m_PlayerId == uaPtr->getPlayerId()) {
				m_WorldManager->updateVisibility(uaPtr, VISIBILITY_OPEN);
				setSelectedActor(uaPtr);
			}
		}
	}
	
	void BaseGraphicsEngine::removeActor( int actorId ) {
		ActorPtr actor = m_ActorManager->getActor(actorId);
		if (actor != NullActorPtr) {
			m_WorldManager->removeActor(m_ActorManager->getActor(actorId));
			m_ActorManager->removeActor(actorId);
			m_pSelectedActor = NullActorPtr;
			m_SelectionMode = SM_NONE;
			glutSetWindow(m_StatsHandle);
			glutPostRedisplay();
		}
	}
	
	bool BaseGraphicsEngine::loadGame( std::string gameName, GameMap & a_Map, int a_PlayerId ) {
		
		m_PlayerId = a_PlayerId;
		
		// ----------------------------------------------------------------
		// Initialize variables
		// ----------------------------------------------------------------
		m_Camera = new CameraData;
		m_WorldManager = new WorldManager;
		m_ActorManager = new ActorManager;
		
		m_WorldManager->initialize(a_Map);
		
		m_Mouse = new MouseData;
		
		m_FileMenu = new char[5];
		sprintf(m_FileMenu, "File");
		
		m_ActionMenu = new char[7];
		sprintf(m_ActionMenu, "Action");
		
		m_HelpMenu = new char[5];
		sprintf(m_HelpMenu, "Help");
		
		m_FileColor   = new Color(0.8f, 0.8f, 0.8f);
		m_ActionColor = new Color(0.8f, 0.8f, 0.8f);
		m_HelpColor   = new Color(0.8f, 0.8f, 0.8f);
		
		initializeAppWindow( gameName );
	
		Vector2 mapPos(-1, -1);
		Vector3 drawPos( 0.0f, -6.0f, 0.0f );
		
		// Load the models, so it is fast when we need them
		for (int i = 0; i < 12; i++) {
			UnitActor * unit = new UnitActor( -1,  -1 * i, (ObjectType) i );
			UnitActorPtr uaPtr( unit );
			
			// Set the color and position of the unit
			
			uaPtr->initialize( mapPos, drawPos );
			
			// Now register the new actor to the managers
			m_ActorManager->addActor(uaPtr);
		}
		
		return true;
	}
	
	void BaseGraphicsEngine::moveActor( int actorId, Vector2 & a_Position ) {
		if (!m_DoLoad) {
			if (!m_OpenMode) {
				TileMatrix tiles = m_WorldManager->getTiles();
				for (TileMatrix::iterator it = tiles.begin(); it != tiles.end(); it++) {
					for (TileVector::iterator it2 = it->begin(); it2 != it->end(); it2++) {
						if ((*it2)->getVisibility() == VISIBILITY_OPEN) {
							(*it2)->setVisibility(VISIBILITY_FOG);
						}
					}
				}
			}
			ActorPtr ptr = m_ActorManager->getActor(actorId);
			if (ptr.get() == NULL)
				return;
			m_WorldManager->setActorPosition( ptr, a_Position );
			m_ActorManager->move( actorId, a_Position);
		
			if (!m_OpenMode) {
				// TODO:  Switch this back to m_PlayerId
				PlayerPtr player = m_GameEngine->getSpecificPlayer(m_PlayerId);
		
				std::list<UnitPtr> units = player->getUnitList();
				std::list<StructurePtr> structures = player->getStructureList();
				for (std::list<UnitPtr>::iterator it = units.begin(); it != units.end(); it++)
					m_WorldManager->updateVisibility(m_ActorManager->getActor((*it)->getId()), VISIBILITY_OPEN);
				for (std::list<StructurePtr>::iterator it = structures.begin(); it != structures.end(); it++)
					m_WorldManager->updateVisibility(m_ActorManager->getActor((*it)->getId()), VISIBILITY_OPEN);
			}
		
			glutSetWindow(m_StatsHandle);
			glutPostRedisplay();
		} else {
			ActorPtr ptr = m_ActorManager->getActor(actorId);
			m_WorldManager->setActorPosition( ptr, a_Position );
			GameTilePtr tile = m_WorldManager->getTile(a_Position);
			
			Vector3 destination = tile->getDrawPos();
			destination.y += 0.5f;
						
			switch (ptr->getType()) {
				case BOMBER:
				case AIRFIGHTER:
					destination.y += 1.5f;
					break;
				default:
					if (tile->getType() == T_SEA || tile->getType() == T_SEA2)
						destination.y -= 2.0f;
					break;
			}
			
			ptr->setDrawPosition(destination);
		}
	}
	
	void BaseGraphicsEngine::startGame( bool doLoad, std::list<IEventDataPtr> events) {
		
		m_DoLoad = doLoad;
		
		if (doLoad)
			m_Events = events;
		
		m_LastTime = clock();
		m_OpenMode = false;
		m_GetSaveFile = false;
		glutMainLoop();
	}
	
	// ----------------------------------------------
	// IEventListener implementation
	// ----------------------------------------------
	
	char const * BaseGraphicsEngine::GetName( void ) {
		return "Graphics Engine";
	}
	
	bool BaseGraphicsEngine::HandleEvent( IEventData & a_Event ) {
		EventType evtType = a_Event.VGetEventType();
		
		if (evtType == AttackEvent::sk_EventType) {
			// Handle attack event
			AttackEvent & attackEvent = static_cast<AttackEvent &>(a_Event);
			
			UnitActorPtr attacker = UnitActorPtr(boost::static_pointer_cast<UnitActor> (m_ActorManager->getActor(attackEvent.m_AttackerId)));
			Vector3 attackDirection = m_ActorManager->getActor(attackEvent.m_DefenderId)->getDrawPos();
			
			attacker->attack(attackDirection);
			
			// Animate the attack towards the defender
		} else if (evtType == BuildCancelledEvent::sk_EventType) {
			// Handle a build cancelled event
			//BuildCancelledEvent & buildCancelledEvent = static_cast<BuildCancelledEvent &>(a_Event);
			
			glutSetWindow(m_StatsHandle);
			glutPostRedisplay();
			
			// Refresh the structure data
		} else if (evtType == BuildEvent::sk_EventType) {
			// Handle a build event
			//BuildEvent & buildEvent = static_cast<BuildEvent &>(a_Event);
			
			//int structureId = buildEvent.m_StructureId;
			if (!m_DoLoad) {
				glutSetWindow(m_StatsHandle);
				glutPostRedisplay();
			}
		} else if (evtType == CreateObjectEvent::sk_EventType) {
			CreateObjectEvent & createEvent = static_cast<CreateObjectEvent &> (a_Event);
			addActor(createEvent.m_Params);
		} else if (evtType == FortifyEvent::sk_EventType) {
			// Handle a fortify event
			FortifyEvent & fortifyEvent = static_cast<FortifyEvent &>(a_Event);
			//boost::static_pointer_cast<UnitActor>(m_ActorManager->getActor(fortifyEvent.m_UnitId))->setFortified(fortifyEvent.m_Fortify);
		} else if (evtType == MoveUnitEvent::sk_EventType) {
			// Handle a move event
			MoveUnitEvent & moveEvent = static_cast<MoveUnitEvent &>(a_Event);
			
			//printf("\nMove: from %d, %d to %d, %d\n", moveEvent.m_Position.x, moveEvent.m_Position.y, moveEvent.m_Destination.x, moveEvent.m_Destination.y);
			moveActor(moveEvent.m_UnitId, moveEvent.m_Destination);
			
		} else if (evtType == ConquerEvent::sk_EventType) {
			// Handle a conquer event
			ConquerEvent & conquerEvent = static_cast<ConquerEvent &>(a_Event);
			
			int structureId = conquerEvent.m_StructureId;
			int playerId = m_ActorManager->getActor(conquerEvent.m_UnitId)->getPlayerId();
			
			StructureActorPtr structure = boost::static_pointer_cast<StructureActor>(m_ActorManager->getActor(structureId));
			structure->setPlayerId(playerId);
			
			removeActor(conquerEvent.m_UnitId);
			
			if (!m_OpenMode && !m_DoLoad) {
				// TODO:  Switch this back to m_PlayerId
				PlayerPtr player = m_GameEngine->getSpecificPlayer(m_PlayerId);
				
				std::list<UnitPtr> units = player->getUnitList();
				std::list<StructurePtr> structures = player->getStructureList();
				for (std::list<UnitPtr>::iterator it = units.begin(); it != units.end(); it++)
					m_WorldManager->updateVisibility(m_ActorManager->getActor((*it)->getId()), VISIBILITY_OPEN);
				for (std::list<StructurePtr>::iterator it = structures.begin(); it != structures.end(); it++)
					m_WorldManager->updateVisibility(m_ActorManager->getActor((*it)->getId()), VISIBILITY_OPEN);
			}
			// A structure was conquered, need to figure out who took it over
			// and make the appropriate adjustments
		} else if (evtType == AcquiredStructureEvent::sk_EventType) {
			AcquiredStructureEvent & acquiredEvent = static_cast<AcquiredStructureEvent &> (a_Event);
			
			int structureId = acquiredEvent.m_StructureId;
			int playerId = acquiredEvent.m_NewPlayerOwnerId;
			
			StructureActorPtr structure = boost::static_pointer_cast<StructureActor>(m_ActorManager->getActor(structureId));
			structure->setPlayerId(playerId);
			
			//printf("Local Player Id: %d", m_GameEngine->getActualPlayer()->getPlayerId() );
			
			if (!m_DoLoad && m_PlayerId == structure->getPlayerId())
				m_WorldManager->updateVisibility(structure, VISIBILITY_OPEN);
			
		} else if (evtType == UnitDestroyedEvent::sk_EventType) {
			// Handle a unit destroyed event
			UnitDestroyedEvent & unitDestroyedEvent = static_cast<UnitDestroyedEvent &>(a_Event);
			removeActor(unitDestroyedEvent.m_UnitId);
			// A unit was destroyed, show an animation and remove it from the system
			// Particles???
		} else if (evtType == ShellEvent::sk_EventType) {
			// Handle a shell event
			ShellEvent & shellEvent = static_cast<ShellEvent &>(a_Event);
			
			// Show a shelling animation
		} else if (evtType == RepairEvent::sk_EventType) {
			// Handle a repair event
			RepairEvent & repairEvent = static_cast<RepairEvent &>(a_Event);
			
			// A unit is being repaired... at least refresh the text display
		} else if (evtType == EndGameEvent::sk_EventType) {
			// Handle an end game event
			EndGameEvent & endGameEvent = static_cast<EndGameEvent &>(a_Event);
			glutSetWindow(m_GameHandle);
			glClear( GL_COLOR_BUFFER_BIT );
			char * text = "Game Over\0";
			glRasterPos2f(BORDER, BORDER);
			for ( int i = 0; text[i] != '\0'; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
			}
			glutSwapBuffers();
			usleep(200000);
			glutHideWindow();
			glutDestroyWindow(m_AppHandle);
			exit(0);
			// The game is over... show a splash screen or something
		} else if (evtType == GameOverEvent::sk_EventType) {
			// Handle a game over event
			GameOverEvent & gameOverEvent = static_cast<GameOverEvent &>(a_Event);
			
			// The game is over... but in a different way than before
		} else if (evtType == AttackDeniedEvent::sk_EventType) {
			// Handle attack denied
			AttackDeniedEvent & attackDeniedEvent = static_cast<AttackDeniedEvent &>(a_Event);
			
			// Show an error message
		} else if (evtType == RepairDeniedEvent::sk_EventType) {
			// Handle a repair denied event
			RepairDeniedEvent & repairDeniedEvent = static_cast<RepairDeniedEvent &>(a_Event);
			
			// Show an error message
		} else if (evtType == MoveDeniedEvent::sk_EventType) {
			// Handle a move denied event
			MoveDeniedEvent & moveDeniedEvent = static_cast<MoveDeniedEvent &>(a_Event);
			
			// Show an error message
		} else if (evtType == ShellDeniedEvent::sk_EventType) {
			// Handle a shell denied event
			ShellDeniedEvent & shellDeniedEvent = static_cast<ShellDeniedEvent &>(a_Event);
			
			// Show an error message
		} else if (evtType == FortifyDeniedEvent::sk_EventType) {
			// Handle a fortify denied event
			FortifyDeniedEvent & fortifyDeniedEvent = static_cast<FortifyDeniedEvent &>(a_Event);
			
			// Show an error message
		} else if (evtType == BuildDeniedEvent::sk_EventType) {

		} else if (evtType == BeginTurnEvent::sk_EventType) {
			BeginTurnEvent & evt = static_cast<BeginTurnEvent &>(a_Event);
			if (evt.m_PlayerId == playerID) {
				glutSetWindow(m_GameHandle);
				Color c = Actor::getColor(playerID);
				glClearColor(c.x, c.y, c.z, 1.0f);
			} else {
				glutSetWindow(m_GameHandle);
				glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			}
		}
		
		// Don't need to consume any events, so return false
		return false;
	}
	
	// ----------------------------------------------
	// Top level GLUT context
	// ----------------------------------------------
	
	void BaseGraphicsEngine::initializeAppWindow( std::string name ) {
		
		//*****************//
		// Initialize GLUT //
		//*****************//
		
		// Set the window's upper left corner position
		glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
		
		// Set the window's size
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		
		// Set the GLUT display mode
		//    RGBA or RGB - Color model mode
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		
		// The window can now be created
		m_AppHandle = glutCreateWindow( name.c_str() );
		
		// Erase the screen to black
		glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
		
		//****************//
		// GLUT Callbacks //
		//****************//
		glutIdleFunc(&GE_Idle);
		glutDisplayFunc(&GE_Display);
		glutReshapeFunc(&GE_Reshape);
		glutMouseFunc(&GE_Mouse);
		glutMotionFunc(&GE_MouseMotion);
		glutKeyboardFunc(&GE_Keyboard);
		
		//*************//
		// Sub Windows //
		//*************//
		
		m_GameHandle = glutCreateSubWindow(m_AppHandle, BORDER, MENU_HEIGHT, WINDOW_HEIGHT - 2 * BORDER, WINDOW_HEIGHT - BORDER - MENU_HEIGHT);
		initializeGameWindow();
		glutDisplayFunc(&GE_GameDisplay);
		glutReshapeFunc(&GE_GameReshape);
		glutPassiveMotionFunc(&GE_GamePassiveMouseMotion);
		glutMouseFunc(&GE_GameMouse);
		glutMotionFunc(&GE_GameMouseMotion);
		glutKeyboardFunc(&GE_GameKeyboard);
		
		m_MapHandle = glutCreateSubWindow(m_AppHandle, WINDOW_HEIGHT, MENU_HEIGHT, WINDOW_WIDTH - WINDOW_HEIGHT - BORDER, WINDOW_WIDTH - WINDOW_HEIGHT - MENU_HEIGHT);
		initializeMapWindow();
		glutDisplayFunc(&GE_MapDisplay);
		glutReshapeFunc(&GE_MapReshape);
		
		m_StatsHandle = glutCreateSubWindow(m_AppHandle, WINDOW_HEIGHT, WINDOW_WIDTH - WINDOW_HEIGHT, WINDOW_WIDTH - WINDOW_HEIGHT, WINDOW_HEIGHT - (WINDOW_WIDTH - WINDOW_HEIGHT));
		initializeStatsWindow();
		glutDisplayFunc(&GE_StatsDisplay);
		glutReshapeFunc(&GE_StatsReshape);
		glutMouseFunc(&GE_StatsMouse);
		glutKeyboardFunc(&GE_StatsKeyboard);

	}
	
	void BaseGraphicsEngine::reshapeAppWindow( int width, int height ) {
		
		m_WindowWidth = width;
		m_WindowHeight = height;
		
		glViewport(0, 0, width, height);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glScalef(1.0f, -1.0f, 1.0f);
		glTranslatef(0.0f, -height, 0.0f);

		int w = 2 * width / 3 - BORDER / 2;
		int h = height - MENU_HEIGHT;
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glutSetWindow(m_GameHandle);
		glutPositionWindow(0, MENU_HEIGHT);
		glutReshapeWindow(w, h);
		
		glutSetWindow(m_MapHandle);
		glutPositionWindow(w + BORDER, MENU_HEIGHT);
		w -= width / 3;
		h /= 3;
		glutReshapeWindow(w, h - BORDER / 2);
		
		glutSetWindow(m_StatsHandle);
		glutPositionWindow(w + width / 3 + BORDER, MENU_HEIGHT + h + BORDER);
		glutReshapeWindow(w, 2 * h - BORDER / 2);
	}
	
	void BaseGraphicsEngine::drawAppWindow( void ) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			drawMenu();
		
			glutSwapBuffers();
		
			glutPostRedisplay();
	}
	
	void BaseGraphicsEngine::mainMouse( int a_Button, int a_State, int a_X, int a_Y ) {
		if (a_Button == GLUT_LEFT_BUTTON) {
			if (a_State == GLUT_DOWN) {
				if (a_X >= m_FileStart && a_X <= m_FileStart + m_FileLength) {
					m_FileColor->x = 0.5f;
					m_FileColor->y = 0.5f;
					m_FileColor->z = 1.0f;
					m_MenuStatus = MENU_FILE;
				} else if (a_X >= m_ActionStart && a_X <= m_ActionStart + m_ActionLength) {
					m_ActionColor->x = 0.5f;
					m_ActionColor->y = 0.5f;
					m_ActionColor->z = 1.0f;
					m_MenuStatus = MENU_ACTION;
				} else if (a_X >= m_HelpStart && a_X <= m_HelpStart + m_HelpLength) {
					m_HelpColor->x = 0.5f;
					m_HelpColor->y = 0.5f;
					m_HelpColor->z = 1.0f;
					m_MenuStatus = MENU_HELP;
				}
			} else if (a_State == GLUT_UP) {
				
				if (m_MenuStatus == MENU_FILE) {
					// New Game
					if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  2 * MENU_HEIGHT && m_Mouse->y >= MENU_HEIGHT);
					
					// Join Game
					if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  3 * MENU_HEIGHT && m_Mouse->y >= 2 * MENU_HEIGHT);
					
					// Save Game
					if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  4 * MENU_HEIGHT && m_Mouse->y >= 3 * MENU_HEIGHT) {
						m_GetSaveFile = true;
						m_SaveName = std::string("File Name: ");
						m_FileName = std::string("");
					}
					
					// Load Game
					if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  5 * MENU_HEIGHT && m_Mouse->y >= 4 * MENU_HEIGHT);
					
					// Multiplayer Options
					if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  7 * MENU_HEIGHT && m_Mouse->y >= 6 * MENU_HEIGHT);
					
					// End Turn
					if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  8 * MENU_HEIGHT && m_Mouse->y >= 7 * MENU_HEIGHT);
					
					// Retire
					if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  10 * MENU_HEIGHT && m_Mouse->y >= 9 * MENU_HEIGHT);
					
					// Quit
					if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  11 * MENU_HEIGHT && m_Mouse->y >= 10 * MENU_HEIGHT) {
						glutHideWindow();
						glutDestroyWindow(m_AppHandle);
						exit(0);
					}
				} else if (m_MenuStatus == MENU_ACTION) {
					// Move
					if (m_Mouse->x >= m_FileStart && m_Mouse->x <= m_FileStart + m_SubmenuWidth && m_Mouse->y <=  2 * MENU_HEIGHT && m_Mouse->y >= MENU_HEIGHT) {
						m_SelectionMode = (m_SelectionMode == SM_MOVE) ? SM_NONE : SM_MOVE;
						
						// Special Ability
					} else if (m_Mouse->x >= m_FileStart && m_Mouse->x <= m_FileStart + m_SubmenuWidth && m_Mouse->y <=  3 * MENU_HEIGHT && m_Mouse->y >= 2 * MENU_HEIGHT) {
						m_SelectionMode = (m_SelectionMode == SM_SPECIAL) ? SM_NONE : SM_SPECIAL;
						// Next Unit
					} else if (m_Mouse->x >= m_FileStart && m_Mouse->x <= m_FileStart + m_SubmenuWidth && m_Mouse->y <=  5 * MENU_HEIGHT && m_Mouse->y >= 4 * MENU_HEIGHT) {
						std::list<UnitPtr> units = m_GameEngine->getSpecificPlayer(m_PlayerId)->getUnitList();
						if (units.empty()) {
							setSelectedActor(NullActorPtr);
							glutPostRedisplay();
							return;
						}
						
						m_USelectedIndex = (m_USelectedIndex + 1) % units.size();
						
						std::list<UnitPtr>::iterator it = units.begin();
						for (int i = 0; i < m_USelectedIndex; i++, it++);
							
						setSelectedActor(m_ActorManager->getActor((*it)->getId()));
						
						glutSetWindow(m_StatsHandle);
						glutPostRedisplay();
						
						// Next Structure
					} else if (m_Mouse->x >= m_FileStart && m_Mouse->x <= m_FileStart + m_SubmenuWidth && m_Mouse->y <=  6 * MENU_HEIGHT && m_Mouse->y >= 5 * MENU_HEIGHT) {
						std::list<StructurePtr> structures = m_GameEngine->getSpecificPlayer(m_PlayerId)->getStructureList();
						if (structures.empty()) {
							setSelectedActor(NullActorPtr);
							glutPostRedisplay();
							return;
						}
						
						m_SSelectedIndex = (m_SSelectedIndex + 1) % structures.size();
						
						std::list<StructurePtr>::iterator it = structures.begin();
						for (int i = 0; i < m_SSelectedIndex; i++, it++);
						
						setSelectedActor(m_ActorManager->getActor((*it)->getId()));
						
						glutSetWindow(m_StatsHandle);
						glutPostRedisplay();
					}
				}
				
				switch(m_MenuStatus) {
					case MENU_FILE:
						m_FileColor->x = 0.8f;
						m_FileColor->y = 0.8f;
						m_FileColor->z = 0.8f;
						m_MenuStatus = MENU_NONE;
						break;
					case MENU_ACTION:
						m_ActionColor->x = 0.8f;
						m_ActionColor->y = 0.8f;
						m_ActionColor->z = 0.8f;
						m_MenuStatus = MENU_NONE;
						break;
					case MENU_HELP:
						m_HelpColor->x = 0.8f;
						m_HelpColor->y = 0.8f;
						m_HelpColor->z = 0.8f;
						m_MenuStatus = MENU_NONE;
						break;
				}
				
				
			}
		}
	}
	
	void BaseGraphicsEngine::mainMouseMotion( int a_X, int a_Y ) {
		if (m_MenuStatus != MENU_FILE && a_Y <= MENU_HEIGHT && a_X >= m_FileStart && a_X <= m_FileStart + m_FileLength) {
			m_FileColor->x = 0.5f;
			m_FileColor->y = 0.5f;
			m_FileColor->z = 1.0f;
			m_ActionColor->x = 0.8f;
			m_ActionColor->y = 0.8f;
			m_ActionColor->z = 0.8f;
			m_HelpColor->x = 0.8f;
			m_HelpColor->y = 0.8f;
			m_HelpColor->z = 0.8f;
			m_MenuStatus = MENU_FILE;
		} else if (m_MenuStatus != MENU_ACTION && a_Y <= MENU_HEIGHT && a_X >= m_ActionStart && a_X <= m_ActionStart + m_ActionLength) {
			m_ActionColor->x = 0.5f;
			m_ActionColor->y = 0.5f;
			m_ActionColor->z = 1.0f;
			m_FileColor->x = 0.8f;
			m_FileColor->y = 0.8f;
			m_FileColor->z = 0.8f;
			m_HelpColor->x = 0.8f;
			m_HelpColor->y = 0.8f;
			m_HelpColor->z = 0.8f;
			m_MenuStatus = MENU_ACTION;
		} else if (m_MenuStatus != MENU_HELP && a_Y <= MENU_HEIGHT && a_X >= m_HelpStart && a_X <= m_HelpStart + m_HelpLength) {
			m_HelpColor->x = 0.5f;
			m_HelpColor->y = 0.5f;
			m_HelpColor->z = 1.0f;
			m_ActionColor->x = 0.8f;
			m_ActionColor->y = 0.8f;
			m_ActionColor->z = 0.8f;
			m_HelpColor->x = 0.8f;
			m_HelpColor->y = 0.8f;
			m_HelpColor->z = 0.8f;
			m_MenuStatus = MENU_HELP;
		} else if (a_Y <= MENU_HEIGHT && a_X > m_ActionStart + m_ActionLength && a_X < m_HelpStart) {
			m_FileColor->x = 0.8f;
			m_FileColor->y = 0.8f;
			m_FileColor->z = 0.8f;
			m_ActionColor->x = 0.8f;
			m_ActionColor->y = 0.8f;
			m_ActionColor->z = 0.8f;
			m_HelpColor->x = 0.8f;
			m_HelpColor->y = 0.8f;
			m_HelpColor->z = 0.8f;
			m_MenuStatus = MENU_NONE;
		} else {
			switch(m_MenuStatus) {
				case MENU_FILE:
					if (a_X < 0 || a_X > m_SubmenuWidth) {
						m_FileColor->x = 0.8f;
						m_FileColor->y = 0.8f;
						m_FileColor->z = 0.8f;
						m_MenuStatus = MENU_NONE;
					}
					break;
				case MENU_ACTION:
					if (a_X < m_FileLength || a_X > m_FileLength + m_SubmenuWidth) {
						m_ActionColor->x = 0.8f;
						m_ActionColor->y = 0.8f;
						m_ActionColor->z = 0.8f;
						m_MenuStatus = MENU_NONE;
					}
					break;
				case MENU_HELP:
					if (a_X < m_WindowWidth - m_HelpLength || a_X > m_WindowWidth) {
						m_HelpColor->x = 0.8f;
						m_HelpColor->y = 0.8f;
						m_HelpColor->z = 0.8f;
						m_MenuStatus = MENU_NONE;
					}
					break;
			}
		}
		m_Mouse->x = a_X;
		m_Mouse->y = a_Y;
	}
	
	void BaseGraphicsEngine::idleAppWindow( void ) {
				
		NetworkManager::get()->Poll();
		
		clock_t currentTime = clock();
		m_DeltaMilliseconds = (currentTime - m_LastTime) / CLOCK_TO_MILLIS;
		m_LastTime = currentTime;
		
		float scale = 0.035f * m_DeltaMilliseconds;
		
		switch (m_Camera->moveState)
		{
			case CMS_P_L:
				m_Camera->lookAt.x += m_Camera->panLR.x * scale;
				m_Camera->lookAt.y += m_Camera->panLR.y * scale;
				m_Camera->lookAt.z += m_Camera->panLR.z * scale;
				
				eyeVec.x = m_Camera->lookAt.x + m_Camera->r * cosf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
				eyeVec.y = m_Camera->lookAt.y + m_Camera->r * sinf(m_Camera->phi * DEG_TO_RAD);
				eyeVec.z = m_Camera->lookAt.z + m_Camera->r * sinf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
				break;
			case CMS_P_R:
				m_Camera->lookAt.x -= m_Camera->panLR.x * scale;
				m_Camera->lookAt.y -= m_Camera->panLR.y * scale;
				m_Camera->lookAt.z -= m_Camera->panLR.z * scale;
				
				eyeVec.x = m_Camera->lookAt.x + m_Camera->r * cosf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
				eyeVec.y = m_Camera->lookAt.y + m_Camera->r * sinf(m_Camera->phi * DEG_TO_RAD);
				eyeVec.z = m_Camera->lookAt.z + m_Camera->r * sinf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
				break;
			case CMS_P_U:
				m_Camera->lookAt.x += m_Camera->panUD.x * scale;
				m_Camera->lookAt.y += m_Camera->panUD.y * scale;
				m_Camera->lookAt.z += m_Camera->panUD.z * scale;
				
				eyeVec.x = m_Camera->lookAt.x + m_Camera->r * cosf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
				eyeVec.y = m_Camera->lookAt.y + m_Camera->r * sinf(m_Camera->phi * DEG_TO_RAD);
				eyeVec.z = m_Camera->lookAt.z + m_Camera->r * sinf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
				break;
			case CMS_P_D:
				m_Camera->lookAt.x -= m_Camera->panUD.x * scale;
				m_Camera->lookAt.y -= m_Camera->panUD.y * scale;
				m_Camera->lookAt.z -= m_Camera->panUD.z * scale;
				
				eyeVec.x = m_Camera->lookAt.x + m_Camera->r * cosf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
				eyeVec.y = m_Camera->lookAt.y + m_Camera->r * sinf(m_Camera->phi * DEG_TO_RAD);
				eyeVec.z = m_Camera->lookAt.z + m_Camera->r * sinf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
				break;
		}
				
		double deltaTime = (currentTime - m_LastTick) / CLOCK_TO_MILLIS;
		if (deltaTime >= 1000) {
			m_LastTick = currentTime;
			float deltaFrameTicks = (float) m_FrameTicks - (float) m_LastFrameTicks;
			m_LastFrameTicks = m_FrameTicks;
			sprintf(m_FramerateBuffer, "Current Player: %d Me: %d FPS: %3.2f", m_GameEngine->getActualPlayer()->getPlayerId(), m_PlayerId, (deltaFrameTicks / deltaTime) * 1000);
			glutSetWindow(m_StatsHandle);
			glutPostRedisplay();
		}
		
		if (m_DoLoad && m_Events.size() > 0) {
			IEventDataPtr evt = m_Events.front();
			safeQueueEvent(evt);
			m_Events.pop_front();
			
			safeTickEventManager(1000);
			safeTickEventManager(1000);
			if (m_Events.size() == 0) {
				m_DoLoad = false;
				loadMode = false;
				TileMatrix matrix = m_WorldManager->getTiles();
				for (TileMatrix::iterator it = matrix.begin(); it != matrix.end(); it++) {
					for (TileVector::iterator it2 = it->begin(); it2 != it->end(); it2++) {
						(*it2)->setVisibility(m_VisibilityStatus.front());
						m_VisibilityStatus.pop_front();
					}
				}
			}
		} else {
			safeTickEventManager(1000);
		}
	}
	
	void BaseGraphicsEngine::drawMenu( void ) {
		m_FileStart = 0;
		m_FileLength = 2 * BORDER + glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) m_FileMenu);
		
		glBegin(GL_QUADS);
		
		glColor3f(m_FileColor->x - 0.2f, m_FileColor->y - 0.2f, m_FileColor->z - 0.2f);
		
		glVertex3f(m_FileStart, MENU_HEIGHT, 0.0f);
		glVertex3f(m_FileLength, MENU_HEIGHT, 0.0f);
		
		glColor3f(m_FileColor->x, m_FileColor->y, m_FileColor->z);
		
		glVertex3f(m_FileLength, 0.0f, 0.0f);
		glVertex3f(m_FileStart, 0.0f, 0.0f);
		
		glEnd();
		
		m_ActionStart = m_FileStart + m_FileLength;
		m_ActionLength = 2 * BORDER + glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) m_ActionMenu);
		
		glBegin(GL_QUADS);
		
		glColor3f(m_ActionColor->x - 0.2f, m_ActionColor->y - 0.2f, m_ActionColor->z - 0.2f);
		
		glVertex3f(m_ActionStart, MENU_HEIGHT, 0.0f);
		glVertex3f(m_ActionStart + m_ActionLength, MENU_HEIGHT, 0.0f);
		
		glColor3f(m_ActionColor->x, m_ActionColor->y, m_ActionColor->z);
		
		glVertex3f(m_ActionStart + m_ActionLength, 0.0f, 0.0f);
		glVertex3f(m_ActionStart, 0.0f, 0.0f);
		
		glEnd();
		
		m_HelpLength = 2 * BORDER + glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) m_HelpMenu);
		m_HelpStart = m_WindowWidth - m_HelpLength;
		
		glBegin(GL_QUADS);
		
		glColor3f(0.6f, 0.6f, 0.6f);
		
		glVertex3f(m_ActionStart + m_ActionLength, MENU_HEIGHT, 0.0f);
		glVertex3f(m_HelpStart, MENU_HEIGHT, 0.0f);
		
		glColor3f(0.8f, 0.8f, 0.8f);
		
		glVertex3f(m_HelpStart, 0.0f, 0.0f);
		glVertex3f(m_ActionStart + m_ActionLength, 0.0f, 0.0f);
		
		glEnd();
		
		glBegin(GL_QUADS);
		
		glColor3f(m_HelpColor->x - 0.2f, m_HelpColor->y - 0.2f, m_HelpColor->z - 0.2f);
		
		glVertex3f(m_HelpStart, MENU_HEIGHT, 0.0f);
		glVertex3f(m_HelpStart + m_HelpLength, MENU_HEIGHT, 0.0f);
		
		glColor3f(m_HelpColor->x, m_HelpColor->y, m_HelpColor->z);
		
		glVertex3f(m_HelpStart + m_HelpLength, 0.0f, 0.0f);
		glVertex3f(m_HelpStart, 0.0f, 0.0f);
		
		glEnd();
		
		glColor3f(0.0f, 0.0f, 0.0f);
		
		glRasterPos2f(m_FileStart + BORDER, 11.0f);
		for (int i = 0; m_FileMenu[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, m_FileMenu[i]);
		
		glRasterPos2f(m_ActionStart + BORDER, 11.0f);
		for (int i = 0; m_ActionMenu[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, m_ActionMenu[i]);
		
		glRasterPos2f(m_HelpStart + BORDER, 11.0f);
		for (int i = 0; m_HelpMenu[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, m_HelpMenu[i]);
		
		if (m_GetSaveFile) {
			glRasterPos2f(m_ActionStart + m_ActionLength + BORDER, 11.0f);
			const char * t = m_SaveName.c_str();
			for (int i = 0; t[i] != '\0'; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, t[i]);
			}
			t = m_FileName.c_str();
			for (int i = 0; t[i] != '\0'; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, t[i]);
			}
		}
	}
	
	int BaseGraphicsEngine::doPick( int a_ViewId, int a_X, int a_Y ) {
		GLint viewport[4];
		
		glutSetWindow(a_ViewId);
		
		m_PickBuffer = new GLuint[PICK_BUFFER_SIZE];
		glSelectBuffer(PICK_BUFFER_SIZE, m_PickBuffer);
		glRenderMode(GL_SELECT);
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		glGetIntegerv(GL_VIEWPORT, viewport);
		gluPickMatrix(a_X, viewport[3] - a_Y,
					    5,                 5, viewport);
		gluPerspective(45,1.0,0.1,1000);
		glMatrixMode(GL_MODELVIEW);
		glInitNames();
		
		// Draw the scene here
		m_WorldManager->draw3D(a_ViewId, 0);
		
		int hits;
		
		// restoring the original projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glFlush();
		
		// returning to normal rendering mode
		hits = glRenderMode(GL_RENDER);
		
		int retVal = -1;
		
		if (hits > 0)
		{
			GLuint names, *ptr, minZ,*ptrNames, numberOfNames;
			
			ptr = m_PickBuffer;
			minZ = 0xffffffff;
			for (int i = 0; i < hits; i++) 
			{	
				names = *ptr;
				ptr++;
				if (*ptr < minZ) {
					numberOfNames = names;
					minZ = *ptr;
					ptrNames = ptr+2;
				}
				
				ptr += names+2;
			}
			
			retVal = (int) (*(ptrNames + (numberOfNames - 1)));
		}
		
		delete m_PickBuffer;
		return retVal;
	}		
	
	void BaseGraphicsEngine::mainKeyboard( unsigned char a_Key, int a_X, int a_Y ) {
		
		if (a_Key == 13) {
			SaveGameEvent * saveGame = new SaveGameEvent( m_PlayerId,  m_FileName );
			IEventDataPtr ptr(saveGame);
			safeQueueEvent(ptr);
			m_GetSaveFile = false;
		} else if (a_Key == 127) {
			if (m_FileName.length() > 0)
				m_FileName.erase(m_FileName.length() - 1);
		} else {
			m_FileName += a_Key;
		}
	}
	
	BaseGraphicsEngine::BaseGraphicsEngine(GameEngine * a_GameEngine) {
		
		m_GameEngine = a_GameEngine;
		
		// Register as a listener to the proper events
		EventListenerPtr graphicsPtr = EventListenerPtr(this);
		safeAddListener( graphicsPtr, AttackEvent::sk_EventType );
		safeAddListener( graphicsPtr, AttackDeniedEvent::sk_EventType );
		safeAddListener( graphicsPtr, BuildCancelledEvent::sk_EventType );
		safeAddListener( graphicsPtr, BuildEvent::sk_EventType );
		safeAddListener( graphicsPtr, BuildDeniedEvent::sk_EventType );
		safeAddListener( graphicsPtr, FortifyEvent::sk_EventType );
		safeAddListener( graphicsPtr, FortifyDeniedEvent::sk_EventType );
		safeAddListener( graphicsPtr, MoveUnitEvent::sk_EventType );
		safeAddListener( graphicsPtr, MoveDeniedEvent::sk_EventType );
		safeAddListener( graphicsPtr, ConquerEvent::sk_EventType );
		safeAddListener( graphicsPtr, UnitDestroyedEvent::sk_EventType );
		safeAddListener( graphicsPtr, ShellEvent::sk_EventType );
		safeAddListener( graphicsPtr, ShellDeniedEvent::sk_EventType );
		safeAddListener( graphicsPtr, RepairEvent::sk_EventType );
		safeAddListener( graphicsPtr, RepairDeniedEvent::sk_EventType );
		safeAddListener( graphicsPtr, EndGameEvent::sk_EventType );
		safeAddListener( graphicsPtr, GameOverEvent::sk_EventType );
		safeAddListener( graphicsPtr, CreateObjectEvent::sk_EventType );
		safeAddListener( graphicsPtr, AcquiredStructureEvent::sk_EventType );
		safeAddListener( graphicsPtr, BeginTurnEvent::sk_EventType );
		
		me = this;
	}
	
	void BaseGraphicsEngine::serializeGameTiles( std::stringstream & ss ) {
		TileMatrix matrix = m_WorldManager->getTiles();
		for (TileMatrix::iterator it = matrix.begin(); it != matrix.end(); it++) {
			for (TileVector::iterator it2 = it->begin(); it2 != it->end(); it2++) {
				ss << (int) (*it2)->getVisibility() << " ";
			}
		}
	}
	
	void BaseGraphicsEngine::deserializeGameTiles( std::stringstream & ss ) {
		for (int i = 0; i < 1600; i++) {
			int v = 0;
			ss >> v;
			m_VisibilityStatus.push_back((Visibility) v);
		}
	}
}
