#pragma once

#include "s_globals.h"

// Camera Attributes
extern glm::vec3    cam3_Position;
extern glm::vec3    cam3_Front;
extern glm::vec3    cam3_Up;
extern glm::vec3    cam3_Right;
extern glm::vec3    cam3_WorldUp;

// Euler Angles
extern float        cam3_Yaw;
extern float        cam3_Pitch;

// Camera options
extern float        cam3_MovementSpeed;
extern float        mouseSensitivity;

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void cam3_processMouseMovement ( int xoffset, int yoffset, GLboolean constrainPitch = true );

// Process movement keys
void cam3_processMovementKeys ( );

// Set matrix to 3d perspective mode
void cam3_createProjMatrix ();

// Create the view matrix
void cam3_createViewMatrix ( glm::vec3 newPosition );

// Init the camera with known values
void cam3_initCamera ( glm::vec3 location );

// Return the direction vector and turn this into velocity for physics
glm::vec3 cam3_getVelocity();

// Return the current camera position
glm::vec3 cam3_getCameraPosition();

