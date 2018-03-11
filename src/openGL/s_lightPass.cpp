#include "s_globals.h"
#include "s_camera.h"
#include "s_openGLWrap.h"
#include "s_shaders.h"
#include "s_lightPass.h"
#include "s_defRender.h"

#include "io_textures.h"
#include "s_shadowMap.h"
#include "s_render.h"
#include "s_entitiesBSP.h"
#include "s_shaderLights.h"

static bool	lightSphereCreated = false;

GLuint	m_diffuseID;
GLuint	m_positionID;
GLuint	m_normalsID;
GLuint  m_camPositionID;
GLuint  m_lightPositionID;
GLuint  m_mvMatrix;
GLuint  m_normalMatrix;
GLuint  m_tShadowMapID;
GLuint	m_DepthMapID;

GLuint  m_PositionShadowMap;

// light sphere geometry:
GLuint lightSpherePositionVbo;
GLuint lightSphereIndexVbo;
GLuint lightSphereIndexCount;
GLuint lightSphereVAO;

//-----------------------------------------------------------------------------
//
// Render all the ligth spheres into the GBuffer
void lt_renderPointLights(int whichShader)
//-----------------------------------------------------------------------------
{
	//
	// Next, we render all the point light soures.
	// We will be doing our own depth testing in frag shader, so disable depth testing.
	// Enable alpha blending. So that the rendered point lights are added to the framebuffer.
	//
	GL_CHECK(glDisable(GL_DEPTH_TEST));
	GL_CHECK(glEnable(GL_BLEND));
	GL_CHECK(glBlendFunc(GL_ONE, GL_ONE));

	// We render only the inner faces of the light sphere.
	// In other words, we render the back-faces and not the front-faces of the sphere.
	// If we render the front-faces, the lighting of the light sphere disappears if
	// we are inside the sphere, which is weird. But by rendering the back-faces instead,
	// we solve this problem.
	GL_CHECK(glFrontFace(GL_CW));
	
		for (int i = 0; i < numOfLights; i++)
		{
			ass_renderMesh(MODEL_SPHERE,  whichShader, allLights[i].position, allLights[i].attenuation * 100, allLights[i].color);
		}

	GL_CHECK(glEnable(GL_DEPTH_TEST));
	GL_CHECK(glDisable(GL_BLEND));
	GL_CHECK(glFrontFace(GL_CCW));
}

//-----------------------------------------------------------------------------
//
// Render a full screen quad
void lt_renderFullscreenQuad(int whichShader)
//-----------------------------------------------------------------------------
{
	
	glm::vec2			quadVerts[4];
	static GLuint		vao = -1;
	static GLuint		buffers[2];
	int					faceCount = 2;
	
	unsigned int vertsTriggerIndex[] =
	{
		0,1,2,0,2,3,
	};
	
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
	
	if (vao == -1)
	{
		// create the VAO
		GL_ASSERT(glGenVertexArrays(1, &vao));
		GL_CHECK(glBindVertexArray(vao));

		// Create buffers for the vertex data
		GL_ASSERT(glGenBuffers(2, buffers));
		
		GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

		// Vertex coordinates buffer
		GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, buffers[0]));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_DYNAMIC_DRAW));
		GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
		GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inVertsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)));

		// Texture coordinates buffer
		GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, buffers[1]));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoords), quadTexCoords, GL_DYNAMIC_DRAW));
		GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
		GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)));		
	}
	
	GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

	//
	// Bind texture if it's not already bound as current texture
	GL_CHECK(glActiveTexture(GL_TEXTURE0 + 0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_POSITION]));
	GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID, "tPosition"), 0));

	GL_CHECK(glActiveTexture(GL_TEXTURE0 + 1));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_NORMAL]));
	GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID, "tNormals"), 1));

	GL_CHECK(glActiveTexture(GL_TEXTURE0 + 2));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_DIFFUSE]));
	GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID, "tDiffuse"), 2));

	GL_CHECK(glUniform3f(glGetUniformLocation(shaderProgram[whichShader].programID, "cameraPosition"), camPosition.x, camPosition.y, camPosition.z));
	
	GL_CHECK(glBindVertexArray(vao));
	//
	// Enable attribute to hold vertex information
	GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
	GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
	
	GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}

//-----------------------------------------------------------------------------
//
// Render a full screen quad
void lt_renderDepthQuad(int whichShader)
//-----------------------------------------------------------------------------
{

	GLfloat quadVertsDepthMap[] =
	{
		0.0,        0.0,
		winWidth / 2,   0.0,
		winWidth / 2,   winHeight / 2,
		0.0,        winHeight / 2,
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

	if (false == initDone)
		{
			//
			// Enable the shader program
			GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

			GL_ASSERT(glGenVertexArrays(1, &vaoQuadDepthMap));
			GL_CHECK(glBindVertexArray(vaoQuadDepthMap));

			GL_ASSERT(glGenBuffers(1, &vboQuadDepthMap));
			//
			// Use Vertices
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboQuadDepthMap));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8, &quadVertsDepthMap, GL_STATIC_DRAW));
			GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
			GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inVertsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 ) ));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

			GL_ASSERT(glGenBuffers(1, &vboQuadTexDepthMap));
			//
			// Use texture coords
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboQuadTexDepthMap));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8, &quadTexCoordsDepthMap, GL_STATIC_DRAW));
			GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
			GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 ) ));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

			glBindVertexArray(0);
			glUseProgram(0);
			initDone = true;
		}

	//
	// Draw in ortho mode
	gl_set2DMode();

	modelMatrix = glm::translate(glm::mat4(), glm::vec3(winWidth / 2, winHeight / 2, 1.0));
	glm::mat4   mvpMatrix = projMatrix * modelMatrix;

	//
	// Use the program to make the attributes available
	GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));
	//
	// Bind texture if it's not already bound as current texture
	wrapglBindTexture(GL_TEXTURE0, gl_returnDepthTexID());

	GL_CHECK(glUniform1i(shaderProgram[whichShader].inTextureUnit, 0));
	//
	// Load the matrixes into the vertex shader
	GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr(mvpMatrix)));

	GL_CHECK(glBindVertexArray(vaoQuadDepthMap));
	GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
	GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
	//
	// Draw a triangle
	GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));

	glDisableVertexAttribArray(shaderProgram[whichShader].inVertsID);
	glDisableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID);

	glBindVertexArray(0);
	glUseProgram(0);

	gl_set3DMode();
}
