#pragma once

// Load a BSP file into memory
bool bsp_loadBSP(const char *strFileName, bool verboseOutput);

// Free memory used by BSP
void bsp_freeMem();
