#pragma once

#include "s_globals.h"

// Load a BSP file into memory
bool bsp_loadBSP ( const char *strFileName, bool verboseOutput );

// Free memory used by BSP
void bsp_freeMem();

// Swap the Z and Y values so Y is up
glm::vec3 bsp_swapValues(glm::vec3 sourceValue);