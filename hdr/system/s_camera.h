#pragma once

#include "s_globals.h"

extern glm::vec3 	camPosition;
extern glm::vec3	camDirection;
extern glm::vec3	wantedCamPosition;
extern glm::vec3	wantedCamCopy;
extern glm::vec3    vecUp;
extern float   	    camMoveDistance;

// Called to point the camera vectors in the right direction
void cam_setupCamera();

//	This will move the camera to the new location
void cam_moveTo( glm::vec3 NewPosition );

//	This will move the camera forward or backward depending on the speed
void cam_moveCamera( float speed, float interpolate );

//	This strafes the camera left and right depending on the speed (-/+)
void cam_strafeCamera( float speed );

//	This updates the camera's view and strafe vector
void cam_update( float interpolate );

//	This updates the camera according to the view and position
void cam_look(glm::vec3 matPosition, glm::vec3 matDirection);

//	This allows us to look around using the mouse, like in most first person games.
void cam_setViewByMouse(float x, float y);

// Used for moving up/down
void cam_moveCameraYPos( float speed, float interpolate );

//	This function sets the camera's position and view and up vVector.
void cam_positionCamera( float positionX, float positionY, float positionZ,
                         float viewX,     float viewY,     float viewZ,
                         float upVectorX, float upVectorY, float upVectorZ );

// Return the current camera position
glm::vec3 cam_getPosition();

// Called each frame to work out collisions with mesh
void cam_resolveCollisions();
