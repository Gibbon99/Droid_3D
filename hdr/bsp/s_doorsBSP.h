#pragma once

#include <vector>
#include <bullet/btBulletDynamicsCommon.h>

//-----------------------------------------------------------------------------
//
// Structures for door models
//
//-----------------------------------------------------------------------------

#define DOOR_SPEED	0.6f
#define DOOR_LIP	2.0f
#define PAUSE_DELAY	0.1f

#define DOOR_STATE_CLOSED	0
#define DOOR_STATE_CLOSING	1
#define DOOR_STATE_OPENING	2
#define DOOR_STATE_OPEN		3

#define TRIGGER_AREA		50.0f		// Extend trigger area out from bounding box

typedef struct
{
	glm::vec3	min;
	glm::vec3	max;
} _triggerZone;

typedef struct
{
	int					setID;				// ID number from entity list
	int					ptrModel;
	uint				textureID;			// Used to draw texture, and in physics to not create brush / solid hull
	int					currentState;
	int					angle;				// Angle used for opening/closing
	int					numVerts;           // Number of verts used for drawing this model
	GLfloat				pause;				// Time to say open for
	glm::vec3			min;				// Starting value for the bounding box
	glm::vec3			max;				// Starting value for the bounding box

	glm::vec3			minOriginal;				// Starting value for the bounding box
	glm::vec3			maxOriginal;				// Starting value for the bounding box

	GLfloat         	travelDistance;     // How far does the door move

	GLfloat         	minMaxMove;         // Track movement for collision detection
	GLfloat				startLocation;		// Y Axis to stop closing at
	GLfloat				currentOffset;		// Where are we at the moment - Use the coords as an offset
	_triggerZone		trigger;			// Area defining door trigger area
	glm::vec3			*originalVertPos;   // Used to record original starting position for moving doors verts around
	int					*sourceIndexPos;    // What's the index that this vert came from in m_pVerts
	
	btConvexHullShape*		shape = NULL;
	btDefaultMotionState*	motionShape = NULL;
	btRigidBody*			rigidBody = NULL;
	btVector3				physicsStartLocation;

} _doorModel;

extern vector<_doorModel>	doorModels;

extern int					numOfDoors;
extern int					numOfDoorsDrawn;
extern int					numOfDoorsNotDrawn;
extern bool					g_debugDoorTriggers;

// Get the number of doors in the level
int bsp_findNumOfDoors();

// Draw all the door models
// First check if they are visible in the frustum or not
void bsp_drawAllDoors();

// Show the door trigger area
void bspDrawAllDoorTriggerZones();

// Check for player vs door trigger collision
bool bsp_checkPlayerVsTrigger();

// Process the movements of all doors
void bspProcessAllDoorMovements ( float interpolate );

// Free door memory
void bspFreeDoorMemory();
