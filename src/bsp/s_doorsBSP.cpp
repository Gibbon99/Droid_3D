#include <hdr/openGL/s_openGLWrap.h>
#include <hdr/bsp/s_doorsBSP.h>
#include "s_varsBSP.h"
#include "s_entitiesBSP.h"
#include "s_doorsBSP.h"
#include "s_maths.h"
#include "s_renderBSP.h"
#include "s_shaders.h"
#include "s_timing.h"
#include "s_convertBSP.h"
#include "io_keyboard.h"
#include "s_camera3.h"

int					numOfDoors;
int					numOfDoorsDrawn = 0;
int					numOfDoorsNotDrawn = 0;
bool				g_debugDoorTriggers;
float               doorSpeed;
float               doorDelay;

vector<_doorModel>	doorModels;

/*
 * Disable collision response with solid body - just use as a trigger
 *
 * mBody->setCollisionFlags(mBody->getCollisionFlags() |
    btCollisionObject::CF_NO_CONTACT_RESPONSE));
	 * */

//------------------------------------------------------------------
//
// Get the number of doors in the level
int bsp_findNumOfDoors()
//------------------------------------------------------------------
{
	_doorModel	tempDoor;
	glm::vec3	tempValue;
	//
	// Save value for use later on
	numOfDoors = bsp_getNumEntities ( "func_door" );

	if ( numOfDoors == 0 )
		return 0;

	doorModels.reserve ( numOfDoors );		// Get memory to hold all the doors

	//
	// Get the entity setID for the doors
	//
	// Could also get this from walking the entity array
	bsp_resetEntitySearchFlag();

	for ( int i = 0; i != numOfDoors; i++ )
		{
			tempDoor.setID = bsp_getEntitySetID ( (char *)"func_door", true );
			doorModels.push_back ( tempDoor );
		}

//	char tempKey[16];
	string tempKey;
	int index = 0;

//	strcpy ( tempKey, "model" );	// Store text result in here
	tempKey = "model";
	//
	// Get the BSP model reference index and store for the door
	//
	bsp_resetEntitySearchFlag();

	for ( int i = 0; i != numOfDoors; i++ )
		{
			bsp_findEntityInfo ( "func_door", tempKey, &tempValue, false, doorModels[i].setID, VAR_TYPE_INT );
			{
				doorModels[index].ptrModel = (int) tempValue.x;
				con_print ( CON_INFO, true, "Door [ %i ] ptrModel [ %i ]", index, doorModels[index].ptrModel);
				index++;
				tempKey = "model";
			}
		}

	int whichModel = 0;
	tBSPFace		*ptrFace = NULL;
	int vertCounter = 0;

	//
	// Set each doors default state
	//
	bsp_resetEntitySearchFlag();

	for ( int i = 0; i != numOfDoors; i++ )
		{
			doorModels[i].currentState = DOOR_STATE_CLOSED;
			doorModels[i].currentOffset = 0.0f;
			doorModels[i].min = m_pModels[doorModels[i].ptrModel].min;
			doorModels[i].max = m_pModels[doorModels[i].ptrModel].max;

			doorModels[i].minOriginal = m_pModels[doorModels[i].ptrModel].min;
			doorModels[i].maxOriginal = m_pModels[doorModels[i].ptrModel].max;

			doorModels[i].pause = 5.0f;		//TODO: Make configurable

			//
			// Get the angle for the door and calculate the trigger area
			//
			if ( bsp_findEntityInfo ( "func_door", "angle", &tempValue, false, doorModels[i].setID, VAR_TYPE_INT ) > 0 )
				{
					doorModels[i].angle = ( int ) tempValue.x;

					con_print(CON_INFO, true, "Door [ %i ] Angle [ %i ]", i, doorModels[i].angle);

					switch ( doorModels[i].angle )
						{
							case 180:
							case 360:
								doorModels[i].trigger.min.x = m_pModels[doorModels[i].ptrModel].min.x;
								doorModels[i].trigger.min.y = m_pModels[doorModels[i].ptrModel].min.y;
								doorModels[i].trigger.min.z = m_pModels[doorModels[i].ptrModel].min.z - ( TRIGGER_AREA / 2 );

								doorModels[i].trigger.max.x = m_pModels[doorModels[i].ptrModel].max.x;
								doorModels[i].trigger.max.y = m_pModels[doorModels[i].ptrModel].max.y;
								doorModels[i].trigger.max.z = m_pModels[doorModels[i].ptrModel].max.z + ( TRIGGER_AREA / 2 );

								doorModels[i].startLocation = m_pModels[doorModels[i].ptrModel].min.z;
								doorModels[i].travelDistance = fabs(m_pModels[doorModels[i].ptrModel].max.x - m_pModels[doorModels[i].ptrModel].min.x);
								break;

							case 90:
							case 270:
								doorModels[i].trigger.min.x = m_pModels[doorModels[i].ptrModel].min.x - ( TRIGGER_AREA / 2 );
								doorModels[i].trigger.min.z = m_pModels[doorModels[i].ptrModel].min.z;
								doorModels[i].trigger.min.y = m_pModels[doorModels[i].ptrModel].min.y;

								doorModels[i].trigger.max.x = m_pModels[doorModels[i].ptrModel].max.x + ( TRIGGER_AREA / 2 );
								doorModels[i].trigger.max.z = m_pModels[doorModels[i].ptrModel].max.z;
								doorModels[i].trigger.max.y = m_pModels[doorModels[i].ptrModel].max.y;

								doorModels[i].startLocation = m_pModels[doorModels[i].ptrModel].min.z;
								doorModels[i].travelDistance = fabs(m_pModels[doorModels[i].ptrModel].min.z - m_pModels[doorModels[i].ptrModel].max.z);
								break;

							case -1:
							case -2:
								doorModels[i].trigger.min.x = m_pModels[doorModels[i].ptrModel].min.x - ( TRIGGER_AREA / 2 );
								doorModels[i].trigger.min.y = m_pModels[doorModels[i].ptrModel].min.y;
								doorModels[i].trigger.min.z = m_pModels[doorModels[i].ptrModel].min.z + ( TRIGGER_AREA / 2 );
								doorModels[i].trigger.max.x = m_pModels[doorModels[i].ptrModel].max.x + ( TRIGGER_AREA / 2 );
								doorModels[i].trigger.max.y = m_pModels[doorModels[i].ptrModel].min.y;
								doorModels[i].trigger.max.z = m_pModels[doorModels[i].ptrModel].max.z - ( TRIGGER_AREA / 2 );
								doorModels[i].startLocation = m_pModels[doorModels[i].ptrModel].min.y;
								doorModels[i].travelDistance = fabs ( m_pModels[doorModels[i].ptrModel].max.y - m_pModels[doorModels[i].ptrModel].min.y );
								break;
						}
				}

			//
			// Get the starting / end positions for the doors
			//
			whichModel = doorModels[i].ptrModel;

			vertCounter = 0;

			for ( int j = 0; j != m_pModels[whichModel].numSurfaces; j++ )
				{
					ptrFace = &m_pFaces[m_pModels[whichModel].firstSurface + j];

					vertCounter += ptrFace->numMeshVerts;
				}

			doorModels[i].numVerts = vertCounter;

			con_print ( CON_INFO, true, "Door [ %i ] numVerts [ %i ]", i, doorModels[i].numVerts );
			//
			// Get memory to hold and remember the original vertex positions for the door
			doorModels[i].originalVertPos.reserve(vertCounter);

			//
			// Get memory to hold the index pointer for the source verts
			doorModels[i].sourceIndexPos.reserve(vertCounter);

			//
			// Now copy the original verts into originalVertPos
			//
			int offset = 0;
			int index = 0;
			int whichDoor;

			_myVertex		tempVertex;

			whichModel = doorModels[i].ptrModel;
			whichDoor = i;

			vertCounter = 0;

//
// TODO: Model 0 is the world model
//
			doorModels[whichDoor].vertexInfo.reserve(doorModels[i].numVerts );

			for ( int j = 0; j != m_pModels[whichModel].numSurfaces; j++ )
				{
					ptrFace = &m_pFaces[m_pModels[whichModel].firstSurface + j];
					offset = ptrFace->startVertIndex;

					for ( int k = 0; k != ptrFace->numMeshVerts; k++ )
						{
							index = m_pMeshIndex[ptrFace->startMeshVertIndex + k];

							doorModels[whichDoor].originalVertPos[vertCounter] = m_pVerts[offset + index].vPosition;
							doorModels[whichDoor].sourceIndexPos[vertCounter] = offset + index;
							//
							// Take a copy of the data to upload to the GPU when the position changes
							tempVertex.position =  m_pVerts[offset + index].vPosition;;
							tempVertex.normals =   m_pVerts[offset + index].vNormal;
							tempVertex.texCoords = m_pVerts[offset + index].vTextureCoord;

							doorModels[whichDoor].vertexInfo.push_back(tempVertex);

							vertCounter++;
						}
				}

			//
			// Now create physics object from convex hull
			//
			bspConvertDoors ( whichDoor, 1.0f );
		}

	return numOfDoors;
}

