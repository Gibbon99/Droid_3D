#pragma once

#include "s_varsBSP.h"

extern int g_lightmapShift;

//	This creates a texture map from the light map image bits
void bsp_createLightmapTexture ( unsigned int &texture, byte *pImageBits, int width, int height );

// Work out the position for each light volume on the grid
void bsp_setupLightVolumeData();