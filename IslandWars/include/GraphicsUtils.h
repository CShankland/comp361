/*
 *  GraphicsUtils.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsCommon.h"

#ifndef _GRAPHICS_UTILS_
#define _GRAPHICS_UTILS_

static float rainbow[12][3] =
{
	{ 1.0f, 0.5f, 0.5f },
	{ 1.0f, 0.75f, 0.5f },
	{ 1.0f, 1.0f, 0.5f },
	{ 0.75f, 1.0f, 0.5f },
	{ 0.5f, 1.0f, 0.5f },
	{ 0.5f, 1.0f, 0.75f },
	{ 0.5f, 1.0f, 1.0f },
	{ 0.5f, 0.75f, 1.0f },
	{ 0.5f, 0.5f, 1.0f },
	{ 0.75f, 0.5f, 1.0f },
	{ 1.0f, 0.5f, 1.0f },
	{ 1.0f, 0.5f, 0.75f }
};

void printString(float, float, const char *);

#endif