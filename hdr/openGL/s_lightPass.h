#pragma once

extern GLint  m_worldToLightViewMatrixID;
extern GLint  m_lightViewToProjectionMatrixID;
extern GLint  m_worldToCameraViewMatrixID;

// Render a full screen quad
void lt_renderFullscreenQuad(int whichShader);

// Render a full screen quad
void lt_renderDepthQuad(int whichShader);

// Render all the ligth spheres into the GBuffer
void lt_renderPointLights(int whichShader);