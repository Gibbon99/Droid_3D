#pragma once

#include "s_assimp.h"

extern bool     verboseLogging;

// Show debug information
void showDebugInfo();

// Draw a bounding box
void debug_showBoundingBox ( _boundingBox debugBox, glm::vec3 position, float scaleBy );

// Print the call stack up to this point
void debug_printCallStack();