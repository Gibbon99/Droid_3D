#pragma once

#include "s_globals.h"

extern glm::vec3 g_lightPosition[];

// Setup up the FBO and cubemap
// Return status  of framebuffer binding operation
bool shd_shadowMapInit(unsigned int shadowWidth, unsigned int shadowHeight);

// Clean up the Shadowmap
void shd_exitShadowMap();   // TODO: Call from exit

void SetLightWorldPos(const glm::vec3& Pos);

// Render the level to the depth cubemap textures
void shd_prepareDepthRender(glm::vec3 lightPos);

// Render the level to the depth cubemap textures
void shd_shadowMapPass(int whichShader);

// Get the texture ID for the depth map to debug
GLuint shd_getDepthTextureID();

void renderCube(int whichShader);