//-----------------------------------------------------------------------------
//
// Free door memory
void bsp_freeDoorMemory()
//-----------------------------------------------------------------------------
{
	doorModels.clear();
}

//-----------------------------------------------------------------------------
//
// Show the door trigger area
void bsp_drawDoorTriggerZone ( int whichModel, int whichShader )
//-----------------------------------------------------------------------------
{
#define Y_OFFSET		0.1f

	glm::vec3   		vertsTrigger[4];
	static GLuint		vao_ID;
	static GLuint		buffers[2];
	int                 faceCount = 2;
	static bool         initDone = false;

	unsigned int vertsTriggerIndex[] =
	{
		0,1,2,2,3,0,
	};

if (!initDone)
{
	// create the VAO
	GL_ASSERT (glGenVertexArrays (1, &vao_ID));
	GL_CHECK (glBindVertexArray (vao_ID));

	// Create buffers for the vertex data
	//GL_ASSERT ( glGenBuffers ( 2, buffers ) );
	buffers[0] = wrapglGenBuffers (1, __func__);
	buffers[1] = wrapglGenBuffers (1, __func__);

	// First point
	vertsTrigger[0].x = doorModels[whichModel].trigger.min.x;
	vertsTrigger[0].y = doorModels[whichModel].trigger.min.y + Y_OFFSET;
	vertsTrigger[0].z = doorModels[whichModel].trigger.min.z;
	// Second point
	vertsTrigger[1].x = doorModels[whichModel].trigger.max.x;
	vertsTrigger[1].y = doorModels[whichModel].trigger.min.y + Y_OFFSET;
	vertsTrigger[1].z = doorModels[whichModel].trigger.min.z;
	//Third point
	vertsTrigger[2].x = doorModels[whichModel].trigger.max.x;
	vertsTrigger[2].y = doorModels[whichModel].trigger.min.y + Y_OFFSET;
	vertsTrigger[2].z = doorModels[whichModel].trigger.max.z;
	// Fourth point
	vertsTrigger[3].x = doorModels[whichModel].trigger.min.x;
	vertsTrigger[3].y = doorModels[whichModel].trigger.min.y + Y_OFFSET;
	vertsTrigger[3].z = doorModels[whichModel].trigger.max.z;

	GL_CHECK (glUseProgram (shaderProgram[whichShader].programID));

	// Vertex coordinates buffer
	GL_ASSERT (glBindBuffer (GL_ARRAY_BUFFER, buffers[0]));
	GL_CHECK (glBufferData (GL_ARRAY_BUFFER, sizeof (vertsTrigger), vertsTrigger, GL_STATIC_DRAW));
	GL_CHECK (glEnableVertexAttribArray (shaderProgram[whichShader].inVertsID));
	GL_CHECK (glVertexAttribPointer (shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET (0)));

	// Index buffer
	GL_CHECK (glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, buffers[1]));
	GL_CHECK (glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (vertsTriggerIndex), vertsTriggerIndex, GL_STATIC_DRAW));

	// Unbind the VAO
	glBindVertexArray (0);

	initDone = false;
}

	// Now render it
	cam3_createProjMatrix ();
	modelMatrix = glm::mat4();

	GL_CHECK ( glBindVertexArray ( vao_ID ) );

	GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );

	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr ( modelMatrix ) ) );
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );
	//
	// Enable attribute to hold vertex information
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );

	GL_CHECK ( glDrawElements ( GL_TRIANGLES, faceCount*3, GL_UNSIGNED_INT, 0 ) );

	glUseProgram ( 0 );
	glBindVertexArray ( 0 );

	glDeleteBuffers( 2, buffers );
	glDeleteVertexArrays(1, &vao_ID);
}

