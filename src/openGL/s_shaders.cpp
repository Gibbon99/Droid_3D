#include "s_globals.h"
#include "s_shaders.h"
#include "io_fileSystem.h"
#include "s_lightPass.h"
#include "s_shadowMap.h"

/*
        GLuint      programID;
        GLint		inVertsID;
        GLint		inTextureCoordsID;
        GLint		inColorID;	        // used for colorkey, fade value
        GLint       inTextureUnit;      // which texture is bound
        GLint       projMatrixLocation;
        GLint       modelMatrixLocation;
        bool        linkedOK;
        char        vertFileName[MAX_STRING_SIZE];
        char        fragFileName[MAX_STRING_SIZE];
*/

_shaderProgram                shaderProgram[] =             // holds all the information about shader program
{
	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "depthMap.vert",        "depthMap.frag"},
	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "ttfFont.vert",         "ttfFont.frag"},
	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "justColor.vert",       "justColor.frag"},
//	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "physicsDebug.vert",    "physicsDebug.frag"},
//	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "shadowMap.vert",       "shadowMap.frag"},
	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "geometry_pass.vert",   "geometry_pass.frag"},
	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "lightPass.vert",       "lightPass.frag"},
	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "pointLight.vert",      "pointLight.frag"},
//	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "renderBSP.vert",       "renderBSP.frag"},
//	{-1,    0, 0, 0, 0,  0,  0,  0,  0,  0,  0, false,  "deferredRendering.vert",       "deferredRendering.frag"},
};

//-----------------------------------------------------------------------------
//
// Return string indicating which shader is passed
char *gl_getShaderName(int whichShader)
//-----------------------------------------------------------------------------
{
	return shaderProgram[whichShader].fragFileName;
}

//-----------------------------------------------------------------------------
//
// Get the location of a string variable from the shaders - UNIFORM variables
bool gl_getUniformVariable(int whichShader, char *strVariable, char *shaderText, GLint *varLocation)
//-----------------------------------------------------------------------------
{
	GLint   tempLocation;

	tempLocation = glGetUniformLocation(shaderProgram[whichShader].programID, strVariable);

	if (-1 == tempLocation)
		{
			con_print (CON_ERROR, true, "--> Error getting uniform location [ %s ] - [ %s ]", shaderText, strVariable);
			return false;

		}

	else
		con_print (CON_INFO, true, "Uniform location [ %i ] : [ %s ] - [ %s ]", tempLocation, shaderText, strVariable);

	//
	// Assign value to location passed in
	*varLocation = tempLocation;

	return true;
}

//-----------------------------------------------------------------------------
//
// Get the location of a string variable from the shaders - ATTRIBUTE variables
bool gl_getAttribVariable(int whichShader, char *strVariable, char *shaderText, GLint *varLocation)
//-----------------------------------------------------------------------------
{
	GLint   tempLocation;

	tempLocation = glGetAttribLocation(shaderProgram[whichShader].programID, strVariable);

	if (-1 == tempLocation)
		{
			con_print (CON_ERROR, true, "--> Error getting attribute location [ %s ] - [ %s ]", shaderText, strVariable);
			return false;

		}

	else
		con_print (CON_INFO, true, "Attribute location [ %i ] : [ %s ] - [ %s ]", tempLocation, shaderText, strVariable);

	//
	// Assign value to location passed in
	*varLocation = tempLocation;

	return true;
}

//-----------------------------------------------------------------------------
//
// Set the value of a variable in a shader - single INT version
bool gl_setUniformVariable(int location, int setTo)
//-----------------------------------------------------------------------------
{
	GL_CHECK(glUniform1i(location, setTo));

	return true;
}

//-----------------------------------------------------------------------------
//
// Set the value of a vec3 variable in a shader - VEC3 version
bool gl_setUniformVariable3f(int location, float v1, float v2, float v3)
//-----------------------------------------------------------------------------
{
	GL_CHECK(glUniform3f(location, v1, v2, v3));

	return true;
}

//-----------------------------------------------------------------------------
//
// Set the value of a vec3 variable in a shader - VEC4 version
bool gl_setUniformVariable4f(int location, float v1, float v2, float v3, float v4)
//-----------------------------------------------------------------------------
{
	GL_CHECK(glUniform4f(location, v1, v2, v3, v4));

	return true;
}

