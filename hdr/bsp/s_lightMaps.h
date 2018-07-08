#pragma once

#include "s_varsBSP.h"

#define LIGHT_VOL_WIDTH     64.0f
#define LIGHT_VOL_DEPTH     64.0f
#define LIGHT_VOL_HEIGHT    128.0f
#define LIGHT_VOL_SLIP_COUNT    4

extern int      g_lightmapShift;
extern bool     g_debugVolLights;
extern int     lightVolIndex;
extern int     lightXPos;
extern int     lightZPos;

//	This creates a texture map from the light map image bits
void bsp_createLightmapTexture ( unsigned int &texture, byte *pImageBits, int width, int height );

// Work out the position for each light volume on the grid
void bsp_setupLightVolumeData();

// Show the locations of the light volumes
void bsp_showLightVolPositions(int whichShader);

// Get the ambient light color from the light volume array
// Pass in world location - return ambient color
glm::vec3 bsp_getAmbientColor(glm::vec3 position);