#pragma once

// Init the shadow cubemap and texture
bool shd_initShadowMap(unsigned int shadowWidth, unsigned int shadowHeight);

// 0. create depth cubemap transformation matrices
void shd_shadowMapDepthStartRender(glm::vec3 lightPos, int whichShader);

// Render the level as normal
void shd_shadowRenderNormal(int whichShader, glm::vec3 lightPos);

// Get the Texture ID for the depth map
int shd_getDepthTextureID();
