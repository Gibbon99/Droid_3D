#pragma once

#include "s_varsBSP.h"

extern int g_lightmapShift;

//	This creates a texture map from the light map image bits
void bsp_createLightmapTexture ( unsigned int &texture, byte *pImageBits, int width, int height );

// Work out the position for each light volume on the grid
void bsp_setupLightVolumeData();

// Show the locations of the light volumes
void bsp_showLightVolPositions(int whichShader);

// Get the ambient light color from the light volume array
// Pass in world location - return ambient color
glm::vec3 bsp_getAmbientColor(glm::vec3 position);