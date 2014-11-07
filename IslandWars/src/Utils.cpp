/*
 *  Utils.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/26/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "Utils.h"

Texture * createTexture(const char * filename) {
	
	Texture * texture = new Texture;
	
	GLuint * texId = new GLuint[1];
	
	TgaInfo * tgaInfo = tgaLoad( filename );
	
	glGenTextures(1, texId);
	
	texture->texId = texId[0];
	
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint format;
	if (tgaInfo->type == 2) {
		if (tgaInfo->pixelDepth == 24) {
			format = GL_RGB;
		} else if (tgaInfo->pixelDepth == 32) {
			format = GL_RGBA;
		}
	} else if (tgaInfo->type == 3) {
		format = GL_LUMINANCE;
	}else{
		format = GL_LUMINANCE;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, tgaInfo->width, tgaInfo->height, 0, format, GL_UNSIGNED_BYTE, tgaInfo->imageData);
	
	texture->status = tgaInfo->status;
	
	//delete tgaInfo;
	
	return texture;
}