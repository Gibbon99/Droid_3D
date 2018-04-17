#pragma once

#include "s_varsBSP.h"

typedef struct
{
	glm::vec3   position;
	glm::vec2   texCoords;
	glm::vec3   normals;
} _myVertex;

extern int				g_texturesChanges;
extern int				g_vertIndexCounter;
extern int				g_numVertexPerFrame;

//	Goes through all of the faces and draws them if the type is FACE_POLYGON
void bsp_renderLevel ( const glm::vec3 &vPos, int whichShader );

// Upload all the vertex data in our own structure used to render the level
void bsp_uploadLevelVertex();

// Level data is all on GPU, this index is used to draw based on that data
void bsp_createVextexIndexArray ( tBSPFace *ptrFace );

// Actually draw the BSP face
void bsp_renderFace ( int whichFace, int whichAction, int whichTexture );

// Add the index for the face into vector array
void bsp_addFaceToArray(int whichFace );