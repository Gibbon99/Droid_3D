#pragma once

extern bool            animateLight;

void gl_freeShadowMap();

bool gl_initShadowMap(unsigned int WindowWidth, unsigned int WindowHeight);

void gl_bindWriteShadowMap();

void gl_bindReadShadowMap();

void gl_unbindWriteShadowMap();

void gl_startShadowMap();

void gl_stopShadowMap();

void gl_debugShadowMap();

glm::vec3 gl_lightPos();
glm::vec3 gl_lightDir();

// Adjust light position
void gl_moveLightPos(glm::vec3 moveVector);

// Draw position of the light
//void drawLightPos(int whichShader);

// Move the light in a circle
void shadowMoveLight(float interpolate);

// Return the texture ID of the shadowDepthMap
GLuint gl_getShadowMapTextureID();

// Return the texture ID of the shadowMap positions
GLuint gl_getShadowMapPositionsID();

// Setup GLSL variables for lighting
void gl_getShadowMapVariables(int whichShader);

// Setup GLSL variables for lighting
void gl_setShadowMapVars(int whichShader, glm::mat4 v_matrixPass);

// Draw position of the light
void drawLightPos(int whichShader, glm::vec3 position);
