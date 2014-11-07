/*
 *  UIButton.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/11/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "UIElements.h"

namespace IslandWars
{
	
	Button::Button( int pickName, int width, int height, int viewId, DrawablePtr a_Drawable, Vector2 & position ) 
		: m_PickName( pickName), m_Drawable( a_Drawable ), m_ViewId( viewId ), m_Selected( false ), m_Enabled( true )
	{
		m_Position.x = position.x;
		m_Position.y = position.y;
		
		m_Width = width;
		m_Height = height;
		
		float b[8][3] = {
			{ -width / 2.0f,               -height / 2.0f,               0.0f },
			{ -width / 2.0f + BORDER_SIZE, -height / 2.0f,               0.0f },
			{  width / 2.0f,               -height / 2.0f,               0.0f },
			{  width / 2.0f,               -height / 2.0f + BORDER_SIZE, 0.0f },
			{  width / 2.0f,                height / 2.0f,               0.0f },
			{  width / 2.0f - BORDER_SIZE,  height / 2.0f,               0.0f },
			{ -width / 2.0f,                height / 2.0f,               0.0f },
			{ -width / 2.0f,                height / 2.0f - BORDER_SIZE, 0.0f }
		};

		float c[4][3] = {
			{ -width / 2.0f + BORDER_SIZE, -height / 2.0f + BORDER_SIZE, 0.0f },
			{  width / 2.0f - BORDER_SIZE, -height / 2.0f + BORDER_SIZE, 0.0f },
			{  width / 2.0f - BORDER_SIZE,  height / 2.0f - BORDER_SIZE, 0.0f },
			{ -width / 2.0f + BORDER_SIZE,  height / 2.0f - BORDER_SIZE, 0.0f }
		};
		
		glutSetWindow( viewId );
		
		int dlIndex = glGenLists( 2 );
		
		pressedDl = dlIndex;
		unPressedDl = dlIndex + 1;
		
		// Build the selection border
		glNewList( pressedDl, GL_COMPILE );
		
		glColor3f( 0.2f, 0.2f, 0.2f );
		
		glBegin(GL_TRIANGLES);
		
		glVertex3fv(b[0]);
		glVertex3fv(b[2]);
		glVertex3fv(c[0]);
		
		glVertex3fv(c[0]);
		glVertex3fv(b[2]);
		glVertex3fv(c[1]);
		
		glVertex3fv(b[0]);
		glVertex3fv(c[0]);
		glVertex3fv(b[6]);
		
		glVertex3fv(b[6]);
		glVertex3fv(c[0]);
		glVertex3fv(c[3]);
		
		glColor3f( 0.8f, 0.8f, 0.8f );
		
		glVertex3fv(b[6]);
		glVertex3fv(c[3]);
		glVertex3fv(c[2]);
		
		glVertex3fv(b[6]);
		glVertex3fv(c[2]);
		glVertex3fv(b[4]);
		
		glVertex3fv(b[4]);
		glVertex3fv(c[2]);
		glVertex3fv(b[2]);
		
		glVertex3fv(c[2]);
		glVertex3fv(c[1]);
		glVertex3fv(b[2]);
		
		glEnd();
		
		glEndList();
		
		// Build the selection border
		glNewList( unPressedDl, GL_COMPILE );
		
		glColor3f( 0.8f, 0.8f, 0.8f );
		
		glBegin(GL_TRIANGLES);
		
		glVertex3fv(b[0]);
		glVertex3fv(b[2]);
		glVertex3fv(c[0]);
		
		glVertex3fv(c[0]);
		glVertex3fv(b[2]);
		glVertex3fv(c[1]);
		
		glVertex3fv(b[0]);
		glVertex3fv(c[0]);
		glVertex3fv(b[6]);
		
		glVertex3fv(b[6]);
		glVertex3fv(c[0]);
		glVertex3fv(c[3]);
		
		glColor3f( 0.2f, 0.2f, 0.2f );
		
		glVertex3fv(b[6]);
		glVertex3fv(c[3]);
		glVertex3fv(c[2]);
		
		glVertex3fv(b[6]);
		glVertex3fv(c[2]);
		glVertex3fv(b[4]);
		
		glVertex3fv(b[4]);
		glVertex3fv(c[2]);
		glVertex3fv(b[2]);
		
		glVertex3fv(c[2]);
		glVertex3fv(c[1]);
		glVertex3fv(b[2]);
		
		glEnd();
		
		glEndList();
		
		m_Scale.x = width - BORDER_SIZE * 2;
		m_Scale.y = height - BORDER_SIZE * 2;
	}
	
	Button::~Button()
	{
	}
	
	void Button::draw() {
		glPushMatrix();
		glPushName(m_PickName);
		glTranslatef(m_Position.x, m_Position.y, 0.0f);
		
		if (m_Selected && m_Enabled) {
			glCallList(pressedDl);
			glColor3f(0.5f, 0.5f, 0.5f);
		} else {
			glCallList(unPressedDl);
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		
		glScalef(m_Scale.x, -m_Scale.y, 1.0f);
		glEnable(GL_TEXTURE_2D);
		m_Drawable->draw2D( m_ViewId, 0 );
		glDisable(GL_TEXTURE_2D);
		glPopName();
		glPopMatrix();
	}

	bool Button::intersect(int x, int y)
	{
		int tX = x - m_Position.x;
		int tY = y - m_Position.y;
		
		tX = (tX < 0) ? -1 * tX : tX;
		tY = (tY < 0) ? -1 * tY : tY;
		
		if ( tX < m_Width * 0.5f && tY < m_Height * 0.5f) {
			return true;
		}
		
		return false;
	}
	
	TextureDrawable::TextureDrawable() {
		
	}
	
	TextureDrawable::~TextureDrawable() {
		
	}
	
	void TextureDrawable::draw2D( int a_ViewId, int a_DeltaMilliseconds ) {
		glCallList(m_DrawTable[a_ViewId]->dl2D);
	}
	
	void TextureDrawable::draw3D( int a_ViewId, int a_DeltaMilliseconds ) {
		glCallList(m_DrawTable[a_ViewId]->dl3D);	
	}
	
	void TextureDrawable::attachView( int a_ViewId, DrawData * a_DrawData ) {
		m_DrawTable[a_ViewId] = a_DrawData;
	}
}