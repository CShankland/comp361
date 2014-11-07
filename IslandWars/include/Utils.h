/*
 *  Utils.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/26/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once

#ifndef _GRAPHICS_UTILS_H_
#define _GRAPHICS_UTILS_H_

#include "GraphicsCommon.h"
#include "tga.h"

#include <string>
#include <map>
#include <sstream>
#include <ctime>
#include <cmath>

// Converstion from clock time to milliseconds
const static int CLOCK_TO_MILLIS = CLOCKS_PER_SEC / 1000;

// Supplementary constants
#ifndef M_PI
const static float M_PI       = 3.141592653589793f;
#endif
const static float DEG_TO_RAD = 0.017453292519943f;
const static float M_SQRT3    = 1.732050807568877f;

class Texture {
public:
	Texture() : status( 0 ), texId ( 0 ) {};
	int status;
	GLuint texId;
};

typedef std::map< std::string, Texture > TextureMap;

class Vector4 {
public:
	Vector4( float a_X, float a_Y, float a_Z, float a_W ): x( a_X ), y( a_Y ), z( a_Z ), w( a_W ) { }
	Vector4( const Vector4 & v ): x( v.x ), y( v.y ), z( v.z ), w( v.w ) { }
	Vector4( ): x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) { }

	Vector4( std::stringstream & in )
	{
		in >> x;
		in >> y;
		in >> z;
		in >> w;
	}

	void Serialize( std::stringstream & out )
	{
		out << x << " ";
		out << y << " ";
		out << z << " ";
		out << w << " ";
	}

	Vector4 cross(const Vector4 & v)
	{
		return Vector4(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x, 1.0f);
	}

	void Normalize()
	{
		float scale = 1.0f / sqrtf(x * x + y * y + z * z);
		x *= scale;
		y *= scale;
		z *= scale;
	}

	float x;
	float y;
	float z;
	float w;
};

static const Vector4 Y_UP(0.0, 1.0, 0.0, 0.0);

class Vector3 {
public:
	Vector3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {};
	Vector3( float a_X, float a_Y, float a_Z ) : x( a_X ), y( a_Y ), z( a_Z ) {};
	Vector3( const Vector3 & a_Vec ) : x( a_Vec.x ), y( a_Vec.y ), z( a_Vec.z ) {};

	float x, y, z;
};

typedef Vector3 Color;

class Vector2 {

    friend Vector2 operator+(const Vector2& v1, const Vector2& v2){
        Vector2 v3;
        v3.x = v1.x + v2.x;
        v3.y = v1.y + v2.y;
        return v3;
    }

public:
	Vector2() : x( 0 ), y( 0 ) { }
	Vector2( int a_X, int a_Y ) : x( a_X ), y( a_Y ) { }
	Vector2( const Vector2 & v ) : x( v.x ), y( v.y ) { }

	Vector2( std::stringstream & in )
	{
		in >> x;
		in >> y;
	}

	void serialize( std::stringstream & out )
	{
		out << x << " ";
		out << y << " ";
	}

	int x, y;
};

Texture * createTexture(const char * filename);

#endif
