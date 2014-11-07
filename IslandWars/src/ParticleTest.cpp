/*
 *  ParticleTest.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/21/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "ParticleTest.h"
#include <cstdlib>
#include <cstdio>

#define SLOWDOWN 1500.0f

void initializeParticles(Particle particle[], int num, float pos[3]) {
	for (int count = 0; count < num; count++) {
		initializeParticle(&particle[count], pos);
	}
}

void initializeParticle(Particle *particle, float pos[3]) {
		
	// Activate the particle, and give it full life
	particle->active = true;
	particle->life = 1.0f;
	
	// The rate that the particle will fade
	particle->fade = float(rand() % 100) / 1000 + 0.003f;
	
	// Initial color of the particle
	int rainbowIndex = rand() % 12;
	particle->r = rainbow[rainbowIndex][0];
	particle->g = rainbow[rainbowIndex][1];
	particle->b = rainbow[rainbowIndex][2];
	
	// Initial position of the particle
	particle->x = pos[0];
	particle->y = pos[1];
	particle->z = pos[2];
	
	// Set a speed for the particle
	particle->i = float (rand() % 50 - 25.0f) * 10.0f;
	particle->j = float (rand() % 50) * 10.0f;
	particle->k = float (rand() % 50 - 25.0f) * 10.0f;
	
	// Set gravity
	particle->gi = 0.0f;
	particle->gj = -1.8f;
	particle->gk = 0.0f;
}

void drawParticle(Particle *particle) {
	// Set the color
	glColor4f(particle->r, particle->g, particle->b, particle->life);
	
	// Draw the particle
}

void drawTexturedParticle(Particle *particle) {
	
	// Set the color
	glColor4f(particle->r, particle->g, particle->b, particle->life);

	// Draw the particle, with texture coords
	glBegin(GL_TRIANGLE_STRIP);
	
	glTexCoord2f(1, 1); 
	glVertex3f(particle->x + 0.25f, particle->y + 0.25f, particle->z);
	
	glTexCoord2f(0, 1); 
	glVertex3f(particle->x - 0.25f, particle->y + 0.25f, particle->z);
	
	glTexCoord2f(1, 0); 
	glVertex3f(particle->x + 0.25f, particle->y - 0.25f, particle->z);
	
	glTexCoord2f(0, 0); 
	glVertex3f(particle->x - 0.25f, particle->y - 0.25f, particle->z);
	
	glEnd();
}

bool updateParticle(Particle *particle) {
	
	// Determine if this particle will make it another iteration
	particle->life -= particle->fade;
		
	// If not, recycle it
	if (particle->life <= 0) {
		particle->active = false;
		return false;
	// Otherwise, move it
	} else {
		particle->x += particle->i / SLOWDOWN;
		particle->y += particle->j / SLOWDOWN;
		particle->z += particle->k / SLOWDOWN;
		
		particle->i += particle->gi;
		particle->j += particle->gj;
		particle->k += particle->gk;
		
		return true;
	}
}

void printParticle(Particle particle) {
	printf("***** Particle Info *****\n");
	printf("Active: %d, Life: %.2f, Fade: %.2f\n", particle.active, particle.life, particle.fade);
	printf("Color: R %.2f, G %.2f, B %.2f\n", particle.r, particle.g, particle.b);
	printf("Direction: %.2f, %.2f, %.2f\n", particle.i, particle.j, particle.k);
}