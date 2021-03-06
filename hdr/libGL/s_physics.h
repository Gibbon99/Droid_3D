#pragma once

#include "s_globals.h"
#include <bullet/btBulletDynamicsCommon.h>

typedef struct
{
	btCollisionShape* 		shape;
	btDefaultMotionState* 	motionState;
	btRigidBody* 			rigidBody;
} _physicsObject;

extern float        gravityX;
extern float        gravityY;
extern float        gravityZ;

extern btDiscreteDynamicsWorld*                dynamicsWorld;

// Start Physics system
bool bul_startPhysics();

// Stop Physics system
bool bul_stopPhysics();

// Step the physics world
void bul_processPhysics ( btScalar physicsTime );

// Set gravity to current values in gravity variables
bool bul_setGravity();

// Return position of the camera after physics world processed
glm::vec3   bul_returnCameraPosition();

// Set camera physics object
void bul_setCameraVelocity ( glm::vec3 camVelocity );

// Add a physics object to the world
int bul_addPhysicsObject ( int index, int whichMesh, float scaleBy, int objectType, float objectMass, glm::vec3 objectPosition );

// Get position for physics object by index
glm::vec3 phy_getObjectPosition ( int index );

// Add a physics BSP hull object to the world
void bul_addPhysicsBSP ( float scalePhysicsBy, bool isEntity, int whichDoor, btAlignedObjectArray<btVector3>& vertices );

// Apply a force to a physics object
void bul_applyMovement(int index, float applyAmount, glm::vec3 direction);

// Set player rigid body to the same location as the camera
void bul_setPlayerPosition(glm::vec3 position, glm::vec3 orientation);

// Translate bullet results into GLM matrix
glm::mat4 phy_bulletToGlmMatrix ( int index );

// Remove a physics object from the world
void bul_removePhysicsObject( int physicsIndex );