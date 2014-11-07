/*
 *  GameTile.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/26/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */


#include "Actor.h"
#include "tga.h"
#include "GraphicsCommon.h"
#include "GeometryUtils.h"
#include "GameEngine.h"

#include <iostream>
#include <algorithm>

namespace IslandWars
{
	
	GameTile::GameTile( Vector2 a_Position, Terrain a_Type ) : 
		m_Position( a_Position ), m_Type( a_Type ), m_Visibility( VISIBILITY_NONE )
	{ 
	}
	
	GameTile::~GameTile()
	{
		for (DrawTable::iterator it = m_DrawTable.begin(); it != m_DrawTable.end(); it++) {
			glutSetWindow(it->first);
			glDeleteLists(it->second->dl2D, 1);
			glDeleteLists(it->second->dl3D, 1);
		}
	}
	
	void GameTile::initialize( Vector3 & a_Position, float * a_HeightMap ) {
		m_DrawPosition.x = a_Position.x;
		m_DrawPosition.y = a_Position.y;
		m_DrawPosition.z = a_Position.z;
		
		m_HeightMap = a_HeightMap;
	}
	
	void GameTile::addActor( ActorPtr a_Actor ) {
		if (m_Actors.size() == 0) {
			m_Actors.push_back(a_Actor);
			return;
		}
		if (a_Actor->getType() == TOWER || a_Actor->getType() == LAND_CITY || a_Actor->getType() == PORT_CITY) {
			m_Actors.push_front(a_Actor);
		} else if (a_Actor->getType() == FLAG) {
			m_Actors.push_back(a_Actor);
		} else {
			for (ActorList::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
				if ((*it)->getType() == TOWER || (*it)->getType() == LAND_CITY || (*it)->getType() == PORT_CITY) {
					continue;
				}
				if ((*it)->getType() == FLAG) {
					m_Actors.insert(it, a_Actor);
					return;
				}
				UnitPtr unit = GameEngine::get()->getUnitPointer((*it)->getActorId(), m_Position);
				if (unit.get() != NULL) {
					int hitpoints = unit->getRemainingHitpoints();
					if (unit->getRemainingHitpoints() < hitpoints) {
						m_Actors.insert(it, a_Actor);
						return;
					}
				}
			}
			if (m_Actors.back()->getType() == FLAG) {
				ActorPtr p = m_Actors.back();
				m_Actors.pop_back();
				m_Actors.push_back(a_Actor);
				m_Actors.push_back(p);
			} else {
				m_Actors.push_back(a_Actor);
			}
		}
	}
	
	ActorPtr GameTile::getActor( int a_Id ) {
		return NullActorPtr;
	}
	
	ActorPtr GameTile::removeActor( int a_Id ) {
		ActorPtr val = ActorPtr();
		for (ActorList::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
			if ((*it)->getActorId() == a_Id) {
				val = *it;
				m_Actors.erase(it);
				break;
			}
		}
		
		return val;
	}
	
	void GameTile::draw3D( int a_ViewId, int a_DeltaMilliseconds ) {
		glPushName(getHashCode(this));
		switch (m_Visibility) {
			case VISIBILITY_NONE:
				glDisable(GL_TEXTURE_2D);
				glPushMatrix();
				glTranslatef(m_DrawPosition.x, m_DrawPosition.y, m_DrawPosition.z);
				glColor3f(0.0f, 0.0f, 0.0f);
				glCallList(m_DrawTable[a_ViewId]->dl2D);
				glPopMatrix();
				glEnable(GL_TEXTURE_2D);
				break;
			case VISIBILITY_FOG:
				glColor3f(0.5f, 0.5f, 0.5f);
				glCallList(m_DrawTable[a_ViewId]->dl3D);
				break;
			case VISIBILITY_OPEN:
				glColor3f(1.0f, 1.0f, 1.0f);
				glCallList(m_DrawTable[a_ViewId]->dl3D);
				break;
		}
		glPopName();
	}
	
	void GameTile::draw2D( int a_ViewId, int a_DeltaMilliseconds ) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glCallList(m_DrawTable[a_ViewId]->dl2D);
	}
	
	void GameTile::attachView( int viewId, DrawData * a_DrawTable )
	{ 
		m_DrawTable[viewId] = a_DrawTable;
	}
}; // Namespace IslandWars