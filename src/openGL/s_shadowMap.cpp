#include <hdr/openGL/s_shaders.h>
#include <hdr/system/s_camera3.h>
#include "s_globals.h"
#include "s_renderBSP.h"

unsigned int g_shadowWidth;
unsigned int g_shadowHeight;

static unsigned int        depthMapFBO;
static unsigned int        depthCubemap;

std::vector<glm::mat4> g_shadowTransforms;

float near_plane = 1.0f;
float far_plane = 25.0f;

//-----------------------------------------------------------------------------
//
// Get the Texture ID for the depth map
int shd_getDepthTextureID()
//-----------------------------------------------------------------------------
{
	printf ("Cube map ID [ %i ]\n", depthCubemap);
	return depthCubemap;
}

//-----------------------------------------------------------------------------
//
// Init the shadow cubemap and texture
bool shd_initShadowMap(unsigned int shadowWidth, unsigned int shadowHeight)
//-----------------------------------------------------------------------------
{
	int textureSize = 1024;

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

	g_shadowWidth = shadowWidth;
	g_shadowHeight = shadowHeight;

	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthCubemap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	//
	// Create the six sides of the cube map
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, g_shadowWidth, g_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, image);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
}

//-----------------------------------------------------------------------------
//
// 0. create depth cubemap transformation matrices
void shd_shadowMapDepthStartRender(glm::vec3 lightPos, int whichShader)
//-----------------------------------------------------------------------------
{
	int stride = sizeof ( _myVertex ); // BSP Vertex, not float[3]

	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);

	GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO) );
	GL_ASSERT( glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0) );
//	GL_ASSERT( glDrawBuffer(GL_NONE) );
//	GL_ASSERT( glReadBuffer(GL_NONE) );

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, 0);

	GL_ASSERT( glUseProgram(shaderProgram[whichShader].programID) );

	GL_CHECK ( glBindVertexArray ( bspVAO ) );
	//
	// Bind data buffer holding all the vertices
	GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, bspVBO ));

	glm::mat4 shadowProj = glm::perspective (glm::radians (90.0f), (float) g_shadowWidth / (float) g_shadowHeight, near_plane,far_plane);

	g_shadowTransforms.push_back(shadowProj*glm::lookAt(lightPos, lightPos+ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	g_shadowTransforms.push_back(shadowProj*glm::lookAt(lightPos, lightPos+ glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	g_shadowTransforms.push_back(shadowProj*glm::lookAt(lightPos, lightPos+ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	g_shadowTransforms.push_back(shadowProj*glm::lookAt(lightPos, lightPos+ glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	g_shadowTransforms.push_back(shadowProj*glm::lookAt(lightPos, lightPos+ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	g_shadowTransforms.push_back(shadowProj*glm::lookAt(lightPos, lightPos+ glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	// 1. render scene to depth cubemap
	// --------------------------------
	glViewport(0, 0, g_shadowWidth, g_shadowHeight);

	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "shadowMatrices[0]"), 1, false, glm::value_ptr (g_shadowTransforms[0])));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "shadowMatrices[1]"), 1, false, glm::value_ptr (g_shadowTransforms[1])));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "shadowMatrices[2]"), 1, false, glm::value_ptr (g_shadowTransforms[2])));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "shadowMatrices[3]"), 1, false, glm::value_ptr (g_shadowTransforms[3])));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "shadowMatrices[4]"), 1, false, glm::value_ptr (g_shadowTransforms[4])));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "shadowMatrices[5]"), 1, false, glm::value_ptr (g_shadowTransforms[5])));

	GL_ASSERT ( glUniformMatrix4fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "u_modelMat" ), 1, false, glm::value_ptr ( glm::mat4() ) ) );

	GL_ASSERT ( glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID) );
	GL_ASSERT ( glVertexAttribPointer (shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, GL_FALSE, stride, offsetof (_myVertex, position)) );

	GL_ASSERT ( glUniform1f  ( glGetUniformLocation ( shaderProgram[whichShader].programID, "far_plane" ), far_plane ) );
	GL_ASSERT ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "lightPos" ), 1, glm::value_ptr ( lightPos ) ) );

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

//	g_facesForFrame.clear();
//	g_currentFrameVertexIndex.clear();
//	g_texturesChanges = 0;
//	g_vertIndexCounter = 0;
//	g_numVertexPerFrame = 0;
}

//-----------------------------------------------------------------------------
//
// Render the level as normal
void shd_shadowRenderNormal(int whichShader, glm::vec3 lightPos)
//-----------------------------------------------------------------------------
{
	int stride = sizeof ( _myVertex ); // BSP Vertex, not float[3]

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, winWidth, winHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl_set3DMode();
	glUseProgram(shaderProgram[whichShader].programID);

	//
	// Bind data buffer holding all the vertices
	GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, bspVBO ));

	GL_ASSERT ( glUniformMatrix4fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "projection" ), 1, false, glm::value_ptr ( projMatrix ) ) );
	GL_ASSERT ( glUniformMatrix4fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "view" ), 1, false, glm::value_ptr ( viewMatrix ) ) );
	GL_ASSERT ( glUniformMatrix4fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "model" ), 1, false, glm::value_ptr ( glm::mat4() ) ) );
	GL_ASSERT ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "lightPos" ), 1, glm::value_ptr ( lightPos ) ) );
	GL_ASSERT ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "viewPos" ), 1, glm::value_ptr ( cam3_getCameraPosition () ) ) );
	GL_ASSERT ( glUniform1i  ( glGetUniformLocation ( shaderProgram[whichShader].programID, "shadows" ), 1 ) );
	GL_ASSERT ( glUniform1f  ( glGetUniformLocation ( shaderProgram[whichShader].programID, "far_plane" ), far_plane ) );

	GL_ASSERT ( glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID) );
	GL_ASSERT ( glVertexAttribPointer (shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, GL_FALSE, stride, offsetof (_myVertex, position)) );

	// Normals
	GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );
	GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inNormalsID, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)( offsetof ( _myVertex, normals ) ) ) );

	// Texture coords
	GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
	GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)( offsetof ( _myVertex, texCoordsLightmap ) ) ) );

	// Texture coords for texture
	if (true == g_renderTextures)
	{
		// Texture coords
		GL_ASSERT (glEnableVertexAttribArray (shaderProgram[whichShader].inTextureCoordsID_1));
		GL_ASSERT (glVertexAttribPointer (shaderProgram[whichShader].inTextureCoordsID_1, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (offsetof (_myVertex, texCoords))));
	}

	if (true == g_renderTextures)
	{
		GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID_1) );
	}

	g_facesForFrame.clear();
	g_currentFrameVertexIndex.clear();
//	g_texturesChanges = 0;
	g_vertIndexCounter = 0;
	g_numVertexPerFrame = 0;

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
}