#pragma once

extern GLint  m_worldToLightViewMatrixID;
extern GLint  m_lightViewToProjectionMatrixID;
extern GLint  m_worldToCameraViewMatrixID;

// Render a full screen quad
void lt_renderFullscreenQuad(int whichShader);

// Get shader values for caching
void lt_initShaderValues(int whichShader);

// Start shader pass to render scene
void lt_beginLightPass();

// Render a full screen quad
void lt_renderDepthQuad(int whichShader);

// Render all the ligth spheres into the GBuffer
void lt_renderPointLights(int whichShader);

// Create simple UV-sphere to represent point light
void lt_createLightSphere();
