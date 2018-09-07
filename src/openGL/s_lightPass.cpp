#include "s_globals.h"
#include "s_camera3.h"
#include "s_openGLWrap.h"
#include "s_shaders.h"
#include "s_lightPass.h"
#include "s_defRender.h"

#include "io_textures.h"
#include "s_shadowMap.h"
#include "s_render.h"
#include "s_entitiesBSP.h"
#include "s_shaderLights.h"

glm::vec3   cubeVerts[8];

unsigned int faceIndex[] =
{
	0,1,2,0,2,3,
	4,5,6,4,6,7,
	2,5,4,2,4,3,
	2,1,6,2,5,6,
	0,6,1,0,7,6,
	3,0,7,3,7,4
};

//-----------------------------------------------------------------------------
//
// Render all the light spheres into the GBuffer
void lt_renderPointLights ( int whichShader )
//-----------------------------------------------------------------------------
{
	//
	// Next, we render all the point light sources.
	// We will be doing our own depth testing in frag shader, so disable depth testing.
	// Enable alpha blending. So that the rendered point lights are added to the frame buffer.
	//
	GL_CHECK ( glDisable ( GL_DEPTH_TEST ) );
	GL_CHECK ( glEnable ( GL_BLEND ) );
//	GL_CHECK ( glBlendFunc ( GL_ONE, GL_ONE ) );
	GL_CHECK ( glBlendFunc ( GL_SRC_ALPHA, GL_ONE ) );

	// We render only the inner faces of the light sphere.
	// In other words, we render the back-faces and not the front-faces of the sphere.
	// If we render the front-faces, the lighting of the light sphere disappears if
	// we are inside the sphere, which is weird. But by rendering the back-faces instead,
	// we solve this problem.

//	GL_CHECK ( glFrontFace ( GL_CW ) );

	for ( int i = 0; i < numOfLights + MAX_NUM_BULLETS; i++ )
		{
			if ( allLights[i].active )
				{
					ass_renderMeshVec3Position ( MODEL_SPHERE,  whichShader, allLights[i].position, allLights[i].attenuation * 100, allLights[i].color );

					if (g_debugLightPos)
						ass_renderMeshVec3Position ( MODEL_CRATE,  whichShader, allLights[i].position, 5.0f, glm::vec3{1.0f, 1.0f, 1.0f});
				}
		}

	GL_CHECK ( glEnable ( GL_DEPTH_TEST ) );
	GL_CHECK ( glDisable ( GL_BLEND ) );
	GL_CHECK ( glFrontFace ( GL_CCW ) );
}

//-----------------------------------------------------------------------------
//
// Render a full screen quad
void lt_renderFullscreenQuad ( int whichShader )
//-----------------------------------------------------------------------------
{

	glm::vec2			quadVerts[4];
	static GLuint		vao = -1;
	static GLuint		buffers[2];
	static bool			initDone = false;
	
	quadVerts[0].x = -1.0f;
	quadVerts[0].y = -1.0f;

	quadVerts[1].x = 1.0;
	quadVerts[1].y = -1.0f;

	quadVerts[2].x = 1.0;
	quadVerts[2].y = 1.0;

	quadVerts[3].x = -1.0f;
	quadVerts[3].y = 1.0;

	GLfloat quadTexCoords[] =
	{
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
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
			
			initDone = true;
		}

	GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );

	//
	// Bind texture if it's not already bound as current texture
	GL_CHECK ( glActiveTexture ( GL_TEXTURE0 + 0 ) );
	GL_CHECK ( glBindTexture ( GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_POSITION] ) );
	GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "tPosition" ), 0 ) );

	GL_CHECK ( glActiveTexture ( GL_TEXTURE0 + 1 ) );
	GL_CHECK ( glBindTexture ( GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_NORMAL] ) );
	GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "tNormals" ), 1 ) );

	GL_CHECK ( glActiveTexture ( GL_TEXTURE0 + 2 ) );
	GL_CHECK ( glBindTexture ( GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_DIFFUSE] ) );
	GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "tDiffuse" ), 2 ) );

	GL_CHECK ( glUniform3f ( glGetUniformLocation ( shaderProgram[whichShader].programID, "cameraPosition" ), cam3_Position.x, cam3_Position.y, cam3_Position.z ) );

	GL_CHECK ( glBindVertexArray ( vao  ) );
	//
	// Enable attribute to hold vertex information
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );

	GL_CHECK ( glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 ) );
}

