#pragma once

extern bool         drawWireframe;
extern bool         showGBuffers;

extern     glm::mat4 shadowMat;
extern     glm::mat4 shadowViewMat;
extern     glm::mat4 shadowProjMat;

// Draw everything to the screen
void updateScreen ( float interpolate );
