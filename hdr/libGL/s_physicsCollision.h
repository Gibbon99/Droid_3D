#pragma once

typedef struct
{
	bool	active = false;
	uint	objectSource;
	uint	objectIndex;
} _collisionObject;

extern vector<_collisionObject>		collisionObjects;

// Do collision detection
void phy_doCollisionDetection();

// Add an object to the collision object list for checking on collision
// Return the index into the collisionObjects list
int phy_addCollisionObject(int objectType, int objectIndex);

// Show collision object data
void phy_debugShowCollisionData();

// Remove a collision object from the list
void phy_removeCollisionObject( int collisionIndex );
