#include <hdr/openGL/s_shaders.h>
#include "s_globals.h"
#include "s_shaders.h"
#include "io_fileSystem.h"
#include "s_lightPass.h"
#include "s_shadowMap.h"
#include <map>

/*
GLuint           programID;
	GLuint	        inVertsID;
	GLuint           inNormalsID;
	GLuint		    inTextureCoordsID;
	GLuint		    inTextureCoordsID_1;
	GLuint		    inColorID;	    		// used for colorkey, fade value
	GLuint           inColorID_2;    		// used for holding secondary color info
	GLuint           inTextureUnit;  		// which primary texture is bound
	GLuint           inTextureUnit_1;  		// which secondary texture is bound
	GLuint           viewProjectionMat;
	GLuint           modelMat;
	GLuint			screenSizeID;

	bool            linkedOK;
	char            vertFileName[MAX_STRING_SIZE];
	char            fragFileName[MAX_STRING_SIZE];
	char            geomFileName[MAX_STRING_SIZE];
*/
typedef struct
{
	GLint       locationID;
	int         refCount;
	bool        isFound;
} _shaderVarType;

std::map<string, _shaderVarType> shaderValues;
std::map<string, _shaderVarType>::iterator shaderItr;

_shaderProgram                shaderProgram[] =             // holds all the information about shader program
{
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "depthMap.vert",          "depthMap.frag",        ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "ttfFont.vert",           "ttfFont.frag",         ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "justColor.vert",         "justColor.frag",       ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "physicsDebug.vert",      "physicsDebug.frag",    ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "geometry_pass.vert",     "geometry_pass.frag",   ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "geometry_pass.vert",     "model_pass.frag",      ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "lightPass.vert",         "lightPass.frag",       ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "pointLight.vert",        "pointLight.frag",      ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "billboard.vert",         "billboard.frag",       "billboard.geom"},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "quad2D.vert",            "quad2D.frag",          ""},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "shadowMap.vert",         "shadowMap.frag",       "shadowMap.geom"},
	{0,    0, 0, 0, 0,  0,  0,  0,  0,  0, 0,  0, false,  "shadowLighting.vert",    "shadowLighting.frag",  ""},
};

//-----------------------------------------------------------------------------
//
// Return string indicating which shader is passed
char *gl_getShaderName ( int whichShader )
//-----------------------------------------------------------------------------
{
	return shaderProgram[whichShader].fragFileName;
}
#define VERBOSE_SHADERS 1

//-----------------------------------------------------------------------------
//
// Get the location of a string variable from the shaders - UNIFORM variables
bool gl_getUniformVariable ( int whichShader, char *strVariable, char *shaderText, GLuint *varLocation )
//-----------------------------------------------------------------------------
{
	GLint   tempLocation;

	tempLocation = glGetUniformLocation ( shaderProgram[whichShader].programID, strVariable );

#ifdef VERBOSE_SHADERS
	if ( -1 == tempLocation )
		{
			con_print ( CON_ERROR, true, "--> Error getting uniform location [ %s ] - [ %s ]", shaderText, strVariable );
			return false;

		}

	else
		con_print ( CON_INFO, true, "Uniform location [ %i ] : [ %s ] - [ %s ]", tempLocation, shaderText, strVariable );
#endif
	//
	// Assign value to location passed in
	*varLocation = tempLocation;

	return true;
}

//-----------------------------------------------------------------------------
//
// Get the location of a string variable from the shaders - ATTRIBUTE variables
bool gl_getAttribVariable ( int whichShader, char *strVariable, char *shaderText, GLuint *varLocation )
//-----------------------------------------------------------------------------
{
	GLint   tempLocation;

	tempLocation = glGetAttribLocation ( shaderProgram[whichShader].programID, strVariable );

#ifdef VERBOSE_SHADERS
	if ( -1 == tempLocation )
		{
			con_print ( CON_ERROR, true, "--> Error getting attribute location [ %s ] - [ %s ]", shaderText, strVariable );
			return false;

		}

	else
		con_print ( CON_INFO, true, "Attribute location [ %i ] : [ %s ] - [ %s ]", tempLocation, shaderText, strVariable );
#endif

	//
	// Assign value to location passed in
	*varLocation = tempLocation;

	return true;
}

