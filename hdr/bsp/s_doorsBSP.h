#pragma once

#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include "s_renderBSP.h"

//-----------------------------------------------------------------------------
//
// Structures for door models
//
//-----------------------------------------------------------------------------

#define DOOR_LIP	2.0f

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
	GLfloat				pause;				// Time to stay open for
	glm::vec3			min;				// Current location for the bounding box
	glm::vec3			max;				// Current location for the bounding box

	glm::vec3			minOriginal;		// Starting value for the bounding box
	glm::vec3			maxOriginal;		// Starting value for the bounding box

	GLfloat         	travelDistance;     // How far does the door move

	GLfloat         	minMaxMove;         // Track movement for collision detection
	GLfloat				startLocation;		// 
	GLfloat				currentOffset;		// Where are we at the moment - Use the coords as an offset
	_triggerZone		trigger;			// Area defining door trigger area
	//glm::vec3			*originalVertPos;   // Used to record original starting position for moving doors verts around
	vector<glm::vec3>	originalVertPos;   // Used to record original starting position for moving doors verts around
	//int					*sourceIndexPos;    // What's the index that this vert came from in m_pVerts
	vector<int>			sourceIndexPos;    // What's the index that this vert came from in m_pVerts
	
	btConvexHullShape*		shape = NULL;
	btDefaultMotionState*	motionShape = NULL;
	btRigidBody*			rigidBody = NULL;
	btVector3				physicsStartLocation;
	
	vector<_myVertex>		vertexInfo;		// Copy the vertex for updating and uploading to GPU on move

} _doorModel;

extern vector<_doorModel>	doorModels;

extern int					numOfDoors;
extern int					numOfDoorsDrawn;
extern int					numOfDoorsNotDrawn;
extern bool					g_debugDoorTriggers;
extern float                doorSpeed;
extern float                doorDelay;

// Get the number of doors in the level
int bsp_findNumOfDoors();

// Draw all the door models
// First check if they are visible in the frustum or not
void bsp_addDoorFaces ();

// Show the door trigger area
void bsp_drawAllDoorTriggerZones ();

// Check for player vs door trigger collision
bool bsp_checkPlayerVsTrigger();

// Process the movements of all doors
void bsp_processAllDoorMovements ( );

// Free door memory
void bsp_freeDoorMemory();

// Upload door vertex information to the GPU
void bsp_uploadDoorVertex ();
