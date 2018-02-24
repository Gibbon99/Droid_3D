#pragma once

//#include "../s_system/s_globals.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ZERO_MEM(a) memset(a, 0, sizeof(a))

enum GBUFFER_TEXTURE_TYPE
{
	GBUFFER_TEXTURE_TYPE_DIFFUSE,

	GBUFFER_TEXTURE_TYPE_POSITION,

	GBUFFER_TEXTURE_TYPE_NORMAL,
//    GBUFFER_TEXTURE_TYPE_TEXCOORD,
	GBUFFER_NUM_TEXTURES
};

// Clean up FBO and allocated GBuffers
void gl_stopDefRender();

// Create the GBuffer
void gl_createGBufTex( GLenum texUnit, GLenum internalFormat, GLuint &texid, GLenum format, GLuint width, GLuint height, GLenum dataType );

// Which buffer will we read from
void gl_setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

// Bind FBO for reading
void gl_bindForReading();

// Bind FBO for writing
void gl_bindForWriting();

// Setup the FBO and other GBuffers
bool gl_initDefRender(int screenWidth, int screenHeight);

// Show GBuffers for debugging
void gl_showGBuffers();

// Return texture ID for texture array by index
GLuint gl_returnTexID(int index);

GLuint gl_returnDepthTexID();

// print out the FBO infos
void printFramebufferInfo();