//-----------------------------------------------------------------------------
//
// Set the value of a variable in a shader - single INT version
bool gl_setUniformVariable ( int location, int setTo )
//-----------------------------------------------------------------------------
{
	GL_CHECK ( glUniform1i ( location, setTo ) );

	return true;
}

//-----------------------------------------------------------------------------
//
// Set the value of a vec3 variable in a shader - VEC3 version
bool gl_setUniformVariable3f ( int location, float v1, float v2, float v3 )
//-----------------------------------------------------------------------------
{
	GL_CHECK ( glUniform3f ( location, v1, v2, v3 ) );

	return true;
}

//-----------------------------------------------------------------------------
//
// Set the value of a vec3 variable in a shader - VEC4 version
bool gl_setUniformVariable4f ( int location, float v1, float v2, float v3, float v4 )
//-----------------------------------------------------------------------------
{
	GL_CHECK ( glUniform4f ( location, v1, v2, v3, v4 ) );

	return true;
}

//-----------------------------------------------------------------------------
//
// Get errors from Shaders
void gl_getGLSLError ( GLint object, int objectType )
//-----------------------------------------------------------------------------
{
	GLint   logLength;
	GLint   charsWritten;
	GLchar  *infoLog;

	switch ( objectType )
		{
			case GLSL_SHADER:
				glGetShaderiv ( object, GL_INFO_LOG_LENGTH, &logLength );
				break;

			case GLSL_PROGRAM:
				glGetProgramiv ( object, GL_INFO_LOG_LENGTH, &logLength );
				break;

			default:
				break;
		}

	if ( logLength > 0 )
		{
			infoLog = ( GLchar * ) malloc ( logLength );

			if ( NULL == infoLog )
				{
					printf ( "ERROR: Couldn't get memory to store GLSL error log." );
					return;
				}

			switch ( objectType )
				{
					case GLSL_SHADER:
						glGetShaderInfoLog ( object, logLength, &charsWritten, infoLog );
						break;

					case GLSL_PROGRAM:
						glGetProgramInfoLog ( object, logLength, &charsWritten, infoLog );
						break;

					default:
						printf ( "ERROR: Invalid object type passed to sys_GetGLSLError" );
						return;
						break;
				}

			printf ( "GLSL ERROR [ %s ]\n", infoLog );
			io_logToFile ( "GLSL ERROR [ %s ]", infoLog );
			free ( infoLog );
		}
}

