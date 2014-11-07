/*
 *  TextureTest.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "TextureTest.h"
#include <cstring>
#include <cstdlib>

TGAHeader tgaHeader;
TGA tga;

// Uncompressed TGA
GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
// Compressed TGA (RLE)
GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

bool loadTGA(Texture *texture, const char *filename) {
	FILE *fTGA = NULL;
	if (filename) {
		fTGA = fopen(filename, "rb");
	} else {
		printf("Error opening texture file.");
		return false;
	}
	
	if (!fTGA)
		return false;
	
	if (fread(&tgaHeader, sizeof tgaHeader, 1, fTGA) == 0)
		return false;
	
	if (memcmp(uTGAcompare, &tgaHeader, sizeof tgaHeader) == 0) {
		if (!loadUncompressedTGA(texture, filename, fTGA))
			return false;
	} else if (memcmp(cTGAcompare, &tgaHeader, sizeof tgaHeader) == 0) {
		if (!loadCompressedTGA(texture, filename, fTGA))
			return false;
	} else {
		return false;
	}
	
	glGenTextures(1, &texture->texID);
	
	glBindTexture(GL_TEXTURE_2D, texture->texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, texture->type, texture->width, texture->height, 0, texture->type, GL_UNSIGNED_BYTE, texture->imageData);
	
	return true;
}

// Not implemented yet...
bool loadCompressedTGA(Texture *texture, const char *filename, FILE *fTGA) {
	return false;
}

bool loadUncompressedTGA(Texture *texture, const char *filename, FILE *fTGA) {
	// Read the header information
	if (fread(tga.header, sizeof tga.header, 1, fTGA) == 0) {
		return false;
	}
	
	// Unpack the header information
	texture->width = tga.header[1] * 256 + tga.header[0];
	texture->height = tga.header[3] * 256 + tga.header[2];
	texture->bpp = tga.header[4];
	
	// Copy back into the local structure
	tga.width = texture->width;
	tga.height = texture->height;
	tga.bpp = texture->bpp;
	
	// Error checking
	if (tga.width <= 0 || tga.height <= 0 || (tga.bpp != 24 && tga.bpp != 32)) {
		return false;
	}
	
	if (texture->bpp == 24)
		texture->type = GL_RGB;
	else 
		texture->type = GL_RGBA;
	
	// Get the bytes per pizxel
	tga.bytesPerPixel = tga.bpp / 8;
	
	// Memory size of the image
	tga.imageSize = tga.bytesPerPixel * tga.width * tga.height;
	
	// Allocate this memory
	texture->imageData = (GLubyte *)malloc(tga.imageSize);
	// Memory was not allocated
	if (texture->imageData == NULL) {
		return false;
	}
	
	// Read the image into memory, or report an error if there is a problem
	if (fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize) {
		return false;
	}
	
	// Need to swap the byte ordering from BGR to RGB
	for (GLuint cswap = 0; cswap < (int) tga.imageSize; cswap += tga.bytesPerPixel) {
		// 1st byte XOR 3rd byte XOR 1st byte XOR 3rd byte
		GLuint temp = texture->imageData[cswap];
		texture->imageData[cswap] = texture->imageData[cswap + 2];
		texture->imageData[cswap + 2] = temp;
	}
	
	fclose(fTGA);
	return true;
}