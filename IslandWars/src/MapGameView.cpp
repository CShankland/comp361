/*
 *  MapGameView.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/1/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsEngine.h"

namespace IslandWars
{
	
	void BaseGraphicsEngine::initializeMapWindow( void ) {
		// Erase the screen to black
		glClearColor(0.8f, 0.8f, 1.0f, 0.0f);
		
		// Max depth to clear
		glClearDepth(1.0f);
		
		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		
		// Type of depth testing
		glDepthFunc(GL_LEQUAL);		
		
		m_WorldManager->registerView( m_MapHandle );
	}
	
	void BaseGraphicsEngine::drawMapWindow( void ) {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glLoadIdentity();
		
		gluLookAt(0.0, 0.0, 200.0,
				  0.0, 0.0,   0.0,
				  0.0, 1.0,   0.0);
		
		glPushMatrix();
		
		glRotatef(90, 1.0f, 0.0f, 0.0f);
		glRotatef(-90 + m_Camera->theta, 0.0f, 1.0f, 0.0f);
		
		m_WorldManager->draw3D( m_MapHandle, m_DeltaMilliseconds );
		
		glPopMatrix();
		
		// Set up the box
		glColor3d(1.0, 0.0, 0.0);
		glLineWidth(2.0);
		
		float delta = 0.4f * m_Camera->r;
		glRotatef(m_Camera->theta - 90, 0.0f, 0.0f, 1.0f);
		glRotatef(180, 1.0f, 0.0f, 0.0f);
		
		float x1 = m_Camera->lookAt.x - delta * 1.1, x2 = m_Camera->lookAt.x + delta * 1.1;
		float z1 = m_Camera->lookAt.z - delta * 1.1, z2 = m_Camera->lookAt.z + delta * 1.1;
		
		glBegin(GL_LINES);
		
		glVertex3f(x1, z1, 0.0f);
		glVertex3f(x2, z1, 0.0f);
		
		glVertex3f(x2, z1, 0.0f);
		glVertex3f(x2, z2, 0.0f);
		
		glVertex3f(x2, z2, 0.0f);
		glVertex3f(x1, z2, 0.0f);
		
		glVertex3f(x1, z2, 0.0f);
		glVertex3f(x1, z1, 0.0f);
		
		glEnd();
		
		glutSwapBuffers();
		glutPostRedisplay();
	}
	
	void BaseGraphicsEngine::reshapeMapWindow( int a_Width, int a_Height ) {
		if (a_Height == 0)
			a_Height = 1;
		
		glViewport(0, 0, a_Width, a_Height);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		// Set up the projection to be a 45 degree yfovy
		gluPerspective(45.0, (double) a_Width / (double) a_Height, 0.1, 300.0);
		
		glMatrixMode(GL_MODELVIEW);
	}
}