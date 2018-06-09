#pragma once

#include <vector>
#include <hdr/glm/glm.hpp>

#define PI 3.14159265358979323846
#define PARTICLE_FADE_ON    1
#define PARTICLE_FADE_DONE  2

struct _particleMember {
	glm::vec3           position;
	float               alphaValue;
	uint                fadeOnDone;
	glm::vec3           velocity;
	float               lifetimeLeft;
};

struct _particleEmitter {
	bool        inUse;
	uint        type;
	glm::vec3   position;
	uint        texID;
	GLuint      m_particleBuffer[2];
	GLuint      m_transformFeedback[2];
	std::vector<_particleMember> particleMember;
};

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
void par_newParticle(uint type, const glm::vec3 &position, int followIndex);

// Render the particles
void par_renderParticles();