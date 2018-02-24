#pragma once

//
// Used for collision detection types
#define CO_RAY		0
#define CO_SPHERE   1
#define CO_BOX		2

#define EPSILON			0.03125f

extern bool         bspCollisionDetectOn;
extern bool         collisionWithBSP;
extern bool         testCollisionWithBSP;

// This structure will store all the information of a trace. Once we call a collision
// function this structure will be returned, indicating what happen
struct sMoveData
{
	float		Fraction;			// How long has the trace move (0 no move, 1 complete move or not collision)
	glm::vec3	EndPoint;			// Where we collide if we did it? Return the Target if no collision occured
	glm::vec3	CollisionNormal;	// Store the normal of the plane we collide with
	bool		StartOut;			// True if the ray segment starts outside of a solid volume.
	bool		AllSolid;			// True if the line segment is completely enclosed in a solid volume.
	int			nodeIndex;			// Index into the leaf
};

void bsp_checkBoxMove ( glm::vec3 Start, glm::vec3 End, glm::vec3 inputMins, glm::vec3 inputMaxs, sMoveData &localDataMoveBox );

// Check sphere ( with radius ) against BSP tree
void bsp_checkSphereMove( glm::vec3 Start, glm::vec3 End, float Radius, sMoveData &localMoveDataSphere);
