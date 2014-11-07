/*
 *  ParticleTest.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "GraphicsCommon.h"
#include "GraphicsUtils.h"

typedef struct {
	// Related to the visibility of the particle
	bool active;
	float life;
	float fade;
	
	// Related to the color of the particle
	float r;
	float g;
	float b;
	
	// Related to the current position of the particle
	float x;
	float y;
	float z;
	
	// Related to the velocity vector of the particle
	float i;
	float j;
	float k;
	
	// Related to gravity
	float gi;
	float gj;
	float gk;
} Particle;

void printParticle(Particle);
void initializeParticles(Particle [], int, float[3]);
void initializeParticle(Particle *, float[3]);
void drawParticle(Particle *);
void drawTexturedParticle(Particle *);
bool updateParticle(Particle *);