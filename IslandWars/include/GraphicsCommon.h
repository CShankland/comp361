/*
 *  GraphicsCommon.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *	Edited by Guillaume on 24/01/10
 */

#pragma once

#ifndef __GRAPHICS_COMMON_H__
#define __GRAPHICS_COMMON_H__

// Let SDL deal with including the right things for OpenGL.
//#include "SDL.h"
//#include "SDL_opengl.h"

// Need GLUT which brings in all the other OpenGL headers
#include <GLUT/glut.h>

#include <cmath>

#define BORDER 20
#define WINDOW_POSITION_X 0
#define WINDOW_POSITION_Y 0
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MENU_HEIGHT 50
#define MENU_WIDTH 80

#endif
