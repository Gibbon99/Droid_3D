#pragma once

#include "s_globals.h"

typedef struct
{
	glm::vec3	position;
	glm::vec3	lightColor;
	bool		usesPhysics;
	int			meshType;
	int			physicsPtr;
	float		scaleBy;
	glm::vec4	finalSize;
} _object;

extern vector<_object>		gameObject;

void obj_renderAllObjects(int whichShader);

// Add some objects to the world
void obj_addSomeObjects();
