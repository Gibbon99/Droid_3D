#pragma once

#include "s_varsBSP.h"

typedef struct
{
	glm::vec3   position;
	glm::vec3   normals;
	glm::vec2   texCoords;
	glm::vec2   texCoordsLightmap;
} _myVertex;

//extern int				g_texturesChanges;
extern bool             g_renderTextures;
extern int				g_vertIndexCounter;
extern int				g_numVertexPerFrame;

extern vector<unsigned int>				g_currentFrameVertexIndex;
extern vector<int>						g_facesForFrame;

// Prepare to render faces
void bsp_prepareFaceRender(int whichShader);

//	Goes through all of the faces and draws them if the type is FACE_POLYGON
void bsp_renderLevel ( const glm::vec3 &vPos, int whichShader, bool doRender );

// Upload all the vertex data in our own structure used to render the level
void bsp_uploadLevelVertex();

// Level data is all on GPU, this index is used to draw based on that data
void bsp_createVextexIndexArray ( tBSPFace *ptrFace );

// Actually draw the BSP face
void bsp_renderFace ( int whichFace, int whichAction, int whichTexture, int whichShader );

// Add the index for the face into vector array
void bsp_addFaceToArray(int whichFace );

// Go through the vector face indexes and render the face - including door models
//
// Sort the faces based on textureID
void bsp_renderAllFaces(int whichShader);