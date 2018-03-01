#pragma once

// Include GLM
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

extern glm::mat4       projMatrix;
extern glm::mat4       modelMatrix;
extern glm::mat4       viewMatrix;

//-----------------------------------------------------------------------------
//
// Structure to hold OpenGL colors
//
//-----------------------------------------------------------------------------
typedef struct
{
	float		red;
	float		green;
	float		blue;
	float		alpha;
} _glColor;

typedef struct
{
	unsigned int    vaoID;
	unsigned int    vertVBO_ID;
	unsigned int    texVBO_ID;
	unsigned int    colVBO_ID;
	int             inPositionIndex;     // Can return -1 on failure
	int             inTextureCoordsIndex;
	int             inColorIndex;
	int             textureLocation;     // Which texture unit is the font texture bound to
} _dataObject;

// Draw the console screen
void gl_drawScreen(bool updateNow);

// Get all the openGL errors
void gl_getAllGLErrors ( int errorNum, const char *calledFrom, int line );

// Load the font texture
bool gl_loadFontTexture(const char *fontFilename);

// Set OpenGL to 2d orthographic mode
void gl_set2DMode();

// Set matrix to 3d perspective mode
void gl_set3DMode();

// Draw a debug line to see the picking ray
void drawDebugLine( glm::vec3 startPoint, glm::vec3 endPoint, glm::vec3 pos, int drawType, float length, bool reinit, float scaleBy );

// OpenGL Debug - Register callback
void gl_registerDebugCallback();