/*
 *  TextureTest.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsCommon.h"

#ifndef _TEXTURE_TEST_H_
#define _TEXTURE_TEST_H_

#include <cstdio>

typedef struct {
	GLubyte *imageData;
	GLuint bpp;
	GLuint width;
	GLuint height;
	GLuint texID;
	GLuint type;
} Texture;

typedef struct {
	GLubyte Header[12];
} TGAHeader;

typedef struct {
	// Header information
	GLubyte header[6];
	GLuint bytesPerPixel;
	GLuint imageSize;
	GLuint type;
	GLuint height;
	GLuint width;
	GLuint bpp;
} TGA;

bool loadUncompressedTGA(Texture *, const char *, FILE *);
bool loadCompressedTGA(Texture *, const char *, FILE *);

bool loadTGA(Texture *, const char *);
#endif