//------------------------------------------------------------------
//
// Show the door trigger area
void bsp_drawAllDoorTriggerZones ()
//------------------------------------------------------------------
{
	for ( int i = 0; i != numOfDoors; i++ )
		{
			bsp_drawDoorTriggerZone (i, SHADER_COLOR);
		}
}

//------------------------------------------------------------------
//
// Draw all the door models
// First check if they are visible in the frustum or not
void bsp_addDoorFaces ()
//------------------------------------------------------------------
{
	numOfDoorsDrawn = 0;
	numOfDoorsNotDrawn = 0;

	for ( int i = 0; i != numOfDoors; i++ )
		{
			switch ( sys_boxInFrustum ( doorModels[i].min.x, doorModels[i].min.y, doorModels[i].min.z, doorModels[i].max.x, doorModels[i].max.y, doorModels[i].max.z ) )
				{
					case COMPLETE_OUT:
						numOfDoorsNotDrawn++;
						break;

					case COMPLETE_IN:
					case INTERSECT:
						numOfDoorsDrawn++;

						for ( int j = 0; j != m_pModels[doorModels[i].ptrModel].numSurfaces; j++ )
							{
								bsp_addFaceToArray ( m_pModels[doorModels[i].ptrModel].firstSurface + j );

//								con_print(CON_INFO, true, "Door [ %i ] lightmapID [ %i ]", m_pFaces[m_pModels[doorModels[i].ptrModel].firstSurface ].lightmapID);
							}

						break;
				}
		}
}