//-----------------------------------------------------------------------------
//
// Get errors from Shaders
void gl_getGLSLError(GLint object, int objectType)
//-----------------------------------------------------------------------------
{
	GLint   logLength;
	GLint   charsWritten;
	GLchar  *infoLog;

	switch (objectType)
		{
			case GLSL_SHADER:
				glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
				break;

			case GLSL_PROGRAM:
				glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);
				break;

			default:
				break;
		}

	if (logLength > 0)
		{
			infoLog = (GLchar *)malloc(logLength);

			if (NULL == infoLog)
				{
					printf("ERROR: Couldn't get memory to store GLSL error log.");
					return;
				}

			switch (objectType)
				{
					case GLSL_SHADER:
						glGetShaderInfoLog(object, logLength, &charsWritten, infoLog);
						break;

					case GLSL_PROGRAM:
						glGetProgramInfoLog(object, logLength, &charsWritten, infoLog);
						break;

					default:
						printf("ERROR: Invalid object type passed to sys_GetGLSLError");
						return;
						break;
				}

			printf("GLSL ERROR [ %s ]\n", infoLog);
			io_logToFile("GLSL ERROR [ %s ]", infoLog);
			free(infoLog);
		}
}

//-----------------------------------------------------------------------------
//
// Get and store the location of the common variables for each shader
bool gl_setShaderVars(int whichShader)
//-----------------------------------------------------------------------------
{
	//
	// Store the ID for the inPosition within the shader
	gl_getAttribVariable(whichShader, (char *)"inPosition", gl_getShaderName(whichShader), &shaderProgram[whichShader].inVertsID);
	//
	// Store the ID for the inPosition within the shader
	gl_getAttribVariable(whichShader, (char *)"inNormal", gl_getShaderName(whichShader), &shaderProgram[whichShader].inNormalsID);
	//
	// Store the ID for the texture coords for the shader
	gl_getAttribVariable(whichShader, (char *)"inTextureCoords",  gl_getShaderName(whichShader), &shaderProgram[whichShader].inTextureCoordsID);
	gl_getAttribVariable(whichShader, (char *)"inTextureCoords1", gl_getShaderName(whichShader), &shaderProgram[whichShader].inTextureCoordsID_1);
	//
	// Get standard Uniform locations and cache them
	gl_getUniformVariable(whichShader, (char *)"inTexture0", gl_getShaderName(whichShader), &shaderProgram[whichShader].inTextureUnit);
	gl_getUniformVariable(whichShader, (char *)"inTexture1", gl_getShaderName(whichShader), &shaderProgram[whichShader].inTextureUnit_1);
	//
	// Matrix locations
//	gl_getUniformVariable(whichShader, (char *)"u_viewProjectionMat", gl_getShaderName(whichShader), &shaderProgram[whichShader].viewProjectionMat);
	gl_getUniformVariable(whichShader, (char *)"u_modelMat",          gl_getShaderName(whichShader), &shaderProgram[whichShader].modelMat);

	//
	// Do variables for each shader unique to each one
	switch (whichShader)
		{
			case SHADER_TTF_FONT:
				//
				// Get the ID to store the colorkey value in
				gl_getAttribVariable(whichShader, (char *)"fontColor", gl_getShaderName(whichShader), &shaderProgram[whichShader].inColorID);
				break;

/*
			case SHADER_PHYSIC_DEBUG:
				//
				// Get the ID to store the vertex color value in
				gl_getAttribVariable(whichShader, (char *)"debugColor", gl_getShaderName(whichShader), &shaderProgram[whichShader].inColorID);
				break;
*/
			case SHADER_DIR_LIGHT:
				break;

			case SHADER_GEOMETRY_PASS:
				break;

/*
			case SHADER_SHADOWMAP:
//        gl_getShadowMapVariables(whichShader);
				break;
*/
			case SHADER_POINT_LIGHT:
				break;

			default:
				break;
		}

	return true;
}

