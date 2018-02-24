#ifndef HEADER_39C746F5D3A74686
#define HEADER_39C746F5D3A74686

#pragma once

extern float    acceleration;
extern float    maxSpeed;
extern float    currentSpeed;
extern float    friction;

extern float       currentBackwardSpeed;
extern float       currentForwardSpeed;
extern float       currentLeftSpeed;
extern float       currentRightSpeed;

extern glm::vec3   previousCamPosition;		// The position from last movement
extern glm::vec3   previousViewPosition;		// The view from the last movement

// Process player movement speed
void gam_processPlayerMovement(float interpolate);

#endif // header guard
