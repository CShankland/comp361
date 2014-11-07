/*
 *  GraphicsUtils.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsUtils.h"
#include <cstring>

void printString(float x, float y, const char *s) {
	int len, i;
	
	glRasterPos2d(x, y);
	
	len = (int) strlen(s);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
	}
}