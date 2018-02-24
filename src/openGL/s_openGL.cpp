#include "s_globals.h"
#include "s_fontUtil.h"
#include "s_openGL.h"
#include "s_loadImage.h"
#include "io_fileSystem.h"
#include "s_bmpFont.h"
#include "s_window.h"
#include "s_ttfFont.h"
#include "s_shaders.h"
#include "s_maths.h"
#include "s_camera.h"

glm::mat4       projMatrix;
glm::mat4       modelMatrix;
glm::mat4       viewMatrix;

//--------------------------------------------------------------------------------------------
//
// Draw a debug line to see the picking ray
void drawDebugLine( glm::vec3 startPoint, glm::vec3 endPoint, glm::vec3 pos, int drawType, float length, bool reinit, GLfloat scaleBy )
//--------------------------------------------------------------------------------------------
{
	GLuint			lineVAO, lineVBO, lineColorVBO;
	glm::vec3		lineCoords[2];

	static bool initDone = false;

	if ( true == reinit )
		initDone = false;

	if ( false == initDone )
		{
			if (DRAW_LINE == drawType)
				{
					lineCoords[0] = startPoint;
					lineCoords[1] = endPoint;

				}

			else if (DRAW_RAY == drawType)
				{
					lineCoords[0] = startPoint;
					endPoint = glm::normalize(endPoint);
					lineCoords[1] = startPoint + (length * endPoint);
				}

			GL_ASSERT(glUseProgram( shaderProgram[SHADER_COLOR].programID ));

			GL_ASSERT(glGenVertexArrays( 1, &lineVAO ));
			GL_CHECK(glBindVertexArray( lineVAO ));

			GL_ASSERT(glGenBuffers( 1, &lineVBO ));
			GL_CHECK(glBindBuffer( GL_ARRAY_BUFFER, lineVBO ));		// Allocate space and upload from CPU to GPU
			GL_CHECK(glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * 2, &lineCoords, GL_STATIC_DRAW ));
			GL_CHECK(glVertexAttribPointer( shaderProgram[SHADER_COLOR].inVertsID, 3, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 )));
			GL_CHECK(glEnableVertexAttribArray ( shaderProgram[SHADER_COLOR].inVertsID ));
		}

//    if (-1.0f == scaleBy)
//        scaleFactor = meshModels[whichModel].scaleFactor;
//    else
//        scaleFactor = ;

	glm::mat4   scaleMatrix;
	int whichMesh = 0;
	//
	// Adjust the size and position of the mesh
	//
	// Adjust the size and position of the mesh
	scaleMatrix = glm::scale(glm::translate(modelMatrix, pos), glm::vec3(scaleBy, scaleBy, scaleBy));

	gl_set3DMode();
	cam_look(camPosition, camDirection);
	modelMatrix = glm::mat4();

	GL_CHECK(glUseProgram(shaderProgram[SHADER_COLOR].programID));

	GL_CHECK(glUniformMatrix4fv(shaderProgram[SHADER_COLOR].modelMat, 1, false, glm::value_ptr(scaleMatrix)));
	GL_CHECK(glUniformMatrix4fv(shaderProgram[SHADER_COLOR].viewProjectionMat, 1, false, glm::value_ptr(projMatrix * viewMatrix)));

	GL_CHECK(glBindVertexArray ( lineVAO ));
	GL_CHECK(glEnableVertexAttribArray ( shaderProgram[SHADER_COLOR].inVertsID ));

	GL_CHECK(glDrawArrays( GL_LINES, 0, 2 ));

	glUseProgram ( 0 );
	glBindVertexArray ( 0 );
}

//-----------------------------------------------------------------------------
//
// Get all the openGL errors
void gl_getAllGLErrors ( int errorNum, const char *calledFrom, int line )
//-----------------------------------------------------------------------------
{
	if (NULL == errorNum)
		{
			errorNum = glGetError();
		}

	while ( errorNum != GL_NO_ERROR )
		{
			con_print (CON_TEXT,  true, "OpenGL Error: [ %i ] - [ %i ] - [ %s ]", line, errorNum, calledFrom);

			switch ( errorNum )
				{
					case GL_INVALID_ENUM:
						con_print (CON_TEXT,  true, "[ %s ]", "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument." );
						break;

					case GL_INVALID_VALUE:
						con_print (CON_TEXT,  true, "[ %s ]", "GL_INVALID_VALUE: A numeric argument is out of range." );
						break;

					case GL_INVALID_OPERATION:
						con_print (CON_TEXT,  true, "[ %s ]", "GL_INVALID_OPERATION: The specified operation is not allowed in the current state." );
						break;

					case GL_OUT_OF_MEMORY:
						con_print (CON_TEXT,  true, "[ %s ]", "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command." );
						break;

					default:
						con_print (CON_TEXT,  true, "[ %s ]", "Unknown error." );
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
	projMatrix = ortho<float> ( 0.0, winWidth, winHeight, 0.0, 1.0, -1.0 );
	viewMatrix = glm::mat4();
}

//-----------------------------------------------------------------------------
//
// Set matrix to 3d perspective modee
void gl_set3DMode()
//-----------------------------------------------------------------------------
{
//    projMatrix = glm::perspective ( 60.0f, 4.0f / 3.0f, 0.01f, 1000.0f );
	projMatrix = glm::perspective ( 60.0f, (float)winWidth / (float)winHeight, 0.01f, 1000.0f ); // *** These values are also in the depthMap.frag
//    modelMatrix = glm::mat4();
//    viewMatrix = glm::mat4();
}


//-----------------------------------------------------------------------------
//
// Draw the console screen
void gl_drawScreen ( bool updateNow )
//-----------------------------------------------------------------------------
{
	_conLine	conTempLine;
	float       conStartY = 0.0f;

	conStartY = winHeight - ( conFontSize * 2 );

	if ( true == updateNow )
		{
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
//        glLoadIdentity();									// Reset The matrix
		}

	for ( int i = 0; i != ( winHeight / conFontSize ) - 2; i++ )
		{
			gl_setFontColor( conLines[i].conLineColor.red, conLines[i].conLineColor.green, conLines[i].conLineColor.blue, conLines[i].conLineColor.alpha );
//        fontColor = conLines[i].conLineColor;
			ttf_addText ( FONT_SMALL, 0.0f, ( conStartY - ( ( i * conFontSize ) + conFontSize ) ), "%s", conLines[i].conLine.c_str() );
		}

	if ( false == updateNow )
		{
			conTempLine.conLine = conCurrentLine.conLine;

			if ( true == conCursorIsOn )
				conTempLine.conLine += "_";

			else
				conTempLine.conLine += " ";

			ttf_addText ( FONT_SMALL, 0.0f, ( GLfloat ) ( winHeight - ( conFontSize * 2 ) ), "%s", conTempLine.conLine.c_str() );
		}

	if ( true == updateNow )
		{
			lib_swapBuffers();
		}
}
