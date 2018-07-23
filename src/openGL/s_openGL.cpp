#include "s_globals.h"
#include "s_fontUtil.h"
#include "s_openGL.h"
#include "io_loadImage.h"
#include "io_fileSystem.h"
#include "s_bmpFont.h"
#include "s_window.h"
#include "s_ttfFont.h"
#include "s_shaders.h"
#include "s_maths.h"
#include "s_camera.h"

#include <map>
#include <hdr/openGL/s_openGLWrap.h>

typedef struct
{
	int counter{};
	int errorID{};
	string  source;
	string  type;
	string  severity;
	string  message;
} _errorMessage;

std::map <int, _errorMessage> shaderErrorsMap;
std::map <int, _errorMessage>::iterator it;

glm::mat4       projMatrix;
glm::mat4       modelMatrix;
glm::mat4       viewMatrix;

bool			g_debugOpenGL = true;

//--------------------------------------------------------------------------------------------
//
// OpenGL Debug - return string of type source
//
//Parsing code from OpenGL Shader Language CookBook SE
//https://github.com/daw42/glslcookbook
//
std::string getStringForType ( GLenum type )
//--------------------------------------------------------------------------------------------
{
	switch ( type )
		{
		case GL_DEBUG_TYPE_ERROR:
			return"Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			return "Deprecated behavior";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			return "Undefined behavior";
		case GL_DEBUG_TYPE_PORTABILITY:
			return "Portability issue";
		case GL_DEBUG_TYPE_PERFORMANCE:
			return "Performance issue";
		case GL_DEBUG_TYPE_MARKER:
			return "Stream annotation";
		case GL_DEBUG_TYPE_OTHER:
			return "Other";
		default:
			assert ( false );
			return "";
		}
}

//--------------------------------------------------------------------------------------------
//
// OpenGL Debug - return string of error source
std::string getStringForSource ( GLenum source )
//--------------------------------------------------------------------------------------------
{
	switch ( source )
		{
		case GL_DEBUG_SOURCE_API:
			return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			return "Window system";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			return "Shader compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			return "Third party";
		case GL_DEBUG_SOURCE_APPLICATION:
			return "Application";
		case GL_DEBUG_SOURCE_OTHER:
			return "Other";
		default:
			assert ( false );
			return "";
		}
}

//--------------------------------------------------------------------------------------------
//
// OpenGL Debug - get string for severity
std::string getStringForSeverity ( GLenum severity )
//--------------------------------------------------------------------------------------------
{
	switch ( severity )
		{
		case GL_DEBUG_SEVERITY_HIGH:
			return "High";
		case GL_DEBUG_SEVERITY_MEDIUM:
			return "Medium";
		case GL_DEBUG_SEVERITY_LOW:
			return "Low";
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			return "Notification";
		default:
			assert ( false );
			return ( "" );
		}
}

//--------------------------------------------------------------------------------------------
//
// Print out all the GLSL error messages
void gl_displayErrors()
//--------------------------------------------------------------------------------------------
{
	con_print(CON_INFO, true, "---------------------------------------------------");
	for (it = shaderErrorsMap.begin(); it != shaderErrorsMap.end(); it++)
	{
		con_print(CON_INFO, true, "Error count [ %i ]", it->second.counter);
		con_print(CON_INFO, true, "Error ID [ %i ]", it->first);
		con_print(CON_INFO, true, "Source [ %s ]", it->second.source.c_str());
		con_print(CON_INFO, true, "Type [ %s ]", it->second.type.c_str());
		con_print(CON_INFO, true, "Severity [ %s ]", it->second.severity.c_str());
		con_print(CON_INFO, true, "Message [ %s ]", it->second.message.c_str());
		con_print(CON_INFO, true, "---------------------------------------------------");
		con_print(CON_INFO, true, "");
	}
}
//--------------------------------------------------------------------------------------------
//
// OpenGL Debug Callback
void gl_DebugCallback ( GLenum source, GLenum type, GLenum id, GLenum severity,
                        GLsizei length, const GLchar *msg, const void *data )
//--------------------------------------------------------------------------------------------
{
	if (false == g_debugOpenGL)
		return;

	it = shaderErrorsMap.find(id);
	if (it != shaderErrorsMap.end())
	{
		it->second.counter++;
	}
	else
	{
		_errorMessage    errMessage;

		errMessage.counter = 1;
		errMessage.source = getStringForSource (source);
		errMessage.type = getStringForType (type);
		errMessage.severity = getStringForSeverity (severity);
		errMessage.message = msg;

		shaderErrorsMap.insert ( std::make_pair( id, errMessage) );

		con_print (CON_ERROR, true, "ID [ %i ]", id);
		con_print (CON_ERROR, true, "source [ %s ]", getStringForSource (source).c_str ());
		con_print (CON_ERROR, true, "type [ %s ]", getStringForType (type).c_str ());
		con_print (CON_ERROR, true, "severity [ %s ]", getStringForSeverity (severity).c_str ());
		con_print (CON_ERROR, true, "debug call [ %s ]", msg);
	}
}

