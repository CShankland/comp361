/*
 *  UIElements.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/11/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __I_UIELEMENT_H__
#define __I_UIELEMENT_H__

#include "GraphicsCommon.h"
#include "Utils.h"
#include <list>
#include <vector>
#include "Actor.h"

namespace IslandWars
{	
	
	class Button
	{
	public:
		explicit Button( int pickName, int width, int height, int viewId, DrawablePtr a_Drawable, Vector2 & position );
		virtual ~Button();
		
		void draw();
		void setSelected( bool selected ) { m_Selected = selected; }
		void setEnabled( bool enabled ) { m_Enabled = enabled; }
		bool isSelected() { return m_Selected; }
		bool isEnabled() { return m_Enabled; }
		int getHeight() { return m_Height; }
		int getWidth() { return m_Width; }
		int getName() { return m_PickName; }
		
		bool intersect(int x, int y);
		
		void setPosition(Vector2 & pos) { m_Position = pos; }
		Vector2 & getPosition() { return m_Position; }
		
	protected:
		
		static const float BORDER_SIZE = 5.0f;
		
		bool m_Selected, m_Enabled;
		int m_PickName;
		Vector2 m_Position;
		int m_Width, m_Height;
		int pressedDl, unPressedDl, m_ViewId;
		DrawablePtr m_Drawable;
		Vector2 m_Scale;
	};
	
	typedef std::list<Button *> ButtonList;
	
	class TextureDrawable : public Drawable {
	public:
		explicit TextureDrawable();
		virtual ~TextureDrawable();
		
		void draw2D( int a_ViewId, int a_DeltaMilliseconds );
		void draw3D( int a_ViewId, int a_DeltaMilliseconds );
		
		void attachView( int a_ViewId, DrawData * a_DrawData );
	private:
		DrawTable m_DrawTable;
	};
}
#endif