//-----------------------------------------------------------------------------
//
// Render a full screen quad
void lt_renderDepthQuad ( int whichShader )
//-----------------------------------------------------------------------------
{
	GLfloat quadVertsDepthMap[] =
	{
		0.0,        0.0,
		(GLfloat)winWidth,     0.0,
		(GLfloat)winWidth,    (GLfloat)winHeight,
		0.0,         (GLfloat)winHeight,
	};

	GLfloat quadTexCoordsDepthMap[] =
	{
		0.0,      1.0,
		1.0,      1.0,
		1.0,      0.0,
		0.0,      0.0,
	};

	static bool initDone = false;
	static GLuint vboQuadDepthMap, vboQuadTexDepthMap, vaoQuadDepthMap;

	if ( !initDone )
		{
			//
			// Enable the shader program
			GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );

			GL_ASSERT ( glGenVertexArrays ( 1, &vaoQuadDepthMap ) );
			GL_CHECK ( glBindVertexArray ( vaoQuadDepthMap ) );

			vboQuadDepthMap = wrapglGenBuffers(1, __func__);
			//
			// Use Vertices
			GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, vboQuadDepthMap ) );
			GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( GLfloat ) * 8, &quadVertsDepthMap, GL_STATIC_DRAW ) );
			GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
			GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inVertsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
			GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, 0 ) );

			vboQuadTexDepthMap = wrapglGenBuffers(1, __func__);
			//
			// Use texture coords
			GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, vboQuadTexDepthMap ) );
			GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( GLfloat ) * 8, &quadTexCoordsDepthMap, GL_STATIC_DRAW ) );
			GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
			GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
			GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, 0 ) );

			glBindVertexArray ( 0 );
			glUseProgram ( 0 );
			initDone = true;
		}

	//
	// Draw in ortho mode
	gl_set2DMode();

	modelMatrix = glm::translate ( glm::mat4(), glm::vec3 ( 0.0, 0.0, 1.0 ) );
	glm::mat4   mvpMatrix = projMatrix * modelMatrix;

	//
	// Use the program to make the attributes available
	GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );
	//
	// Bind texture if it's not already bound as current texture
	wrapglBindTexture ( GL_TEXTURE0, gl_returnDepthTexID() );

	GL_CHECK ( glUniform1i ( shaderProgram[whichShader].inTextureUnit, 0 ) );
	//
	// Load the matrixes into the vertex shader
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr ( mvpMatrix ) ) );

	GL_CHECK ( glBindVertexArray ( vaoQuadDepthMap ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
	//
	// Draw a triangle
	GL_CHECK ( glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 ) );

	glDisableVertexAttribArray ( shaderProgram[whichShader].inVertsID );
	glDisableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID );

	glBindVertexArray ( 0 );
	glUseProgram ( 0 );

	cam3_createProjMatrix ();
}


//-----------------------------------------------------------------------------
//
// Generate verts for a cube
void generateCubeVerts ( float cubeSize, glm::vec3 position )
//-----------------------------------------------------------------------------
{
	float halfSize = cubeSize / 2;

	cubeVerts[0].x = position[0] - halfSize;
	cubeVerts[0].y = position[1] + halfSize;
	cubeVerts[0].z = position[2] - halfSize;
	cubeVerts[1].x = position[0] - halfSize;
	cubeVerts[1].y = position[1] - halfSize;
	cubeVerts[1].z = position[2] - halfSize;
	cubeVerts[2].x = position[0] + halfSize;
	cubeVerts[2].y = position[1] - halfSize;
	cubeVerts[2].z = position[2] - halfSize;
	cubeVerts[3].x = position[0] + halfSize;
	cubeVerts[3].y = position[1] + halfSize;
	cubeVerts[3].z = position[2] - halfSize;

	cubeVerts[4].x = position[0] + halfSize;
	cubeVerts[4].y = position[1] + halfSize;
	cubeVerts[4].z = position[2] + halfSize;
	cubeVerts[5].x = position[0] + halfSize;
	cubeVerts[5].y = position[1] + halfSize;
	cubeVerts[5].z = position[2] + halfSize;
	cubeVerts[6].x = position[0] - halfSize;
	cubeVerts[6].y = position[1] - halfSize;
	cubeVerts[6].z = position[2] + halfSize;
	cubeVerts[7].x = position[0] - halfSize;
	cubeVerts[7].y = position[1] + halfSize;
	cubeVerts[7].z = position[2] + halfSize;
}

//-----------------------------------------------------------------------------
//
// Draw position of the light
void drawLightPos ( int whichShader, glm::vec3 position )
//-----------------------------------------------------------------------------
{
	static GLuint           vao;
	int                     faceCount = 12;
	static GLuint           buffers[2];
	static bool             initDone = false;

	if ( false == initDone )
		{
			generateCubeVerts ( 5.0f, position );

			// create the VAO
			GL_ASSERT ( glGenVertexArrays ( 1, &vao ) );
			GL_CHECK ( glBindVertexArray ( vao ) );
			//
			// create buffers for our vertex data
			//GL_ASSERT ( glGenBuffers ( 2, buffers ) );
			buffers[0] = wrapglGenBuffers(1, __func__);
			buffers[1] = wrapglGenBuffers(1, __func__);

			GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );
			//
			//vertex coordinates buffer
			GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, buffers[0] ) );
			GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, sizeof ( cubeVerts ), cubeVerts, GL_STATIC_DRAW ) );
			GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
			GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inVertsID,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET ( 0 ) ) );
			//
			//index buffer
			GL_CHECK ( glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, buffers[1] ) );
			GL_CHECK ( glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( faceIndex ), faceIndex, GL_STATIC_DRAW ) );
			//
			// unbind the VAO
			glBindVertexArray ( 0 );

			initDone = true;
		}

	modelMatrix = glm::mat4();

	GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );

	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr ( modelMatrix ) ) );
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );

	GL_CHECK ( glBindVertexArray ( vao ) );
	//
	// Enable attribute to hold vertex information
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );

	GL_CHECK ( glDrawElements ( GL_TRIANGLES, faceCount*3, GL_UNSIGNED_INT, 0 ) );

	glUseProgram ( 0 );
	glBindVertexArray ( 0 );
}
