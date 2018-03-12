#pragma once

#include "s_globals.h"

//
// Each shader will track the location of common variables
// inVertsID - location of vertices
// inTextureCoordsID - which texture unit is being used
// inColorID - can be used to pass in color values for things like colorkey, fade, tint
// inTextureUnit - which texture unit is the texture in
//
// Every shader will have these common variables which will be used by the current
// object being drawn

typedef struct
{
	GLint           programID;
	GLint	        inVertsID;
	GLint           inNormalsID;
	GLint		    inTextureCoordsID;
	GLint		    inTextureCoordsID_1;
	GLint		    inColorID;	    // used for colorkey, fade value
	GLint           inColorID_2;    // used for holding secondary color info
	GLint           inTextureUnit;  // which texture is bound
	GLint           inTextureUnit_1;  // which texture is bound
	GLint           viewProjectionMat;
	GLint           modelMat;
//        GLint           MVPLocation;
	bool            linkedOK;
	char            vertFileName[MAX_STRING_SIZE];
	char            fragFileName[MAX_STRING_SIZE];
} _shaderProgram;

extern _shaderProgram         shaderProgram[];

#if defined (WIN32)
extern enum
#else
enum
#endif
{
	SHADER_DEPTHMAP,
	SHADER_TTF_FONT,
	SHADER_COLOR,
//	SHADER_PHYSIC_DEBUG,
//	SHADER_SHADOWMAP,
	SHADER_GEOMETRY_PASS,
	SHADER_DIR_LIGHT,
	SHADER_POINT_LIGHT,
//	SHADER_RENDER_BSP,
//	SHADER_DEF_RENDER,
	NUM_SHADERS
};

// Get errors from Shaders
void gl_getGLSLError(GLint object, int objectType);

// Load and compile shaders
bool gl_loadCompileShaders(int programObject);

// Get the location of a string variable from the shaders
bool gl_getAttribVariable(int whichShader, char *strVariable, char *shaderText, GLint *varLocation);
bool gl_getUniformVariable(int whichShader, char *strVariable, char *shaderText, GLint *varLocation);

// Set the value of a vec3 variable in a shader - VEC3 version
bool gl_setUniformVariable3f(int location, float v1, float v2, float v3);

// Set the value of a vec3 variable in a shader - VEC4 version
bool gl_setUniformVariable4f(int location, float v1, float v2, float v3, float v4);

// Set the value of a variable in a shader - single INT version
bool gl_setUniformVariable(int location, int setTo);

// Return string indicating which shader is passed
char *gl_getShaderName(int whichShader);
