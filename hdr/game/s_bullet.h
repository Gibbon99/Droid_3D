#pragma once

#include "s_globals.h"
#include "s_bullet.h"

typedef struct
{
	bool            active;
	glm::vec3       direction;
	glm::vec3       position;
	glm::vec3       destination;
	GLfloat         speed;
} _bullet;

extern _bullet             bullet[];
extern float               bullet_1_speed;
extern int                 numActiveBullets;

// Process a bullets movement
void gam_processBulletMovement(GLfloat interpolate);

// Create new bullet
void gam_createBullet(glm::vec3 direction, glm::vec3 position, GLfloat speed);

// Draw bullet model
void gam_drawBullets(int whichShader);
