/*
 *  Actor.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/27/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "Actor.h"

namespace IslandWars {
	
	// --------------------------------------------------
	// Actor implementation
	// --------------------------------------------------
	Actor::Actor( int a_PlayerId, int a_ActorId, ObjectType a_ObjectType ) :
		m_PlayerId( a_PlayerId ), m_ActorId( a_ActorId ), m_Type( a_ObjectType ),
		m_Visible( false ), m_Selected( false )
	{
		m_Color = getColor(a_PlayerId);
	}
	
	Actor::~Actor() {
		
	}
	
	void Actor::draw2D( int a_ViewId, int a_DeltaMilliseconds ) {
		//glColor3f(m_Color.x, m_Color.y, m_Color.z);
		glCallList(m_DrawTable[a_ViewId]->dl2D);
	}
	
	void Actor::draw3D( int a_ViewId, int a_DeltaMilliseconds ) {
		glPushMatrix();
		glTranslatef(m_DrawPosition.x, m_DrawPosition.y, m_DrawPosition.z);
		float emiss[4] = {m_Color.x, m_Color.y, m_Color.z, 0.3f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emiss);
		glCallList(m_DrawTable[a_ViewId]->dl3D);
		glPopMatrix();
	}
	
	void Actor::attachView( int viewId, DrawData * a_DrawTable ) {
		m_DrawTable[viewId] = a_DrawTable;
	}
			
	void Actor::initialize( Vector2 & a_MapPos, Vector3 & a_DrawPosition ) {
		m_MapPos = a_MapPos;
		m_DrawPosition = a_DrawPosition;
	}
	
	Color Actor::getColor( int a_PlayerId ) {
		switch (a_PlayerId) {
			case 1:
				return Color(0.2f, 0.0f, 0.0f);
			case 2:
				return Color(0.5f, 0.8f, 0.5f);
			case 3:
				return Color(0.0f, 0.0f, 0.2f);
			case 4:
				return Color(0.2f, 0.2f, 0.0f);
			case 5:
				return Color(0.0f, 0.2f, 0.2f);
			case 6:
				return Color(0.2f, 0.0f, 0.2f);
			default:
				return Color(0.0f, 0.0f, 0.0f);
		}
	}
	
	// --------------------------------------------------
	// UnitActor implementation
	// --------------------------------------------------
	UnitActor::UnitActor( int a_PlayerId, int a_ActorId, ObjectType a_ObjectType ) :
		Actor( a_PlayerId, a_ActorId, a_ObjectType ),
		m_Fortified( false )
	{
	}
	
	UnitActor::~UnitActor() {
		
	}
	
	// --------------------------------------------------
	// StructureActor implementation
	// --------------------------------------------------
	StructureActor::StructureActor( ProductionCapabilities a_BuildOptions, int a_PlayerId, int a_ActorId, ObjectType a_ObjectType ) :
		Actor( a_PlayerId, a_ActorId, a_ObjectType ),
		m_BuildOptions( a_BuildOptions )
	{
	}
	
	StructureActor::~StructureActor() {
		
	}
	
}; // Namespace IslandWars
