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

typedef struct		// Changes need to be relfect in shaders.cpp as well - fields must match
{
	GLuint          programID;
	GLuint	        inVertsID;
	GLuint          inNormalsID;
	GLuint		    inTextureCoordsID;
	GLuint		    inTextureCoordsID_1;
	GLuint		    inColorID;	    		// used for colorkey, fade value
	GLuint          inColorID_2;    		// used for holding secondary color info
	GLuint          inTextureUnit;  		// which primary texture is bound
	GLuint          inTextureUnit_1;  		// which secondary texture is bound
	GLuint          viewProjectionMat;
	GLuint          modelMat;
	GLuint			screenSizeID;

	bool            linkedOK;
	char            vertFileName[MAX_STRING_SIZE];
	char            fragFileName[MAX_STRING_SIZE];
	char            geomFileName[MAX_STRING_SIZE];
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
	SHADER_PHYSIC_DEBUG,
	SHADER_GEOMETRY_PASS,
	SHADER_MODEL_PASS,
	SHADER_DIR_LIGHT,
	SHADER_POINT_LIGHT,
	SHADER_BILLBOARD,
	SHADER_QUAD_2D,
	SHADER_SHADOW_MAP,
	SHADER_SHADOW_LIGHTING,
	NUM_SHADERS
};

// Get errors from Shaders
void gl_getGLSLError ( GLint object, int objectType );

// Load and compile shaders
bool gl_loadCompileShaders ( int programObject );

// Get the location of a string variable from the shaders
bool gl_getAttribVariable ( int whichShader, char *strVariable, char *shaderText, GLuint *varLocation );
bool gl_getUniformVariable ( int whichShader, char *strVariable, char *shaderText, GLuint *varLocation );

// Set the value of a vec3 variable in a shader - VEC3 version
bool gl_setUniformVariable3f ( int location, float v1, float v2, float v3 );

// Set the value of a vec3 variable in a shader - VEC4 version
bool gl_setUniformVariable4f ( int location, float v1, float v2, float v3, float v4 );

// Set the value of a variable in a shader - single INT version
bool gl_setUniformVariable ( int location, int setTo );

// Return string indicating which shader is passed
char *gl_getShaderName ( int whichShader );

// Set the value of a shader uniform or attribute
GLint shaderGetLocation ( int whichShader, string keyName );