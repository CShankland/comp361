/*
 *  GeometryUtils.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/18/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef __GEOMETRY_UTILS_H__
#define __GEOMETRY_UTILS_H__

#include "GraphicsCommon.h"

namespace IslandWars
{
	#define DX_CONVERSION 1.5
	#define DY_CONVERSION 0.866
	
	int generateHexagonGridDL( int x, int y, double radius, int * texIdPtr );
	int generateHexagonDL( double radius );
	void buildHexagon( double radius, int texId = -1 );
}
#endif