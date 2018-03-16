#pragma once

#include "s_varsBSP.h"

// Upload vertex arrays to GPU
bool bsp_uploadBspToGPU();

//	Goes through all of the faces and draws them if the type is FACE_POLYGON
void bsp_renderLevel ( const glm::vec3 &vPos, int whichShader );

// Add the faces to the unsorted face array
void bsp_addFaceToArray ( int whichFace, int leafIndex );
