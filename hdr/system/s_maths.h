#pragma once

// BoxInFrustum will return one of this defines according to the position of the box in the frustum
#define COMPLETE_OUT 0
#define INTERSECT    1
#define COMPLETE_IN  2

//
// Used internally for sys_BoxInFrustrum
#define V_IN 1
#define V_OUT 2
#define V_INTERSECT 3


extern 		    glm::vec3 	clickRayOrigin;
extern          glm::vec3 	clickRayDirection;

// This extracts our frustum from the projection and modelview matrix
void sys_calculateFrustum();

// This determines if a BOX is in or around our frustum by it's min and max points
int sys_boxInFrustum ( float x, float y, float z, float x2, float y2, float z2 );
