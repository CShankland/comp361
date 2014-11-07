/*
 *  GraphicsTest.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GraphicsTest.h"

#include <queue>

#define kWindowHeight 800
#define kWindowWidth 1200
#define MINI_MAP_HEIGHT 200
#define STATISTICS_HEIGHT 600

#define RADIUS 0.5f
#define D_X 0.75f
#define D_Y 0.433f

#define MAX_PARTICLES 25000

// Methods

// Called before the OpenGL context is live
void InitGL(void);

// Called every time the window needs to repaint itself
void DrawGLScene(void);

// Called when the window containing the OpenGL context changes size
void ReSizeGLScene(int width, int height);

// Responds to mouse clicks
void mouse(int button, int state, int x, int y);

// Responds to mouse drags
void mouse_moved(int x, int y);

// Simple framerate calculator
void calcFramerate(int x);

// Handles keyboard input
void keys(unsigned char, int, int);

// Builds the game world out of hexagons
void buildHexGrid(int, int, int []);

// Variables

// Window number for the main window
int gMainWindow = 0;

// Framerate counter, and the fps for the past second
int frameticks, fps;

// Positioning for the geometry
float fps_x, fps_y, fps_z;

// Old position (i.e. since the mouse moved)
float prev_x, prev_y;

// Textures to be used
Texture textures[6]; 

// Builds the world, each value represents an index
// into the texture array.
extern int texCoords[160];

// Light test
float ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float diffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };

bool lightSwitch = true;

// Particle test
Texture particleTexture;
Particle particles[MAX_PARTICLES];

bool showParticles = false;
float particle_pos[3];

int grid_x = 0, grid_y = 0;

int rot = -80;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(kWindowWidth, kWindowHeight);
	glutInitWindowPosition(100, 100);
	gMainWindow = glutCreateWindow("Test");
	glutReshapeFunc(ReSizeGLScene);
	glutDisplayFunc(DrawGLScene);
	
	InitGL();
	
	glutTimerFunc(1000, calcFramerate, 0);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_moved);
	glutKeyboardFunc(keys);
	
	glutMainLoop();
	
	return 0;
}

void keys(unsigned char key, int x, int y) {
	switch (key) {
		case 'j':
			fps_x -= 0.25f;
			break;
		case 'l':
			fps_x += 0.25f;
			break;
		case 'i':
			fps_y += 0.25f;
			break;
		case 'k':
			fps_y -= 0.25f;
			break;
		case 'p':
			if (lightSwitch) {
				glEnable(GL_LIGHTING);
				lightSwitch = false;
			} else {
				glDisable(GL_LIGHTING);
				lightSwitch = true;
			}
			break;
		case 'a':
			grid_x--;
			grid_x = (grid_x < 0) ? 0 : grid_x;
			break;
		case 'd':
			grid_x++;
			grid_x = (grid_x > 15) ? 15 : grid_x;
			break;
		case 's':
			grid_y++;
			grid_y = (grid_y > 9) ? 9 : grid_y;
			break;
		case 'w':
			grid_y--;
			grid_y = (grid_y < 0) ? 0 : grid_y;
			break;
		case 'z':
			rot -= 5;
			break;
		case 'x':
			rot += 5;
			break;
		default:
			break;
	}
}

void calcFramerate(int x) {
	fps = frameticks;
	frameticks = 0;
	glutTimerFunc(1000, calcFramerate, 0);
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		
		int viewport[4];
		double modelview[16];
		double projection[16];
		
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		
		double objX, objY, objZ;
		
		float winZ;
		
		y = viewport[3] - y;
		
		//glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
				
		winZ = 1.0f;
		
		gluUnProject(x, y, winZ, modelview, projection, viewport, &objX, &objY, &objZ);
		
		particle_pos[0] = (float)objX;
		particle_pos[1] = (float)objY;
		particle_pos[2] = (float)objZ;
		
		showParticles = true;
		
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		showParticles = false;
	}
}

void mouse_moved(int x, int y) {
	
	glLoadIdentity();
	
	int viewport[4];
	double modelview[16];
	double projection[16];
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	
	double objX, objY, objZ;
	float winZ;
	
	y = viewport[3] - y;
	
	//glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	
	winZ = 1.0f;
	
	gluUnProject(x, y, winZ, modelview, projection, viewport, &objX, &objY, &objZ);
	
	particle_pos[0] = (float)objX;
	particle_pos[1] = (float)objY;
	particle_pos[2] = (float)objZ;
	
	glutPostRedisplay();
}

void InitGL(void) {
	
	// Load some textures
	if (!loadTGA(&textures[0], "Resources/HexTex_128_deep.tga"))
		printf("Error loading texture");
		
	if (!loadTGA(&textures[1], "Resources/HexTex_128_shallow.tga"))
		printf("Error loading texture");
	
	if (!loadTGA(&textures[2], "Resources/HexTex_128_beach.tga"))
		printf("Error loading texture");
	
	if (!loadTGA(&textures[3], "Resources/HexTex_128_forest.tga"))
		printf("Error loading texture");
	
	if (!loadTGA(&textures[4], "Resources/ParticleTex.tga"))
		printf("Error loading particle texture");
	
	if (!loadTGA(&textures[5], "Resources/airplane_256.tga"))
		printf("Error loading airplane texture");
	
	// Define some default values for positioning
	fps_x = -3;
	fps_y = -1;
	fps_z = -6;
	
	// Erase the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Use a smooth shading model
	glShadeModel(GL_SMOOTH);
	
	// Max depth to clear
	glClearDepth(1.0f);
	
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	
	// Type of depth testing
	glDepthFunc(GL_LEQUAL);
	
	// Define vertex winding
	glFrontFace(GL_CCW);
	
	// Lighting
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseColor);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	
	// Enable the light
	glEnable(GL_LIGHT1);
	
	glEnable(GL_BLEND);							// Enable Blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Type Of Blending To Perform
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);			// Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);	
	
	// Enable material coloring... so that color affects
	// the material's color.
	glEnable(GL_COLOR_MATERIAL);
}

void DrawGLScene(void) {
		
	frameticks++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	glBindTexture(GL_TEXTURE_2D, textures[4].texID);
	glEnable(GL_TEXTURE_2D);
	
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (particles[i].active) {
			drawTexturedParticle(&particles[i]);
			updateParticle(&particles[i]);
		}
	}
		
	if (showParticles) {
		for (int i = 0; i < MAX_PARTICLES; i++) {
			if (!particles[i].active)
				initializeParticle(&particles[i], particle_pos);
		}
	}
	
	glDisable(GL_TEXTURE_2D);
	
	glLoadIdentity();
	glTranslatef(fps_x, fps_y, -10.0f);
	buildHexGrid(16, 10, texCoords);
	
	glLoadIdentity();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(-3.8f, 2.4f, fps_z);
	
	char outString[256] = "";
	sprintf(outString, "FPS: %d", fps);
	printString(0, 0, outString);
	
	glutSwapBuffers();
	
	glutPostRedisplay();
}

void ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set up the projection to be a 45 degree yfovy
	gluPerspective(45.0, (GLfloat) width / (GLfloat) height, 0.1f, 200.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void buildHexGrid(int x, int y, int texNum[]) {
	glRotatef((float)rot, 1.0f, 0.0f, 0.0f);
	glPushMatrix();
	
	glEnable(GL_TEXTURE_2D);
	
	
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			glTranslated(0.0f, -2 * D_Y, 0.0f);
			buildHexagon(RADIUS, textures[texNum[i*y + j]]);
			
			if (i == grid_x && j == grid_y) {
				glBindTexture(GL_TEXTURE_2D, textures[5].texID);
			
				glDisable(GL_BLEND);
				
				glBegin(GL_QUADS);
				// Front Face
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-RADIUS / 4.0f, 0,  RADIUS / 2.0f);	// Bottom Left Of The Texture and Quad
				glTexCoord2f(1.0f, 1.0f); glVertex3f( RADIUS / 4.0f, 0,  RADIUS / 2.0f);	// Bottom Right Of The Texture and Quad
				glTexCoord2f(1.0f, 0.0f); glVertex3f( RADIUS / 4.0f,  0,  0);	// Top Right Of The Texture and Quad
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-RADIUS / 4.0f,  0,  0);	// Top Left Of The Texture and Quad
				glEnd();
				
				glEnable(GL_BLEND);
			}
		}
		glTranslatef(D_X, y * 2 * D_Y, 0.0f);
		if (i % 2 == 0)
			glTranslatef(0.0f, -D_Y, 0.0f);
		else
			glTranslatef(0.0f, D_Y, 0.0f);
	}
	
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	
	glTranslatef(D_X * 5, -D_Y * 5, 0.0f);
	
	
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}