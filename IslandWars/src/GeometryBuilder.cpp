/*
 *  GeometryBuilder.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsCommon.h"
#include "GeometryBuilder.h"

//M_PI (or just PI)is not(!) part of the standard C (or C++) math.h library in fact
// It is for mine :(
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Generate the vertex coords
float coords[7][2];
bool coordsInitialized = false;

void buildHexagon(float radius, Texture texture) {
	if (!coordsInitialized) {
		for (int i = 0; i < 7; i++) {
			coords[i][0] = radius * cos(M_PI / 3.0f * i);
			coords[i][1] = radius * sin(M_PI / 3.0f * i);
		}
		coordsInitialized = true;
	}
	
	glBindTexture(GL_TEXTURE_2D, texture.texID);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	// Geometry
	glBegin(GL_TRIANGLE_FAN);
	// Center
	glTexCoord2f(0.5f, 0.435f); 
	glVertex3f(0.0, 0.0, 0.0);
	
	// Starting from 0, 0); 
	glTexCoord2f(1.0f, 0.435f);
	glVertex3f(coords[0][0], coords[0][1], 0.0);
	
	glTexCoord2f(0.75f, 0.87f); 
	glVertex3f(coords[1][0], coords[1][1], 0.0);
	
	glTexCoord2f(0.25f, 0.87f); 
	glVertex3f(coords[2][0], coords[2][1], 0.0);
	
	glTexCoord2f(0.0f, 0.435f); 
	glVertex3f(coords[3][0], coords[3][1], 0.0);
	
	glTexCoord2f(0.25, 0.0); 
	glVertex3f(coords[4][0], coords[4][1], 0.0);
	
	glTexCoord2f(0.75, 0.0); 
	glVertex3f(coords[5][0], coords[5][1], 0.0);
	
	glTexCoord2f(1.0f, 0.435f); 
	glVertex3f(coords[6][0], coords[6][1], 0.0);
	
	glEnd();
}

void buildTexturedQuad(Texture textures[]) {
	
	glBindTexture(GL_TEXTURE_2D, textures[0].texID);
	
	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, textures[1].texID);
	glBegin(GL_QUADS);
	// Back Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, textures[2].texID);
	glBegin(GL_QUADS);
	// Top Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, textures[3].texID);
	glBegin(GL_QUADS);
	// Bottom Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, textures[4].texID);
	glBegin(GL_QUADS);
	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, textures[5].texID);
	glBegin(GL_QUADS);
	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glEnd();
}

void buildPyramid(void) {
	
	// Make a pyramid
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, 1.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(-1.0, -1.0, -1.0);
	
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, 1.0, 0.0);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(-1.0, -1.0, -1.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	
	glEnd();
}