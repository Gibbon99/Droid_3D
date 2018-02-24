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
