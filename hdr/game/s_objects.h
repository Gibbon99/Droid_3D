#pragma once

#include "s_globals.h"

typedef struct
{
	glm::vec3	position;
	glm::vec3	lightColor;
	bool		usesPhysics;
	uint		meshType;
	uint		physicsPtr;
	uint		objectID;
	uint		collisionID;
	float		scaleBy;
} _object;

extern vector<_object>		gameObject;

void obj_renderAllObjects(int whichShader);

// Add some objects to the world
void obj_addSomeObjects();
