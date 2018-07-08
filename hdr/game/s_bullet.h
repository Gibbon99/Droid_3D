#pragma once

#include "s_globals.h"
#include "s_bullet.h"
#include "s_physics.h"

typedef struct
{
	bool            active;
	glm::vec3       direction;
	glm::vec3       position;
	glm::vec3       destination;
	GLfloat         speed;
	int				lightIndex;
	int				physicsIndex;
	int				collisionIndex;
	int             particleIndex;
	int				whichMesh;
	float			meshScaleFactor;
} _bullet;

extern _bullet             bullet[];
extern float               bullet_1_speed;
extern int                 numActiveBullets;

// Process a bullets movement
void gam_processBulletMovement ( );

// Create new bullet
void gam_createBullet ( glm::vec3 direction, glm::vec3 position, GLfloat speed );

// Draw bullet model
void gam_drawBullets ( int whichShader );

// Remove a bullet from the World
void gam_removeBullet (int bulletIndex );

// Get the bullets current position by index
glm::vec3 gam_getBulletPosition(uint bulletIndex);