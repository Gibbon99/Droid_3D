#pragma once

#include "s_assimp.h"

extern bool     verboseLogging;
extern int      voxelCount;

// Show debug information
void showDebugInfo();

// Draw a bounding box
void debug_showBoundingBox(_boundingBox debugBox, glm::vec3 position, float scaleBy);