//--------------------------------------------------------------------------------------------
//
// OpenGL Debug - Register callback
void gl_registerDebugCallback()
//--------------------------------------------------------------------------------------------
{
	glEnable ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback ( gl_DebugCallback, nullptr);
	glDebugMessageControl ( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE );
}

//--------------------------------------------------------------------------------------------
//
// Draw a debug line
void drawDebugLine ( glm::vec3 startPoint, glm::vec3 endPoint, glm::vec3 pos, int drawType, float length, bool reinit, GLfloat scaleBy )
//--------------------------------------------------------------------------------------------
{
	static GLuint			lineVAO = -1;
	static GLuint			lineVBO = -1;
	static bool				initDone = false;
	
	glm::vec3		lineCoords[2];

	glm::mat4 		scaleMatrix;

	if (false == initDone )
		{
			GL_ASSERT ( glGenVertexArrays 	( 1, &lineVAO ) );
			//GL_ASSERT ( glGenBuffers 		( 1, &lineVBO ) );
			lineVBO = wrapglGenBuffers(1, __func__);
			initDone = true;
		}

	switch ( drawType )
		{
		case DRAW_LINE:
			lineCoords[0] = startPoint;
			lineCoords[1] = endPoint;
			break;

		case DRAW_RAY:
			lineCoords[0] = startPoint;
			endPoint = glm::normalize ( endPoint );
			lineCoords[1] = startPoint + ( length * endPoint );
			break;
		}

	GL_ASSERT ( glUseProgram ( shaderProgram[SHADER_COLOR].programID ) );
	GL_CHECK ( glBindVertexArray ( lineVAO ) );

	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, lineVBO ) );		// Allocate space and upload from CPU to GPU

	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( glm::vec3 ) * 2, &lineCoords,  GL_DYNAMIC_DRAW ) );
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[SHADER_COLOR].inVertsID, 3, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_COLOR].inVertsID ) );

	//
	// Adjust the size and position of the line
	scaleMatrix = glm::scale ( glm::translate ( modelMatrix, pos ), glm::vec3 ( scaleBy, scaleBy, scaleBy ) );

	GL_CHECK ( glUseProgram ( shaderProgram[SHADER_COLOR].programID ) );

	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[SHADER_COLOR].modelMat, 1, false, glm::value_ptr ( scaleMatrix ) ) );
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[SHADER_COLOR].viewProjectionMat, 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );

	GL_CHECK ( glBindVertexArray ( lineVAO ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_COLOR].inVertsID ) );

	GL_CHECK ( glDrawArrays ( GL_LINES, 0, 2 ) );

	glUseProgram ( 0 );
	glBindVertexArray ( 0 );

//	glDeleteVertexArrays ( 1, &lineVAO );
//	glDeleteBuffers ( 1, &lineVBO );
}

//-----------------------------------------------------------------------------
//
// Get all the openGL errors
void gl_getAllGLErrors ( int errorNum, const char *calledFrom, int line )
//-----------------------------------------------------------------------------
{
	errorNum = glGetError();

	while ( errorNum != GL_NO_ERROR )
		{
			con_print ( CON_TEXT,  true, "OpenGL Error: [ %i ] - [ %i ] - [ %s ]", line, errorNum, calledFrom );

			switch ( errorNum )
				{
				case GL_INVALID_ENUM:
					con_print ( CON_TEXT,  true, "[ %s ]", "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument." );
					break;

				case GL_INVALID_VALUE:
					con_print ( CON_TEXT,  true, "[ %s ]", "GL_INVALID_VALUE: A numeric argument is out of range." );
					break;

				case GL_INVALID_OPERATION:
					con_print ( CON_TEXT,  true, "[ %s ]", "GL_INVALID_OPERATION: The specified operation is not allowed in the current state." );
					break;

				case GL_OUT_OF_MEMORY:
					con_print ( CON_TEXT,  true, "[ %s ]", "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command." );
					break;

				default:
					con_print ( CON_TEXT,  true, "[ %s ]", "Unknown error." );
					break;
				}

			errorNum = glGetError();
		}
}

