#pragma once

const float DEGS_TO_RADS = 3.141592654f / 180.0f;
const float RADS_TO_DEGS = 180.0f / 3.141592654f;

// Define our cardinal axes
const glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f); // Positive x-axis points directly to the right
const glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f); // Positive y-axis points directly up
const glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f); // Positive z-axis points directly out of the screen

extern glm::vec3	cam2Location;
extern glm::vec3	cam2Rotation;
extern glm::vec3	cam2Direction;

// Set the initial values for the camera
void cam_initCamera(glm::vec3 location, glm::vec3 rotation);

// Handle mouse movement changes
void cam_handleMouseMovement ( float mouseX, float mouseY );

// Create the view matrix
void cam_createViewMatrix(glm::vec3 camLocation);

// Calculate which direction we need to move the camera and by what amount
glm::vec3 cam_getVelocity(float interpolate);

glm::vec3 cam_getLocation();
