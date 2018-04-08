#pragma once

#include "s_varsBSP.h"

typedef struct
{
	glm::vec3   position;
	glm::vec2   texCoords;
//	glm::vec2   lightmapCoords;
	glm::vec3   normals;
} _myVertex;

typedef struct
{
	int                 vertIndex;      // Index into vertex array - starting point
	int                 numVerts;
	int                 textureID;
//	int                 lightmapID;
} _myFace;

// Upload vertex arrays to GPU
bool bsp_uploadBspToGPU();

//	Goes through all of the faces and draws them if the type is FACE_POLYGON
void bsp_renderLevel ( const glm::vec3 &vPos, int whichShader );

// Add the faces to the unsorted face array
void bsp_addFaceToArray ( int whichFace, int leafIndex );

// Upload all the vertex data in our own structure used to render the level
void bsp_uploadLevelVertex();
