#pragma once

#include <vector>
#include <hdr/glm/glm.hpp>
#include "s_globals.h"

//#define PI 3.14159265358979323846
#define PARTICLE_FADE_ON    1
#define PARTICLE_FADE_DONE  2

struct _particleMember {
	glm::vec3           position;
	float               alphaValue;
	float               sizeValue;
	uint                fadeOnDone;
	glm::vec3           velocity;
	float               lifetimeLeft;
};

struct _particleEmitter {
	bool        inUse;
	uint        type;
	uint        followIndex;
	int         spawnCounter;
	int         entitySetID;
	glm::vec3   position;
	std::vector<_particleMember> particleMember;
};

struct _particleTexture {
	GLint           texID;
	std::string     fileName;
};

extern std::vector<_particleTexture> particleTexture;
extern std::vector<_particleEmitter> particleEmitter;

// Setup a number of particles ready to use
void par_initParticleSystem ();

// Render a billboard texture at passed in position
void par_renderBillBoard(const uint whichEmitter);

// Animate the particles
void par_processParticles(float timeDelta);

// Init the particle system
//
// Pass in what sort of particle, it's starting position, fixed or moving dependant
// on followIndex - more than 0 means attach to a moving object, otherwise fixed
int par_newParticle(uint type, const glm::vec3 &position, uint followIndex);

// Render the particles
void par_renderParticles();

// Call this function from the script to populate the texture names
void par_addParticleTexture(uint particleType, string fileName);

// Remove an emitter from use
void par_removeEmitter(int emitterIndex);