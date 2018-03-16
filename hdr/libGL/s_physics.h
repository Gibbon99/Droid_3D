#pragma once

#include "s_globals.h"
#include <bullet/btBulletDynamicsCommon.h>


extern float        gravityX;
extern float        gravityY;
extern float        gravityZ;

// Start Physics system
bool bul_startPhysics();

// Stop Physics system
bool bul_stopPhysics();

// Step the physics world
void bul_processPhysics ( btScalar physicsTime );

// Set gravity to current values in gravity variables
bool bul_setGravity();

// Create debug line information
void bul_enableDebug ( bool performDebug );

// Return position of the camera after physics world processed
glm::vec3   bul_returnCameraPosition();

// Set camera physics object
void bul_setCameraVelocity ( glm::vec3 camVelocity );

// Add a physics object to the world
bool bul_addPhysicsObject ( int index, int objectSize, int objectType, float objectMass, glm::vec3 objectPosition, btAlignedObjectArray<btVector3>& vertices  );
