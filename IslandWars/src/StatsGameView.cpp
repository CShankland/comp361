/*
 *  StatsGameView.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/1/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "Structure.h"

#include <boost/pointer_cast.hpp>
#include <list>
#include <cmath>

namespace IslandWars
{
	void BaseGraphicsEngine::initializeStatsWindow( void ) {
		
		m_FramerateBuffer = new char[50];
		m_StatsMouse = new MouseData;
		
		m_LastTick = clock();
		m_LastFrameTicks = m_FrameTicks = 0;
		
		// Erase the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		
		// Max depth to clear
		glClearDepth(1.0f);
		
		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		
		// Type of depth testing
		glDepthFunc(GL_LEQUAL);
		
		m_ActorManager->registerView(m_StatsHandle);

		int dlId = glGenLists(1);
		
		glNewList(dlId, GL_COMPILE);
		
		glColor3f(0.5f, 0.5f, 0.8f);
		
		glBegin(GL_QUADS);
		glVertex3f(0.5f, 0.5f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
		glEnd();
		
		glEndList();
		
		DrawData * drawData = new DrawData;
		drawData->dl2D = dlId;
		drawData->dl3D = dlId;
		
		TextureDrawable * drawable = new TextureDrawable();
		drawable->attachView(m_StatsHandle, drawData);
		m_DrawInfo = DrawablePtr( drawable );
		
		Vector2 nuPos( 0, 0 );
		Vector2 nsPos( 0, 0 );
		Vector2 etPos( 0, 0 );
		
		nextUButton = new Button( BUTTON_NEXT_UNIT, 100, 25, m_StatsHandle, m_DrawInfo, nuPos );
		nextSButton = new Button( BUTTON_NEXT_STRUCTURE, 100, 25, m_StatsHandle, m_DrawInfo, nsPos );
		endTurnButton = new Button( BUTTON_END_TURN, 200 + BORDER, 25, m_StatsHandle, m_DrawInfo, etPos );
		
		nextUText = "Next Unit\0";
		nextSText = "Next Structure\0";
		endTurnText = "End Turn\0";
		
		m_StructureMenuId = 0;
	}
	
	void BaseGraphicsEngine::drawStatsWindow( void ) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glPushMatrix();
		nextUButton->draw();
		nextSButton->draw();
		endTurnButton->draw();

		for (ButtonList::iterator it = m_Buttons.begin(); it != m_Buttons.end(); it++)
			(*it)->draw();
		
		glColor3f(0.2f, 0.2f, 0.2f);
		
		int width = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) nextUText);
		glRasterPos2f(nextUButton->getPosition().x - width / 2, nextUButton->getPosition().y + 4);
		for (int i = 0; nextUText[i] != '\0'; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, nextUText[i]);
		}
		
		width = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) nextSText);
		glRasterPos2f(nextSButton->getPosition().x - width / 2, nextSButton->getPosition().y + 4);
		for (int i = 0; nextSText[i] != '\0'; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, nextSText[i]);
		}
		
		width = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) endTurnText);
		glRasterPos2f(endTurnButton->getPosition().x - width / 2, endTurnButton->getPosition().y + 4);
		for (int i = 0; endTurnText[i] != '\0'; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, endTurnText[i]);
		}
		
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		if (m_pSelectedActor != NullActorPtr) {
			switch (m_pSelectedActor->getType()) {
				case LAND_CITY:
				case PORT_CITY:
				case TOWER:
					buildStructureStats( m_pSelectedActor );
					switch (m_StructureMenuId) {
						case 0:
							buildStructureMainMenu( m_pSelectedActor );
							break;
						case 1:
							buildStructureBuildMenu( m_pSelectedActor );
							break;
					}
					break;
				default:
					buildUnitStats( m_pSelectedActor );
			}
		}
		
		glPopMatrix();

		glPushMatrix();
		glColor3d(1.0, 1.0, 1.0);
		
		int length = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) m_FramerateBuffer);
		
		glRasterPos2d(m_StatsWidth - length, 10);
		for (int i = 0; m_FramerateBuffer[i] != '\0'; i++)  {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, m_FramerateBuffer[i]);
		}
		glPopMatrix();
		glutSwapBuffers();
	}
	
	void BaseGraphicsEngine::reshapeStatsWindow( int a_Width, int a_Height ) {
		if (a_Height == 0)
			a_Height = 1;
		
		glViewport(0, 0, a_Width, a_Height);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		// Set up an orthographic projection where 1 pixel is 1 unit of screen space
		gluOrtho2D(0, a_Width, 0, a_Height);
		
		// Invert the Y axis
		glScalef(1, -1, 1);
		
		// Move the origin to the upper left corner
		glTranslatef(0, -a_Height, 0);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		m_StatsWidth = a_Width;
		m_StatsHeight = a_Height;
		
		Vector2 pos( (a_Width - 3 * BORDER) / 2 - nextUButton->getWidth() / 2,  a_Height - BORDER * 2 - endTurnButton->getHeight() - nextUButton->getHeight() / 2);
		Vector2 pos2( (a_Width - 3 * BORDER) / 2 + nextSButton->getWidth() / 2 + BORDER,  a_Height - BORDER * 2 - endTurnButton->getHeight() - nextSButton->getHeight() / 2);
		Vector2 pos3( (a_Width - 2 * BORDER) / 2,  a_Height - BORDER * 2 - endTurnButton->getHeight() / 2);
		
		nextUButton->setPosition(pos);
		nextSButton->setPosition(pos2);
		endTurnButton->setPosition(pos3);
		
		glutPostRedisplay();
	}
	
	void BaseGraphicsEngine::statsMouse( int a_Button, int a_State, int a_X, int a_Y ) {
		switch (a_Button) {
			case GLUT_LEFT_BUTTON:
				switch (a_State) {
					case GLUT_DOWN:
						m_Selection = doButtonPick( a_X, a_Y );
						if (m_Selection) {
							m_Selection->setSelected(true);
							glutPostRedisplay();
						}
						
						break;
					case GLUT_UP:
						if (m_Selection) {
							m_Selection->setSelected(false);
							
							if (m_Selection->getName() == BUTTON_STRUCTURE_BUILD_MENU && m_pSelectedActor->getPlayerId() == m_PlayerId) {
								m_StructureMenuId = 1;
								m_Buttons.clear();
								glutPostRedisplay();
								return;
							}
							
							if (m_Selection == endTurnButton /*&& m_GameEngine->getActualPlayer()->getPlayerId() == m_PlayerId*/) {
								//endTurnButton->setEnabled(false);
								EndTurnEvent * endTurn = new EndTurnEvent( m_PlayerId );
								IEventDataPtr ptr( endTurn );
								safeQueueEvent(ptr);
								glutPostRedisplay();
								return;
							}
							
							if (m_Selection == nextUButton) {
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
								return;
							}
							
							if (m_Selection == nextSButton) {
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
								return;
							}
							
							if (m_Selection->getName() > -100 && m_Selection->getName() < 0) {
								BuildRequestEvent * build = new BuildRequestEvent( m_pSelectedActor->getActorId(), m_pSelectedActor->getMapPos(), (ObjectType) (-1 * m_Selection->getName()));
								IEventDataPtr ptr( build );
								safeQueueEvent(ptr);
								m_StructureMenuId = 0;
								m_Buttons.clear();
								glutPostRedisplay();
								return;
							}
							
							if (m_Selection->getName() >= 0) {
								if (m_pSelectedActor != NullActorPtr && (m_pSelectedActor->getType() == TOWER || m_pSelectedActor->getType() == LAND_CITY || m_pSelectedActor->getType() == PORT_CITY )) {
									BuildCancelledEvent * cancel = new BuildCancelledEvent( m_pSelectedActor->getActorId(), m_pSelectedActor->getMapPos(), m_Selection->getName() );
									IEventDataPtr p(cancel);
									safeQueueEvent(p);
								} else {
									setSelectedActor(m_ActorManager->getActor(m_Selection->getName()));
								}
							}
						}
						glutPostRedisplay();
						break;
				}
				break;
			case GLUT_RIGHT_BUTTON:
				switch (a_State) {
					case GLUT_DOWN:
						break;
					case GLUT_UP:
						break;
				}
		}
	}
	
	void BaseGraphicsEngine::setSelectedActor( ActorPtr selected ) {
		if (m_pSelectedActor != NullActorPtr)
			m_pSelectedActor->setSelected(false);
		m_pSelectedActor = selected;
		
		m_Buttons.clear();
		
		if (selected == NullActorPtr) {
			
		} else {
			selected->setSelected(true);
			m_Camera->lookAt.x = selected->getDrawPos().x;
			m_Camera->lookAt.y = selected->getDrawPos().y;
			m_Camera->lookAt.z = selected->getDrawPos().z;
			m_Camera->r = 10.0f;
			
			eyeVec.x = m_Camera->lookAt.x + m_Camera->r * cosf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
			eyeVec.y = m_Camera->lookAt.y + m_Camera->r * sinf(m_Camera->phi * DEG_TO_RAD);
			eyeVec.z = m_Camera->lookAt.z + m_Camera->r * sinf(m_Camera->theta * DEG_TO_RAD) * cosf(m_Camera->phi * DEG_TO_RAD);
			
			// Need to draw different things depending on what kind of actor it is
			
			switch (selected->getType()) {
				case LAND_CITY:
				case PORT_CITY:
				case TOWER:
					buildStructureStats( selected );
					switch (m_StructureMenuId) {
						case 0:
							buildStructureMainMenu( selected );
							break;
						case 1:
							buildStructureBuildMenu( selected );
							break;
					}
					break;
				default:
					buildUnitStats( selected );
			}
		}
		glutSetWindow(m_StatsHandle);
		glutPostRedisplay();
	}
	
	void BaseGraphicsEngine::setSelectedActors( ActorList actors ) {
		if (m_pSelectedActor.get() != NULL && m_pSelectedActor != NullActorPtr)
			m_pSelectedActor->setSelected(false);
		m_pSelectedActor = NullActorPtr;
		
		ActorList availableActors;
		
		for (ActorList::iterator it = actors.begin(); it != actors.end(); it++) {
			if ((*it)->getPlayerId() == playerID)
				availableActors.push_back(*it);
		}
		
		if (availableActors.size() == 0) {
			m_pSelectedActor = NullActorPtr;
			return;
		} else if (availableActors.size() == 1) {
			setSelectedActor(availableActors.front());
			return;
		}
		
		m_Buttons.clear();
		int size = availableActors.size();
		int numWide = (int) (0.5 + sqrt(size));
		
		int buttonWidth = (m_StatsWidth - BORDER) / numWide;
		buttonWidth -= BORDER;
		
		int buttonHeight = (m_StatsHeight - 50 - 2 * BORDER) / (numWide + 1);
		buttonHeight -= BORDER;
		
		Vector2 buttonPos( BORDER + buttonWidth / 2, BORDER + buttonHeight / 2 );
		
		buttonHeight -= (10 + BORDER);
		buttonPos.y += (10 + BORDER) / 2;

		for (ActorList::iterator it = availableActors.begin(); it != availableActors.end(); it++) {
			Button * button = new Button( (*it)->getActorId(), buttonWidth, buttonHeight, m_StatsHandle, *it, buttonPos );
			m_Buttons.push_back(button);
			buttonPos.x += BORDER + buttonWidth;
			if (buttonPos.x > m_StatsWidth) {
				buttonPos.x = BORDER + buttonWidth / 2;
				buttonPos.y += BORDER + buttonHeight;
			}
		}
	}
	
	Button * BaseGraphicsEngine::doButtonPick( int a_X, int a_Y ) {
		
		if (nextUButton->intersect(a_X, a_Y))
			return nextUButton;
			
		if (nextSButton->intersect(a_X, a_Y))
			return nextSButton;
			
		if (endTurnButton->intersect(a_X, a_Y))
			return endTurnButton;
			
		for (ButtonList::iterator it = m_Buttons.begin(); it != m_Buttons.end(); it++) {
			if ((*it)->intersect( a_X, a_Y )) {
				return *it;
			}
		}
		return NULL;
	}
	
	void BaseGraphicsEngine::buildUnitStats( ActorPtr a_Actor ) {
		UnitPtr unit = m_GameEngine->getUnitPointer(a_Actor->getActorId(), a_Actor->getMapPos());
		
		if (unit.get() == NULL)
			return;
		
		// Build the picture
		int buttonHeight = m_StatsHeight - ( 120 + 10 * BORDER );
		int buttonWidth = m_StatsWidth - (2 * BORDER );
		
		if (unit->getUnitClass() == UC_AIR) {
			buttonHeight -= (10 + BORDER);
		}
		
		Vector2 buttonPos( BORDER + buttonWidth / 2, BORDER + buttonHeight / 2 );
		
		buttonHeight -= (10 + BORDER);
		buttonPos.y += (10 + BORDER) / 2;

		int pickName = a_Actor->getActorId();
		Button * button = new Button( pickName, buttonWidth, buttonHeight, m_StatsHandle, a_Actor, buttonPos );
		button->setEnabled(false);
		m_Buttons.push_back(button);
		
		// Build the stats
		//
		// Abilities:
		// Moves: cur / max
		// Radar:
		// Health: cur / max
		// Strength:
		// Fortified: 
		
		char * aText, * mText, * rText, * hText, * sText, * fText, * fuelText;
		
		int abilities = unit->getAbilities();
		std::string abilityString("");
		if (abilities & ABILITY_SONAR) {
			abilityString.append("Sonar");
		}
		
		if (abilities & ABILITY_SHELL) {
			if (abilityString.size() > 0) {
				abilityString.append(", Shelling");
			} else {
				abilityString.append("Shelling");
			}
		}
		
		if (abilities & ABILITY_BOMB) {
			if (abilityString.size() > 0) {
				abilityString.append(", Bombing");
			} else {
				abilityString.append("Bombing");
			}
		}
		
		aText = new char[12 + abilityString.size()];
		sprintf(aText, "Abilities: %s", abilityString.c_str());
		mText = new char[13];
		sprintf(mText, "Moves: %d/%d", unit->getRemainingMoves(), unit->getMaxMoves());
		rText = new char[9];
		sprintf(rText, "Radar: %d", unit->getRadarRange());
		hText = new char[14];
		sprintf(hText, "Health: %d/%d", unit->getRemainingHitpoints(), unit->getHitpoints());
		sText = new char[13];
		sprintf(sText, "Strength: %d", unit->getAttack());
		fText = new char[15];
		sprintf(fText, "Fortified: %s", (unit->isFortified()) ? "Yes" : "No");
		
		if (unit->getUnitClass() == UC_AIR) {
			fuelText = new char[12];
			AirUnitPtr up = boost::static_pointer_cast<AirUnit>(unit);
			sprintf(fuelText, "Fuel: %d/%d", up->getFuel(), up->getMaxFuel());
		}
		
		int start = buttonPos.y + buttonHeight / 2;
		
		glRasterPos2f(BORDER, start + BORDER + 10);
		for (int i = 0; aText[i] != '\0'; i++)  {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, aText[i]);
		}
		glRasterPos2f(BORDER, start + 2 * (BORDER + 10));
		for (int i = 0; mText[i] != '\0'; i++)  {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, mText[i]);
		}
		glRasterPos2f(BORDER, start + 3 * (BORDER + 10));
		for (int i = 0; rText[i] != '\0'; i++)  {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, rText[i]);
		}
		glRasterPos2f(BORDER, start + 4 * (BORDER + 10));
		for (int i = 0; hText[i] != '\0'; i++)  {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, hText[i]);
		}
		glRasterPos2f(BORDER, start + 5 * (BORDER + 10));
		for (int i = 0; sText[i] != '\0'; i++)  {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, sText[i]);
		}
		glRasterPos2f(BORDER, start + 6 * (BORDER + 10));
		for (int i = 0; fText[i] != '\0'; i++)  {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, fText[i]);
		}
		
		if (unit->getUnitClass() == UC_AIR) {
			glRasterPos2f(BORDER, start + 7 * (BORDER + 10));
			for (int i = 0; fuelText[i] != '\0'; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, fuelText[i]);
			}
		}
	}
	
	void BaseGraphicsEngine::buildStructureStats( ActorPtr a_Actor ) {
		StructurePtr structure = m_GameEngine->getHexagon(a_Actor->getMapPos())->getStructure();
		
		int buttonHeight = (m_StatsHeight - 4 * BORDER - 20) / 3;
		int buttonWidth = m_StatsWidth - 2 * BORDER;
		
		Vector2 buttonPos(BORDER + buttonWidth / 2, BORDER + buttonHeight / 2);
		
		buttonHeight -= (10 + BORDER);
		buttonPos.y += (10 + BORDER) / 2;
		
		int pickName = a_Actor->getActorId();
		Button * button = new Button( pickName, buttonWidth, buttonHeight, m_StatsHandle, a_Actor, buttonPos );
		button->setEnabled(false);
		m_Buttons.push_back(button);
	}
	
	void BaseGraphicsEngine::buildStructureMainMenu( ActorPtr a_Actor ) {
		StructureActorPtr a_Structure = boost::static_pointer_cast<StructureActor>(a_Actor);
		int buttonWidth = (m_StatsWidth - 2 * BORDER);
		int buttonHeight = 25;
		
		Vector2 position(BORDER + buttonWidth / 2, (m_StatsHeight - 4 * BORDER - 20) / 3 + 2 * BORDER + buttonHeight / 2);
		Button * button = new Button( BUTTON_STRUCTURE_BUILD_MENU, buttonWidth, buttonHeight, m_StatsHandle, m_DrawInfo, position );
		button->setEnabled(true);
		m_Buttons.push_back( button );
		
		const char * text = "Build Unit\0";
		int length = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *) text);
		
		glColor3f(0.0f, 0.0f, 0.0f);
		
		glRasterPos2f(position.x - length / 2, position.y + 3);
		for (int i = 0; text[i] != '\0'; i++)  {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
		}
		
		// Get the pending productions
		StructurePtr structure = m_GameEngine->getHexagon(a_Actor->getMapPos())->getStructure();
		std::list<ProductionRequestPtr> requests = structure->getProduction().getPendingProductions();
		
		buttonWidth = 25;
		buttonHeight = 25;
		
		Vector2 bPos(BORDER + buttonWidth / 2, position.y + BORDER + 25);
		int count = requests.size();
		int max = (m_StatsHeight - bPos.y - 15 - 50 - 2 * BORDER) / 25;
		count = (count > max) ? max : count;
		std::list<ProductionRequestPtr>::iterator it = requests.begin();
		for (int i = 0; i < count; i++, it++) {
			Button * b = new Button( (*it)->getUnitId(), buttonWidth, buttonHeight, m_StatsHandle, m_ActorManager->getActor(-1 * (int) (*it)->getObjectType()), bPos);
			m_Buttons.push_back(b);
			bPos.y += 25 + BORDER;
		}
	}
		
	void BaseGraphicsEngine::buildStructureBuildMenu( ActorPtr a_Actor ) {
		StructureActorPtr a_Structure = boost::static_pointer_cast<StructureActor>(a_Actor);
		
		ProductionCapabilities caps = a_Structure->getBuildOptions();

		int buttonWidth = (m_StatsWidth - 2 * BORDER) / 5;
		int buttonHeight = 50;
		
		Vector2 position(BORDER + buttonWidth / 2, 1.5 * (m_StatsHeight - 4 * BORDER - 20) / 3);
		Vector3 drawPos( 0.0f, -6.0f, 0.0f );
		Vector2 mapPos( -1, -1 );
		
		for (ProductionCapabilities::iterator it = caps.begin(); it != caps.end(); it++) {
			ActorPtr ptr = m_ActorManager->getActor( -1 * (int) (*it));
			if (ptr == NullActorPtr) {
				UnitActor * unit = new UnitActor( -1,  -1 * (int) (*it), (*it) );
				UnitActorPtr uaPtr( unit );
				
				// Set the color and position of the unit
				
				uaPtr->initialize( mapPos, drawPos );
				
				// Now register the new actor to the managers
				m_ActorManager->addActor(uaPtr);
				ptr = m_ActorManager->getActor(-1 * (int) (*it));
			}
			
			Button * button = new Button( ptr->getActorId(), buttonWidth, buttonHeight, m_StatsHandle, ptr, position );
			m_Buttons.push_back( button );
			position.x += buttonWidth;
			if (position.x > m_StatsWidth) {
				position.x = BORDER + buttonWidth / 2;
				position.y += buttonHeight;
			}
		}
	}
	
	void BaseGraphicsEngine::statsKeyboard( unsigned char a_Key, int a_X, int a_Y ) {
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
			case 'o':
				m_OpenMode = !m_OpenMode;
				m_ActorManager->setOpenMode(m_OpenMode);
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
		}
		
		if (m_pSelectedActor == NullActorPtr)
			m_SelectionMode = SM_NONE;
	}
}