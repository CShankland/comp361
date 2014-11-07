/*
 *  MainGameView.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/1/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsEngine.h"
#include "Events.h"

namespace IslandWars
{			
	void BaseGraphicsEngine::initializeGameWindow( void ) {
		
		m_GameMouse = new MouseData;
		
		// Erase the screen to light blue
		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		
		// Max depth to clear
		glClearDepth(1.0f);
		
		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		
		// Type of depth testing
		glDepthFunc(GL_LEQUAL);
		
		// Define vertex winding
		glFrontFace(GL_CCW);
		
		glShadeModel(GL_SMOOTH);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);			// Really Nice Perspective Calculations
		glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);	

		float pos[4] = { -100.0f, 50.0f, -100.0f, 1.0f };
		float col[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
		glEnable(GL_LIGHT0);
		
		float pos2[4] = { -100.0f, 50.0f, 100.0f, 1.0f };
		glLightfv(GL_LIGHT1, GL_POSITION, pos2);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, col);
		glEnable(GL_LIGHT1);
		
		float pos3[4] = { 100.0f, 50.0f, 100.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_POSITION, pos3);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, col);
		glEnable(GL_LIGHT2);
		
		float pos4[4] = { 100.0f, 50.0f, -100.0f, 1.0f };
		glLightfv(GL_LIGHT3, GL_POSITION, pos4);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, col);
		glEnable(GL_LIGHT3);
		
		m_Camera->lookAt.x = 0;
		m_Camera->lookAt.y = 2;
		m_Camera->lookAt.z = 0;
		
		m_Camera->r = 5.0;
		m_Camera->theta = 0;
		m_Camera->phi = 0;	
		
		eyeVec.x = m_Camera->lookAt.x + m_Camera->r * cosf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
		eyeVec.y = m_Camera->lookAt.y + m_Camera->r * sinf(m_Camera->phi * DEG_TO_RAD);
		eyeVec.z = m_Camera->lookAt.z + m_Camera->r * sinf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
		
		Vector4 camVec = Vector4(eyeVec.x - m_Camera->lookAt.x, eyeVec.y - m_Camera->lookAt.y, eyeVec.z - m_Camera->lookAt.z, 1.0);
		m_Camera->panLR = camVec.cross(Y_UP);
		m_Camera->panLR.Normalize();
		m_Camera->panUD = m_Camera->panLR.cross(Y_UP);
		
		m_WorldManager->registerView( m_GameHandle );
		m_ActorManager->registerView( m_GameHandle );
		
		m_pSelectedActor = NullActorPtr;
		m_SelectionMode = SM_NONE;
	}

	void BaseGraphicsEngine::drawGameWindow( void ) {
		
		switch (m_SelectionMode) {
			case SM_MOVE:
				glutSetCursor(GLUT_CURSOR_INFO);
				break;
			case SM_ATTACK:
				glutSetCursor(GLUT_CURSOR_CROSSHAIR);
				break;
			case SM_SPECIAL:
				glutSetCursor(GLUT_CURSOR_DESTROY);
				break;
			default:
				glutSetCursor(GLUT_CURSOR_INHERIT);
				break;
		}
		
		m_FrameTicks++;
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glLoadIdentity();

		gluLookAt( eyeVec.x, eyeVec.y, eyeVec.z,
				   m_Camera->lookAt.x, m_Camera->lookAt.y, m_Camera->lookAt.z,
				   Y_UP.x, Y_UP.y, Y_UP.z );

		m_WorldManager->draw3D( m_GameHandle, m_DeltaMilliseconds );
		
		// Two pass rendering
		
		//glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		//glEnable(GL_COLOR_MATERIAL);
		m_ActorManager->draw3D( m_GameHandle, m_DeltaMilliseconds );
		
		//m_ActorManager->draw3D( m_GameHandle, m_DeltaMilliseconds );
		//glDisable(GL_BLEND);
		//glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
		
		if (m_MenuStatus == MENU_FILE) {
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0, m_GameWidth, 0, m_GameHeight);
			glScalef(1.0f, -1.0f, 1.0f);
			glTranslatef(0.0f, -m_GameHeight, 0.0f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			
			const char * saveText = "Save Game\0";
			const char * loadText = "Load Game\0";
			const char * joinText = "Join Game\0";
			const char * newText = "New Game\0";
			const char * multiplayerText = "Multiplayer Options\0";
			const char * retireText = "Retire\0";
			const char * endText = "End Turn\0";
			const char * quitText = "Quit\0";
			
			m_SubmenuWidth = BORDER + glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) multiplayerText);
			
			// New Game
			glBegin(GL_QUADS);
			if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  2 * MENU_HEIGHT && m_Mouse->y >= MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glEnd();
			
			// Join Game
			glBegin(GL_QUADS);
			if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  3 * MENU_HEIGHT && m_Mouse->y >= 2 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 2 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 2 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, MENU_HEIGHT, 0.0f);
			glEnd();
			
			// Save Game
			glBegin(GL_QUADS);
			if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  4 * MENU_HEIGHT && m_Mouse->y >= 3 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 3 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 3 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 2 * MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, 2 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			// Load Game
			glBegin(GL_QUADS);
			if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  5 * MENU_HEIGHT && m_Mouse->y >= 4 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 4 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 4 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 3 * MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, 3 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			glBegin(GL_QUADS);
			glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 5 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 5 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 4 * MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, 4 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			// Multiplayer Options
			glBegin(GL_QUADS);
			if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  7 * MENU_HEIGHT && m_Mouse->y >= 6 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 6 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 6 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 5 * MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, 5 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			// End Turn
			glBegin(GL_QUADS);
			if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  8 * MENU_HEIGHT && m_Mouse->y >= 7 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 7 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 7 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 6 * MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, 6 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			glBegin(GL_QUADS);
			glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 8 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 8 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 7 * MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, 7 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			// Retire
			glBegin(GL_QUADS);
			if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  10 * MENU_HEIGHT && m_Mouse->y >= 9 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 9 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 9 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 8 * MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, 8 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			// Quit
			glBegin(GL_QUADS);
			if (m_Mouse->x >= 0 && m_Mouse->x <= m_SubmenuWidth && m_Mouse->y <=  11 * MENU_HEIGHT && m_Mouse->y >= 10 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(0.0f, 10 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 10 * MENU_HEIGHT, 0.0f);
			glVertex3f(m_SubmenuWidth, 9 * MENU_HEIGHT, 0.0f);
			glVertex3f(0.0f, 9 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			glColor3f(0.2f, 0.2f, 0.2f);
			glRasterPos2f(0.0f, 11.0f);
			for (int i = 0; newText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, newText[i]);
			
			glRasterPos2f(0.0f, MENU_HEIGHT + 11.0f);
			for (int i = 0; joinText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, joinText[i]);
			
			glRasterPos2f(0.0f, 2 * MENU_HEIGHT + 11.0f);
			for (int i = 0; saveText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, saveText[i]);
			
			glRasterPos2f(0.0f, 3 * MENU_HEIGHT + 11.0f);
			for (int i = 0; loadText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, loadText[i]);
			
			glRasterPos2f(0.0f, 5 * MENU_HEIGHT + 11.0f);
			for (int i = 0; multiplayerText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, multiplayerText[i]);
			
			glRasterPos2f(0.0f, 6 * MENU_HEIGHT + 11.0f);
			for (int i = 0; endText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, endText[i]);
			
			glRasterPos2f(0.0f, 8 * MENU_HEIGHT + 11.0f);
			for (int i = 0; retireText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, retireText[i]);
			
			glRasterPos2f(0.0f, 9 * MENU_HEIGHT + 11.0f);
			for (int i = 0; quitText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, quitText[i]);
			
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			
		} else if (m_MenuStatus == MENU_ACTION) {
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0, m_GameWidth, 0, m_GameHeight);
			glScalef(1.0f, -1.0f, 1.0f);
			glTranslatef(0.0f, -m_GameHeight, 0.0f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			
			const char * moveText = "Move\0";
			const char * specialText = "Special Ability\0";
			const char * nextUText = "Next Unit\0";
			const char * nextSText = "Next Structure\0";
			
			m_SubmenuWidth = BORDER + glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) nextSText);
			int offset = 2 * BORDER + glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) m_FileMenu);
			
			// Move
			glBegin(GL_QUADS);
			if (m_Mouse->x >= offset && m_Mouse->x <= offset + m_SubmenuWidth && m_Mouse->y <=  2 * MENU_HEIGHT && m_Mouse->y >= MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(offset, MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, 0.0f, 0.0f);
			glVertex3f(offset, 0.0f, 0.0f);
			glEnd();
			
			// Special Ability
			glBegin(GL_QUADS);
			if (m_Mouse->x >= offset && m_Mouse->x <= offset + m_SubmenuWidth && m_Mouse->y <=  3 * MENU_HEIGHT && m_Mouse->y >= 2 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(offset, 2 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, 2 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, MENU_HEIGHT, 0.0f);
			glVertex3f(offset, MENU_HEIGHT, 0.0f);
			glEnd();
			
			glBegin(GL_QUADS);
			glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(offset, 3 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, 3 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, 2 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset, 2 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			// Next Unit
			glBegin(GL_QUADS);
			if (m_Mouse->x >= offset && m_Mouse->x <= offset + m_SubmenuWidth && m_Mouse->y <=  5 * MENU_HEIGHT && m_Mouse->y >= 4 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(offset, 4 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, 4 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, 3 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset, 3 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			// Next Structure
			glBegin(GL_QUADS);
			if (m_Mouse->x >= offset && m_Mouse->x <= offset + m_SubmenuWidth && m_Mouse->y <=  6 * MENU_HEIGHT && m_Mouse->y >= 5 * MENU_HEIGHT)
				glColor3f(0.4f, 0.4f, 1.0f);
			else
				glColor3f(0.8f, 0.8f, 0.8f);
			glVertex3f(offset, 5 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, 5 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset + m_SubmenuWidth, 4 * MENU_HEIGHT, 0.0f);
			glVertex3f(offset, 4 * MENU_HEIGHT, 0.0f);
			glEnd();
			
			glColor3f(0.2f, 0.2f, 0.2f);
			glRasterPos2f(offset, 11.0f);
			for (int i = 0; moveText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, moveText[i]);
			
			glRasterPos2f(offset, MENU_HEIGHT + 11.0f);
			for (int i = 0; specialText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, specialText[i]);
			
			glRasterPos2f(offset, 3 * MENU_HEIGHT + 11.0f);
			for (int i = 0; nextUText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, nextUText[i]);
			
			glRasterPos2f(offset, 4 * MENU_HEIGHT + 11.0f);
			for (int i = 0; nextSText[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, nextSText[i]);
			
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		
		glutSwapBuffers();
		glutPostRedisplay();
	}
		
	void BaseGraphicsEngine::reshapeGameWindow( int a_Width, int a_Height ) {
		if (a_Height == 0)
			a_Height = 1;
		
		glViewport(0, 0, a_Width, a_Height);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		// Set up the projection to be a 45 degree yfovy
		gluPerspective(45.0, 1.0, 1, 1000);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		m_GameWidth = a_Width;
		m_GameHeight = a_Height;
	}
	
	void BaseGraphicsEngine::gamePassiveMouseMotion( int a_X, int a_Y ) {

		if ( a_X < 30 && a_X > 0) 
		{
			m_Camera->moveState = CMS_P_L;
		} 
		else if ( a_X > (m_GameWidth - 30) && a_X < m_GameWidth)
		{
			m_Camera->moveState = CMS_P_R;
		}
		
		if (a_Y < 30 && a_Y > 0)
		{
			m_Camera->moveState = CMS_P_U;
		}
		else if ( a_Y > (m_GameHeight - 30)  && a_Y < m_GameHeight)
		{
			m_Camera->moveState = CMS_P_D;
		}
		
		if (a_X > m_GameWidth || a_X < 0 || a_Y > m_GameHeight || a_Y < 0 || (a_X > 30 && a_X < (m_GameWidth - 30) && a_Y > 30 && a_Y < (m_GameHeight - 30)))
			m_Camera->moveState = CMS_NONE;
	
	}
	
	void BaseGraphicsEngine::gameMouse( int a_Button, int a_State, int a_X, int a_Y ) {
		switch (a_Button) {
			case GLUT_LEFT_BUTTON:
				switch (a_State) {
					case GLUT_DOWN:
						m_GameMouse->state = 1;
						m_GameMouse->x = a_X;
						m_GameMouse->y = a_Y;
						m_GameMouse->lastPress = clock();
						break;
					case GLUT_UP:
						m_GameMouse->state = -1;
						if (clock() - m_GameMouse->lastPress < CLICK_THRESHOLD) {
							int selection = doPick(m_GameHandle, m_GameMouse->x, m_GameMouse->y);
							//printf("Selection: %d\n", selection);
							if (selection != -1) {
								int x = selection >> 16;
								int y = (short) selection;
								Vector2 mapPosition = Vector2(x, y);
								GameTilePtr gameTile = m_WorldManager->getTile( mapPosition );
								if (m_SelectionMode == SM_NONE) {
									ActorList actorList = m_WorldManager->getContainedActors( mapPosition );
									if (actorList.size() == 1) {
										setSelectedActor(actorList.front());
									} else if (actorList.size() > 1) {
										setSelectedActors( actorList );
									}
								} else if (m_SelectionMode == SM_MOVE) {
									MoveRequestEvent * moveRequest = new MoveRequestEvent( m_pSelectedActor->getActorId(), m_pSelectedActor->getMapPos(), mapPosition );
									IEventDataPtr eventPtr( moveRequest );
									safeQueueEvent(eventPtr);
								} else if (m_SelectionMode == SM_ATTACK) {
									MoveRequestEvent * moveRequest = new MoveRequestEvent( m_pSelectedActor->getActorId(), m_pSelectedActor->getMapPos(), mapPosition );
									IEventDataPtr eventPtr( moveRequest );
									safeQueueEvent(eventPtr);
								} else if (m_SelectionMode == SM_SPECIAL) {
									ObjectType type = m_pSelectedActor->getType();
									IEventData * event = NULL;
									ActorList actorList = m_WorldManager->getContainedActors( mapPosition);
									
									switch (type) {
										case ARTILLERY:
										case BATTLESHIP:
											for (ActorList::iterator it = actorList.begin(); it != actorList.end(); it++) {
												if ((*it)->getType() != LAND_CITY && (*it)->getType() != PORT_CITY && (*it)->getType() != TOWER) {
													event = new ShellRequestEvent( m_pSelectedActor->getActorId(), m_pSelectedActor->getMapPos(), (*it)->getActorId(), (*it)->getMapPos() );
													break;
												}
											}
											if (event == NULL)
												event = new ShellDeniedEvent( m_pSelectedActor->getActorId(), mapPosition );
											break;
										case BOMBER:
											for (ActorList::iterator it = actorList.begin(); it != actorList.end(); it++) {
												if ((*it)->getType() == LAND_CITY || (*it)->getType() == PORT_CITY || (*it)->getType() == TOWER) {
													event = new BombRequestEvent( m_pSelectedActor->getActorId(), m_pSelectedActor->getMapPos(), (*it)->getActorId(), (*it)->getMapPos() );
													break;
												}
											}
											if (event == NULL)
												event = new BombDeniedEvent( m_pSelectedActor->getActorId(), mapPosition );
											break;
									}
									
									if (event == NULL) {
										m_SelectionMode = SM_NONE;
									} else {
										IEventDataPtr eventPtr( event );
										safeQueueEvent(eventPtr);
									}
								}
							} else {
								if (m_SelectionMode == SM_NONE) {
									setSelectedActor(NullActorPtr);
								} else {
									m_SelectionMode = SM_NONE;
								}
							}
						}
				}
				break;
			case GLUT_RIGHT_BUTTON:
				switch (a_State) {
					case GLUT_DOWN:
						m_GameMouse->state = 2;
						m_GameMouse->y = a_Y;
						break;
					case GLUT_UP:
						m_GameMouse->state = -1;
						break;
				}
		}
	}
	
	void BaseGraphicsEngine::gameMouseMotion( int a_X, int a_Y ) {
		if (m_GameMouse->state == 1) 
		{
			int dX = (a_X - m_GameMouse->x) * 0.25;
			int dY = (a_Y - m_GameMouse->y) * 0.25;
			
			m_GameMouse->x = a_X;
			m_GameMouse->y = a_Y;
			
			m_Camera->theta = (m_Camera->theta + dX) % 360;
			
			m_Camera->phi = (m_Camera->phi + dY);
			m_Camera->phi = (m_Camera->phi < 0) ? 0 : m_Camera->phi;
			m_Camera->phi = (m_Camera->phi > 89) ? 89 : m_Camera->phi;
			
			eyeVec.x = m_Camera->lookAt.x + m_Camera->r * cosf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
			eyeVec.y = m_Camera->lookAt.y + m_Camera->r * sinf(m_Camera->phi * DEG_TO_RAD);
			eyeVec.z = m_Camera->lookAt.z + m_Camera->r * sinf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
			
			Vector4 camVec = Vector4(eyeVec.x - m_Camera->lookAt.x, eyeVec.y - m_Camera->lookAt.y, eyeVec.z - m_Camera->lookAt.z, 1.0);
			m_Camera->panLR = camVec.cross(Y_UP);
			m_Camera->panLR.Normalize();
			m_Camera->panUD = m_Camera->panLR.cross(Y_UP);
		} 
		else if (m_GameMouse->state == 2) 
		{
			int dY = (a_Y - m_GameMouse->y);
			m_GameMouse->y = a_Y;
			
			m_Camera->r += dY;
			m_Camera->r = (m_Camera->r > 1.0f) ? m_Camera->r : 1.0f;
			
			eyeVec.x = m_Camera->lookAt.x + m_Camera->r * cosf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
			eyeVec.y = m_Camera->lookAt.y + m_Camera->r * sinf(m_Camera->phi * DEG_TO_RAD);
			eyeVec.z = m_Camera->lookAt.z + m_Camera->r * sinf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
		}
	}
	
	void BaseGraphicsEngine::gameKeyboard( unsigned char a_Key, int a_X, int a_Y ) {
		switch (a_Key) {
			case 'm':
				m_SelectionMode = (m_SelectionMode == SM_MOVE) ? SM_NONE : SM_MOVE;
				break;
			case 'a':
				m_SelectionMode = (m_SelectionMode == SM_ATTACK) ? SM_NONE : SM_ATTACK;
				break;
			case 's':
				m_SelectionMode = (m_SelectionMode == SM_SPECIAL) ? SM_NONE : SM_SPECIAL;
				break;
			case 'f':
				if (m_pSelectedActor != NullActorPtr) {
					if (m_pSelectedActor->getType() != TOWER && m_pSelectedActor->getType() != PORT_CITY && m_pSelectedActor->getType() != LAND_CITY) {
						bool fortify = m_GameEngine->getUnitPointer( m_pSelectedActor->getActorId(), m_pSelectedActor->getMapPos() );
						FortifyRequestEvent * req = new FortifyRequestEvent( m_pSelectedActor->getActorId(), m_pSelectedActor->getMapPos(), !fortify );
						IEventDataPtr ptr(req);
						safeQueueEvent(ptr);
					}
				}
				break;
			case 'o':
				m_OpenMode = !m_OpenMode;
				m_ActorManager->setOpenMode(m_OpenMode);
				break;
		}
		
		if (m_pSelectedActor == NullActorPtr)
			m_SelectionMode = SM_NONE;
	}
}; // Namespace IslandWars