//-------------------------------------------------------------------------------
//
// Check for collision between door bounding box and point
int bsp_checkPointDoorCollision ( glm::vec3 objectPos, int whichDoor )
//-------------------------------------------------------------------------------
{
	switch (doorModels[whichDoor].angle)
		{
			case 180:
			case 360:
				if ( objectPos.x < doorModels[whichDoor].trigger.min.x )
					if ( objectPos.x > doorModels[whichDoor].trigger.max.x )
						if ( objectPos.z > doorModels[whichDoor].trigger.min.z )
							if ( objectPos.z < doorModels[whichDoor].trigger.max.z )
								return whichDoor;

				break;

			case 90:
			case 270:
				if ( objectPos.x > doorModels[whichDoor].trigger.min.x )
					if ( objectPos.x < doorModels[whichDoor].trigger.max.x )
						if ( objectPos.z > doorModels[whichDoor].trigger.min.z )
							if ( objectPos.z < doorModels[whichDoor].trigger.max.z )
								return whichDoor;

				break;
		}
	return -1;
}

//-------------------------------------------------------------------------------
//
// Move a door model
void bsp_startModelMoveDoor ( int whichDoor, int direction )
//-------------------------------------------------------------------------------
{
	if ( ( doorModels[whichDoor].currentState == DOOR_STATE_OPEN ) ||
	        ( doorModels[whichDoor].currentState == DOOR_STATE_OPENING ) )
		return;		// Door is already moving, don't do anything

	doorModels[whichDoor].currentState = direction;
}

//-------------------------------------------------------------------------------
//
// Check for player vs door trigger collision
bool bsp_checkPlayerVsTrigger()
//-------------------------------------------------------------------------------
{
	for ( int i = 0; i != numOfDoors; i++ )
		{
			switch ( sys_boxInFrustum ( doorModels[i].min.x, doorModels[i].min.y, doorModels[i].min.z, doorModels[i].max.x, doorModels[i].max.y, doorModels[i].max.z ) )
				{
					case COMPLETE_IN:
					case INTERSECT:
						if ( bsp_checkPointDoorCollision (cam3_Position, i) > -1 )
							{
								bsp_startModelMoveDoor (i, DOOR_STATE_OPENING);

								printf ("Inside door trigger\n");
							}
				}
		}
	return true;
}