//-----------------------------------------------------------------------------
//
// Get and store the location of the common variables for each shader
bool gl_setShaderVars ( int whichShader )
//-----------------------------------------------------------------------------
{
	//
	// Store the ID for the inPosition within the shader
	gl_getAttribVariable ( whichShader, ( char * ) "inPosition", gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inVertsID );
	//
	// Store the ID for the inPosition within the shader
	gl_getAttribVariable ( whichShader, ( char * ) "inNormal", gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inNormalsID );
	//
	// Store the ID for the texture coords for the shader
	gl_getAttribVariable ( whichShader, ( char * ) "inTextureCoords",  gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inTextureCoordsID );
	gl_getAttribVariable ( whichShader, ( char * ) "inTextureCoords1", gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inTextureCoordsID_1 );
	//
	// Get standard Uniform locations and cache them
	gl_getUniformVariable ( whichShader, ( char * ) "inTexture0", gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inTextureUnit );
	gl_getUniformVariable ( whichShader, ( char * ) "inTexture1", gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inTextureUnit_1 );
	//
	// Matrix locations
	gl_getUniformVariable ( whichShader,  (char * ) "u_viewProjectionMat", gl_getShaderName ( whichShader ), 	&shaderProgram[whichShader].viewProjectionMat);
	gl_getUniformVariable ( whichShader, ( char * ) "u_modelMat",          gl_getShaderName ( whichShader ), 	&shaderProgram[whichShader].modelMat );

	//
	// Do variables for each shader unique to each one
	switch ( whichShader )
		{
			case SHADER_TTF_FONT:
				//
				// Get the ID to store the colorkey value in
				gl_getAttribVariable ( whichShader, ( char * ) "fontColor", gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inColorID );
				
//				gl_getUniformVariable (whichShader, ( char *) "inScreenSize", gl_getShaderName (whichShader), &shaderProgram[whichShader].screenSizeID );
				break;

			case SHADER_PHYSIC_DEBUG:
				//
				// Get the ID to store the vertex color value in
				gl_getAttribVariable ( whichShader, ( char * ) "debugColor", gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inColorID );
				break;

			case SHADER_DIR_LIGHT:
				break;

			case SHADER_GEOMETRY_PASS:
				break;

			case SHADER_POINT_LIGHT:
				break;

			case SHADER_MODEL_PASS:
//				gl_getAttribVariable ( whichShader, ( char * ) "color", gl_getShaderName ( whichShader ), &shaderProgram[whichShader].inColorID );
				break;

			default:
				break;
		}

	return true;
}
//-----------------------------------------------------------------------------
//
// Load the shader from the text file into memory and compile
// Return shaderObject ID as param
bool gl_loadIntoMemory(string fileName, GLuint *returnObject_ID, int shaderType)
//-----------------------------------------------------------------------------
{
	GLint   compiled;
	GLint fileLength;
	GLuint shaderObject_ID;
	char *sourceText = NULL;

	fileLength = io_getFileSize ( (char *)fileName.c_str());
	if (fileLength == -1)
		return false;

	sourceText = (char *) malloc (sizeof (char) * (fileLength + 1));
	if ( nullptr == sourceText)
		return false;

	if (-1 == io_getFileIntoMemory ((char*)fileName.c_str(), sourceText))
	{
		free(sourceText);
		return false;
	}
	//
	// Make sure it's null terminated   // Need fileLength + 1 ??
	sourceText[fileLength] = '\0';

	switch (shaderType)
	{
		case GL_VERTEX_SHADER:
			shaderObject_ID = glCreateShader(GL_VERTEX_SHADER);
			break;

		case GL_FRAGMENT_SHADER:
			shaderObject_ID = glCreateShader(GL_FRAGMENT_SHADER);
			break;

		case GL_GEOMETRY_SHADER:
			shaderObject_ID = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		default:
			con_print(CON_ERROR, true, "Invalid shader type passed.");
			return false;
			break;
	}
	if (shaderObject_ID == 0)
	{
		con_print(CON_ERROR, true, "Error: Failed to create shader ID.");
		return false;
	}

	glShaderSource ( shaderObject_ID, 1, (const GLchar**)&sourceText, nullptr) ;

	glCompileShader ( shaderObject_ID ) ;
	//
	// Check it compiled ok
	glGetShaderiv ( shaderObject_ID, GL_COMPILE_STATUS, &compiled );

	if ( compiled )
		con_print ( CON_TEXT, true, "Shader compiled ok - [ %s ]", fileName.c_str() );
	else
	{
		con_print ( CON_TEXT, true, "Shader compile failed [ %s]", fileName.c_str() );
		gl_getGLSLError ( shaderObject_ID, GLSL_SHADER );
		return false;
	}

	free(sourceText);
	*returnObject_ID = shaderObject_ID;

	return true;
}
//-----------------------------------------------------------------------------
//
// Load and compile shaders
bool gl_loadCompileShaders ( int programObject )
//-----------------------------------------------------------------------------
{
	GLint       linked;
	GLuint       vertexShaderObject, fragmentShaderObject, geometryShaderObject;

	con_print ( CON_TEXT, true, "-----------------------------------------------------------------------" );

	//
	// Load the shaders and set their object ID
	if ( !gl_loadIntoMemory(shaderProgram[programObject].vertFileName, &vertexShaderObject, GL_VERTEX_SHADER) ||
	   !gl_loadIntoMemory(shaderProgram[programObject].fragFileName, &fragmentShaderObject, GL_FRAGMENT_SHADER))
	{
		con_print(CON_ERROR, true, "Error loading and compiling shader.");
		return false;
	}
	if (strlen(shaderProgram[programObject].geomFileName) > 1)

		if (!gl_loadIntoMemory(shaderProgram[programObject].geomFileName, &geometryShaderObject, GL_GEOMETRY_SHADER))
		{
			con_print(CON_ERROR, true, "Error loading and compiling Geometry shader.");
			return false;
		}
	//
	// Create the program ID
	shaderProgram[programObject].programID = glCreateProgram();
	//
	// Attach the object ID to the program ID ready for compiling
	GL_CHECK ( glAttachShader ( shaderProgram[programObject].programID, vertexShaderObject ) );
	GL_CHECK ( glAttachShader ( shaderProgram[programObject].programID, fragmentShaderObject ) );

	if (strlen(shaderProgram[programObject].geomFileName) > 1)
	{
		GL_CHECK ( glAttachShader (shaderProgram[programObject].programID, geometryShaderObject ) );
	}
	//
	// Link the shaders
	GL_CHECK ( glLinkProgram ( shaderProgram[programObject].programID ) );
	//
	// Get the status
	GL_CHECK ( glGetProgramiv ( shaderProgram[programObject].programID, GL_LINK_STATUS, &linked ) );

	if ( linked )   // true
		{
			con_print ( CON_TEXT, true, "INFO: Shader linked ok - [ %s ]", shaderProgram[programObject].fragFileName );
			shaderProgram[programObject].linkedOK = true;
		}
	else
		{
			con_print ( CON_TEXT, true, "ERROR: Shader failed to link - [ %s ]", shaderProgram[programObject].fragFileName );
			gl_getGLSLError ( shaderProgram[programObject].programID, GLSL_PROGRAM );
			return false;
		}

	//
	// Needed to avoid error in glUniform in gl_setShaderVars
	GL_CHECK ( glUseProgram ( shaderProgram[programObject].programID ) );
	//
	// Setup variables for this shader
	gl_setShaderVars ( programObject );

	con_print ( CON_TEXT, true, "-----------------------------------------------------------------------" );

	return true;
}

//GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[SHADER_SHADOW_LIGHTING].programID, "shadows" ), 1 ) );

//glUniform1i(shaderGetLocation(whichShader,"shadows"), 1);

//-----------------------------------------------------------------------------
//
// Set the value of a shader uniform or attribute
//
// If variable is not found, still record, but mark has invalid - count how many
// times it is called.
GLint shaderGetLocation ( int whichShader, string keyName )
//-----------------------------------------------------------------------------
{
	string u_keyName;
	GLint keyLocation = 0;
	_shaderVarType      varInfoType;

	u_keyName = to_string (whichShader) + keyName;

	shaderItr = shaderValues.find(u_keyName);
	if (shaderItr == shaderValues.end())   // Key doesn't exist
	{
		con_print(CON_INFO, true, "Key [ %s ] doesn't exist in map.", u_keyName.c_str());

		keyLocation = glGetUniformLocation (shaderProgram[whichShader].programID, keyName.c_str());
		if (keyLocation == 0)
		{
			con_print(CON_INFO, true, "keyName wasn't found in shader using getUniformLocation [ %s - %i ]", keyName.c_str(), whichShader);
			keyLocation = glGetAttribLocation (shaderProgram[whichShader].programID, keyName.c_str ());
		}

		if (keyLocation == -1)
		{
			con_print(CON_ERROR, true, "Shader [ %i ] Value [ %s ] does not exist.", whichShader, keyName.c_str());
			varInfoType.locationID = keyLocation;
			varInfoType.refCount = 0;
			varInfoType.isFound = false;
			shaderValues.insert(std::pair<string, _shaderVarType>(u_keyName, varInfoType));
			return 0;
		}
//		con_print(CON_INFO, true, "Caching value for [ %s - %i ]. Shader [ %i ]", keyName.c_str(), keyLocation, whichShader);

		varInfoType.locationID = keyLocation;
		varInfoType.refCount = 0;
		varInfoType.isFound = true;

		shaderValues.insert(std::pair<string, _shaderVarType>(u_keyName, varInfoType));
		return keyLocation;
	}
	else
	{
		if (shaderItr->second.isFound)
		{
			shaderItr->second.refCount++;
//		con_print(CON_INFO, true, "Got key [ %s ] from map. refCount [ %i ]", keyName.c_str(), shaderItr->second.refCount);
			return shaderItr->second.locationID;
		}
		else
		{
			shaderItr->second.refCount++;
			return -1;  // Count how many times it was called, and not found
		}
	}
}