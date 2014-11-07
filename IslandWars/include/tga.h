/*
 *  tga.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/28/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __TGA_H__
#define __TGA_H__

#include <cstdlib>

//**************//
// Status Codes //
//**************//
// This is what we want
#define TGA_OK 0

// When we're presented with a compressed file 
#define TGA_ERROR_COMPRESSED_FILE 1

#define TGA_ERROR_MEMORY 2

// When we're presented with a color indexed file
#define TGA_ERROR_INDEXED_COLOR 3

#define TGA_ERROR_READING_FILE 4
#define TGA_ERROR_FILE_OPEN 5

class TgaInfo {
public:
	
	~TgaInfo() { 
		if (imageData)
			free(imageData);
	}
	
	int status;
	unsigned char type, pixelDepth;
	short int width, height;
	unsigned char * imageData;
};


TgaInfo * tgaLoad(const char *filename);
void tgaRGBtogreyscale(TgaInfo *info);

int tgaGrabScreenSeries(char *filename, int xmin,int ymin, int xmax, int ymax);
int tgaSave(char *filename, short int width, short int height, unsigned char pixelDepth, unsigned char *imageData);
int tgaSaveSeries(char *filename, short int width, short int height, unsigned char pixelDepth, unsigned char *imageData);

#endif