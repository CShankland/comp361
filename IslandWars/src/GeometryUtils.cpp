/*
 *  GeometryUtils.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/18/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GeometryUtils.h"
#include <iostream>
#include "Actor.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace IslandWars
{
	
	// Drawn in the XZ plane
	int generateHexagonGridDL( int x, int y, double radius, int * texIdPtr )
	{
		// Width
		double l_Width = radius * ( 2 * x - (x / 2) );
		
		// Height
		double l_Height = (y + 0.5) * radius * M_SQRT3;
		
		// Upper left corner
		double l_X0 = - l_Width / 2.0;
		double l_Z0 = - l_Height / 2.0;
		
		double d_z = 0.5 * radius * M_SQRT3;
		double d_x = radius * 1.5;
				
		int dlId = glGenLists( 1 );
				
		glNewList(dlId, GL_COMPILE);
		
		glColor3d(1.0, 1.0, 1.0);
				
		glEnable(GL_TEXTURE_2D);
		
		glTranslated(l_X0 + radius, 0.0, l_Z0 + 0.5 * radius * M_SQRT3);
		
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				buildHexagon(radius, *texIdPtr);
				texIdPtr++;
				glTranslated(0.0, 0.0, 2 * d_z);
			}
			
			glTranslated( d_x, 0.0, -d_z * y * 2);
			
			if (i % 2 == 1)
				glTranslated(0.0, 0.0, d_z);
			else
				glTranslated(0.0, 0.0, -d_z);
		}
				
		glDisable(GL_TEXTURE_2D);
		
		glEndList();
		
		return dlId;
	}
	
	int generateHexagonDL( double radius )
	{
		int dlId = glGenLists( 1 );
		
		glNewList(dlId, GL_COMPILE);
						
		buildHexagon(radius);
				
		glEndList();
		
		return dlId;
	}
	
	// Generate the vertex coords
	double coords[7][2];
	bool coordsInitialized;
	
	void buildHexagon(double radius, int texId ) {
		if (!coordsInitialized) {
			for (int i = 0; i < 7; i++) {
				coords[i][0] = radius * cos(M_PI / 3.0 * i);
				coords[i][1] = radius * sin(M_PI / 3.0 * i);
			}
			coordsInitialized = true;
		}
		
		if (texId >= 0) 
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texId);
		}
				
		// Geometry
		glBegin(GL_TRIANGLE_FAN);
		// Center
		glTexCoord2d(0.5, 0.435); 
		glVertex3d(0.0, 0.0, 0.0);
		
		// Starting from 0, 0); 
		glTexCoord2d(1.0, 0.435);
		glVertex3d(coords[0][0], 0.0, coords[0][1]);
		
		glTexCoord2d(0.75, 0.87); 
		glVertex3d(coords[1][0], 0.0, coords[1][1]);
		
		glTexCoord2d(0.25, 0.87); 
		glVertex3d(coords[2][0], 0.0, coords[2][1]);
		
		glTexCoord2d(0.0, 0.435); 
		glVertex3d(coords[3][0], 0.0, coords[3][1]);
		
		glTexCoord2d(0.25, 0.0); 
		glVertex3d(coords[4][0], 0.0, coords[4][1]);
		
		glTexCoord2d(0.75, 0.0); 
		glVertex3d(coords[5][0], 0.0, coords[5][1]);
		
		glTexCoord2d(1.0, 0.435); 
		glVertex3d(coords[6][0], 0.0, coords[6][1]);
		
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
	}
	
}