//-------------------------------------------------------------------------------
//
// Process the movements of a door
void bsp_processSingleDoorMovement ( int whichDoor )
//-------------------------------------------------------------------------------
{
	switch ( doorModels[whichDoor].currentState )
		{
			case DOOR_STATE_CLOSED:
				doorModels[whichDoor].minMaxMove = 0.0f;
				doorModels[whichDoor].currentOffset = 0.0f;
				break;

			case DOOR_STATE_OPEN:
				doorModels[whichDoor].minMaxMove = 0.0f;
				doorModels[whichDoor].pause -= doorDelay;

				if ( doorModels[whichDoor].pause < 0.0f )
					{
						doorModels[whichDoor].pause = 5.0f;
						doorModels[whichDoor].currentState = DOOR_STATE_CLOSING;
					}
				break;

			case DOOR_STATE_OPENING:
				if ( doorModels[whichDoor].currentOffset > doorModels[whichDoor].travelDistance - DOOR_LIP )
					{
						doorModels[whichDoor].currentOffset = doorModels[whichDoor].travelDistance - DOOR_LIP;
						doorModels[whichDoor].currentState = DOOR_STATE_OPEN;
						doorModels[whichDoor].minMaxMove = doorModels[whichDoor].currentOffset;
					}
				else
					{
						doorModels[whichDoor].currentOffset += doorSpeed;
						doorModels[whichDoor].minMaxMove += doorSpeed;    // same as offset ??
					}
				break;

			case DOOR_STATE_CLOSING:

				if ( doorModels[whichDoor].currentOffset > 0.0f )
					{
						doorModels[whichDoor].currentOffset -= doorSpeed;
						doorModels[whichDoor].minMaxMove = 0 - ( doorModels[whichDoor].minMaxMove - ( doorSpeed ) );		// Get negative move
					}
				else
					{
						doorModels[whichDoor].currentOffset = 0.0f;
						doorModels[whichDoor].currentState = DOOR_STATE_CLOSED;
						doorModels[whichDoor].minMaxMove = 0.0f;
					}
				break;
		}
}

//-------------------------------------------------------------------------------
//
// Move the physics convex hull along with the door model
void bsp_moveDoorPhysics ( int whichDoor, int doorAngle )
//-------------------------------------------------------------------------------
{

	btTransform		doorTransform;
	btVector3		location;

	doorModels[whichDoor].motionShape->getWorldTransform(doorTransform);

	location = doorModels[whichDoor].physicsStartLocation;

	switch (doorAngle)
		{
			case 90:
				location.setZ(location.z() + doorModels[whichDoor].currentOffset);
				break;

			case 270:
				location.setZ(location.z() - doorModels[whichDoor].currentOffset);
				break;

			case 180:
				location.setX(location.x() - doorModels[whichDoor].currentOffset);
				break;

			case 360:
				location.setX(location.x() + doorModels[whichDoor].currentOffset);
				break;
		}

	doorTransform.setOrigin(location);

	doorModels[whichDoor].motionShape->setWorldTransform(doorTransform);
	doorModels[whichDoor].rigidBody->setWorldTransform(doorTransform);
}

//-------------------------------------------------------------------------------
//
// Upload door vertex information to the GPU
void bsp_uploadDoorVertex ()
//-------------------------------------------------------------------------------
{
	GLintptr		offset;
	GLsizeiptr		size;

	size = sizeof(_myVertex);

	GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, bspVBO ));    // Rebind the vertex buffer for the BSP - not the uniform object buffer

	for (int i = 0; i != numOfDoors; i++)
		{
			for (int j = 0; j != doorModels[i].numVerts; j++)
				{
					offset = doorModels[i].sourceIndexPos[j];
					glBufferSubData(GL_ARRAY_BUFFER, offset * size, size, (void *)&doorModels[i].vertexInfo[j]);
				}
		}
}

