#pragma once

#include "s_varsBSP.h"

//	This creates a texture map from the light map image bits
void bsp_createLightmapTexture(unsigned int &texture, byte *pImageBits, int width, int height);