//-----------------------------------------------------------------------------
//
// Set OpenGL to 2d orthographic mode
void gl_set2DMode()
//-----------------------------------------------------------------------------
{
	projMatrix = ortho<float> ( 0.0, (float)winWidth, 0.0f, (float)winHeight, 1.0, -1.0 );	// upside down
//	projMatrix = glm::ortho<float> ( 0.0, (float)winWidth, (float)winHeight, 0.0, 1.0, -1.0 );
//	projMatrix = glm::ortho<float> ( -(winWidth / 2.0f), winWidth / 2.0f, winHeight / 2.0f, -(winHeight / 2.0f), -1.0f, 1.0f);
}

//-----------------------------------------------------------------------------
//
// Set matrix to 3d perspective mode
void gl_set3DMode()
//-----------------------------------------------------------------------------
{
	projMatrix = glm::perspective ( 60.0f, ( float ) winWidth / ( float ) winHeight, 0.01f, 1000.0f ); // *** These values are also in the depthMap.frag
}

//-----------------------------------------------------------------------------
//
// Create a checkboard pattern texture
// From: https://stackoverflow.com/questions/3569261/multiple-textures-in-glsl-only-one-works
/* adapted from the red book */
GLuint gl_makeCheckTex(int textureSize)
//-----------------------------------------------------------------------------
{
	GLubyte image[textureSize][textureSize][4]; // RGBA storage

	for (int i = 0; i < textureSize; i++)
	{
		for (int j = 0; j < textureSize; j++)
		{
			int c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0))*255;
			image[i][j][0]  = (GLubyte)c;
			image[i][j][1]  = (GLubyte)c;
			image[i][j][2]  = (GLubyte)c;
			image[i][j][3]  = (GLubyte)255;
		}
	}

	GLuint texName;
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize, textureSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	return texName;
}

//-----------------------------------------------------------------------------
//
// Draw a 2D quad
void gl_draw2DQuad(glm::vec2 position, glm::vec2 quadSize, int whichShader, GLuint whichTexture, bool showDepthMap)
//-----------------------------------------------------------------------------
{
	glm::vec2			quadVerts[4];
	static GLuint		vao = 0;
	static GLuint		buffers[2];
	static bool			initDone = false;

	quadVerts[0].x = position.x;
	quadVerts[0].y = position.y;

	quadVerts[1].x = position.x;
	quadVerts[1].y = position.y + quadSize.y;

	quadVerts[2].x = position.x + quadSize.x;
	quadVerts[2].y = position.y + quadSize.y;

	quadVerts[3].x = position.x + quadSize.x;
	quadVerts[3].y = position.y;

	GLfloat quadTexCoords[] =
			{
					0.0, 0.0,
					0.0, 1.0,
					1.0, 1.0,
					1.0, 0.0,
			};

	if ( !initDone )
	{
		// create the VAO
		GL_ASSERT ( glGenVertexArrays ( 1, &vao ) );
		GL_CHECK ( glBindVertexArray ( vao ) );

		// Create buffers for the vertex data
		buffers[0] = wrapglGenBuffers(1, __func__);
		buffers[1] = wrapglGenBuffers(1, __func__);

		GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );

		// Vertex coordinates buffer
		GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, buffers[0] ) );
		GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( quadVerts ), quadVerts, GL_DYNAMIC_DRAW ) );
		GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
		GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inVertsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) ) );

		// Texture coordinates buffer
		GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, buffers[1] ) );
		GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( quadTexCoords ), quadTexCoords, GL_DYNAMIC_DRAW ) );
		GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
		GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) ) );

		initDone = false;
	}

	GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );
	//
	// Bind texture if it's not already bound as current texture
	GL_CHECK ( glActiveTexture ( GL_TEXTURE0 ) );

	if (false == showDepthMap)
		GL_CHECK ( glBindTexture ( GL_TEXTURE_2D, whichTexture ) );
	else
		GL_CHECK ( glBindTexture ( GL_TEXTURE_CUBE_MAP, whichTexture ) );

	GL_CHECK ( glUniform1i ( shaderProgram[whichShader].inTextureUnit, 0 ) );

	GL_CHECK ( glUniform2f ( shaderProgram[whichShader].screenSizeID, (float)winWidth / 2, (float)winHeight / 2));

	GL_CHECK ( glUniform1i  ( glGetUniformLocation ( shaderProgram[whichShader].programID, "showDepthMap" ), showDepthMap ) );

	GL_CHECK ( glBindVertexArray ( vao  ) );
	//
	// Enable attribute to hold vertex information
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );

	GL_CHECK ( glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 ) );

	glDeleteBuffers(2, buffers);
	glDeleteVertexArrays(1, &vao);
}