//-----------------------------------------------------------------------------
//
// Load and compile shaders
bool gl_loadCompileShaders(int programObject)
//-----------------------------------------------------------------------------
{
	GLint   compiled;
	GLint   linked;
	GLint   *vertFile = NULL;
	GLint   vertFileLength;

	GLint  *fragFile = NULL;
	GLint   fragFileLength;

	GLuint       vertexShaderObject, fragmentShaderObject;

	con_print (CON_TEXT, true, "-----------------------------------------------------------------------");

	vertFileLength = io_getFileSize((char *)shaderProgram[programObject].vertFileName);

	if (-1 == vertFileLength)
		return false;

	vertFile = (GLint *)malloc(sizeof(char) * vertFileLength);

	if (NULL == vertFile)
		return false;

	if (-1 == io_getFileIntoMemory((char *)shaderProgram[programObject].vertFileName, (char *)vertFile))
		return false;

	fragFileLength = io_getFileSize((char *)shaderProgram[programObject].fragFileName);

	if (-1 == fragFileLength)
		return false;

	fragFile = (GLint *)malloc(sizeof(char) * fragFileLength);

	if (NULL == fragFile)
		return false;

	if (-1 == io_getFileIntoMemory((char *)shaderProgram[programObject].fragFileName, (char *)fragFile))
		return false;

	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	if ((0 == vertexShaderObject) || (0 == fragmentShaderObject))
		{
			printf("Error creating shader objects.\n");
			return false;
		}

	// Do a quick switch so we can do a double pointer below
	const char *szVShader = (char *)vertFile;
	// Attach the shader to the shaderObject
	GL_CHECK(glShaderSource(vertexShaderObject, 1, (const GLchar **)&szVShader, &vertFileLength));
	//
	// Do a quick switch so we can do a double pointer below
	const char *szFShader = (char *)fragFile;
	// Attach the shader to the shaderObject
	GL_CHECK(glShaderSource(fragmentShaderObject, 1, (const GLchar **)&szFShader, &fragFileLength));
	//
	// now compile the shader sourcde
	GL_CHECK(glCompileShader(vertexShaderObject));
	//
	// Check it compiled ok
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &compiled);

	if (true == compiled)
		con_print (CON_TEXT, true, "Vertex shader compiled ok - [ %s ]", shaderProgram[programObject].vertFileName);

	else
		{
			con_print (CON_TEXT, true, "Vertex shader compile failed [ %s]", shaderProgram[programObject].vertFileName);
			gl_getGLSLError(vertexShaderObject, GLSL_SHADER);
			return false;
		}

	glCompileShader(fragmentShaderObject);
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &compiled);

	if (true == compiled)
		con_print (CON_TEXT, true, "Fragment shader compiled ok - [ %s ]", shaderProgram[programObject].fragFileName);

	else
		{
			con_print (CON_TEXT, true, "Fragment shader compile failed [ %s ]", shaderProgram[programObject].fragFileName);
			gl_getGLSLError(fragmentShaderObject, GLSL_SHADER);
			return false;
		}

	shaderProgram[programObject].programID = glCreateProgram();

	GL_CHECK(glAttachShader(shaderProgram[programObject].programID, vertexShaderObject));
	GL_CHECK(glAttachShader(shaderProgram[programObject].programID, fragmentShaderObject));

	GL_CHECK(glLinkProgram(shaderProgram[programObject].programID));

	GL_CHECK(glGetProgramiv(shaderProgram[programObject].programID, GL_LINK_STATUS, &linked));

	if (true == linked)
		{
			con_print (CON_TEXT, true, "INFO: Shaders linked ok - [ %s ]", shaderProgram[programObject].fragFileName);
			shaderProgram[programObject].linkedOK = true;

		}

	else
		{
			con_print (CON_TEXT, true, "ERROR: Shaders failed to link - [ %s ]", shaderProgram[programObject].fragFileName);
			gl_getGLSLError(vertexShaderObject, GLSL_PROGRAM);
			return false;
		}

	//
	// Needed to avoid error in glUniform in gl_setShaderVars
	GL_CHECK(glUseProgram(shaderProgram[programObject].programID));
	//
	// Setup variables for this shader
	gl_setShaderVars(programObject);
	//
	// Free memory
	free(vertFile);
	free(fragFile);

	con_print (CON_TEXT, true, "-----------------------------------------------------------------------");

	return true;
}