//-------------------------------------------------------------------------------
//
// Process the movements of all doors
void bsp_processAllDoorMovements ( )
//-------------------------------------------------------------------------------
{
	int         whichModel;
	int         whichDoor;

	tBSPFace	*ptrFace;

	int j = 0;

	for ( int k = 0; k != numOfDoors; k++ )
		{
			bsp_processSingleDoorMovement (k);

			bsp_moveDoorPhysics (k, doorModels[k].angle);

			whichModel = doorModels[k].ptrModel;
			whichDoor = k;

			j = 0;

			for ( int i = 0; i != m_pModels[whichModel].numSurfaces; i++ )
				{
					ptrFace = &m_pFaces[m_pModels[whichModel].firstSurface + i];

					for ( int l = 0; l != ptrFace->numMeshVerts; l++ )
						{
							switch ( doorModels[whichDoor].angle )
								{
									case 90:
										doorModels[whichDoor].vertexInfo[j].position.z = doorModels[whichDoor].originalVertPos[j].z + doorModels[whichDoor].currentOffset;
										break;

									case 270:
										doorModels[whichDoor].vertexInfo[j].position.z = doorModels[whichDoor].originalVertPos[j].z - doorModels[whichDoor].currentOffset;
										break;

									case 180:
										doorModels[whichDoor].vertexInfo[j].position.x = doorModels[whichDoor].originalVertPos[j].x - doorModels[whichDoor].currentOffset;
										break;

									case 360:
										doorModels[whichDoor].vertexInfo[j].position.x = doorModels[whichDoor].originalVertPos[j].x + doorModels[whichDoor].currentOffset;
										break;

									case -1:	// Move up
										doorModels[whichDoor].vertexInfo[j].position.y = doorModels[whichDoor].originalVertPos[j].y + doorModels[whichDoor].currentOffset;
										break;

									case -2:	// Move down
										doorModels[whichDoor].vertexInfo[j].position.y = doorModels[whichDoor].originalVertPos[j].y - doorModels[whichDoor].currentOffset;
										break;

									default:
										con_print ( CON_INFO, true, "Door [ %i ] has unknown angle [ %i ]", whichDoor, doorModels[whichDoor].angle );
										break;
								}

							j++;
						}
				}

			switch ( doorModels[whichDoor].angle )  // used for culling when drawing - needs to be updated correctly
				{
					case 90:
						doorModels[k].min.z = doorModels[k].minOriginal.z + doorModels[whichDoor].currentOffset;
						doorModels[k].max.z = doorModels[k].maxOriginal.z + doorModels[whichDoor].currentOffset;
						break;

					case 270:	// Working
						doorModels[k].min.z = doorModels[k].minOriginal.z - doorModels[whichDoor].currentOffset;
						doorModels[k].max.z = doorModels[k].maxOriginal.z - doorModels[whichDoor].currentOffset;
						break;

					case 180:
						doorModels[k].min.x = doorModels[k].minOriginal.x - doorModels[whichDoor].currentOffset;
						doorModels[k].max.x = doorModels[k].maxOriginal.x - doorModels[whichDoor].currentOffset;
						break;

					case 360:
						doorModels[k].min.x = doorModels[k].minOriginal.x + doorModels[whichDoor].currentOffset;
						doorModels[k].max.x = doorModels[k].maxOriginal.x + doorModels[whichDoor].currentOffset;
						break;

					case -1:	// Move up
						doorModels[k].min.y = doorModels[k].minOriginal.y + doorModels[whichDoor].currentOffset;
						doorModels[k].max.y = doorModels[k].maxOriginal.z + doorModels[whichDoor].currentOffset;
						break;

					case -2:	// Move down
						doorModels[k].min.y = doorModels[k].minOriginal.y - doorModels[whichDoor].currentOffset;
						doorModels[k].max.y = doorModels[k].maxOriginal.z - doorModels[whichDoor].currentOffset;
						break;

					default:
						con_print (CON_ERROR, GL_TRUE, "Door [ %i ] has unknown angle [ %i ]", whichDoor, doorModels[whichDoor].angle);
						break;
				}
		}
}