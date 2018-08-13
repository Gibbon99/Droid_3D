#include <hdr/openGL/s_shadowMap.h>
#include <hdr/system/s_camera3.h>
#include <hdr/openGL/s_shadowLighting.h>
#include <hdr/bsp/s_renderBSP.h>
#include <hdr/system/s_maths.h>
#include <hdr/io/io_textures.h>
#include <hdr/openGL/s_openGLWrap.h>
#include "s_shaders.h"
#include "s_globals.h"

int 	g_shadowWidth;
int     g_shadowHeight;

float near_plane = 1.0f;
float far_plane = 2500.0f;

std::vector<glm::mat4> shadowTransforms;

#define NUM_LIGHT_CASTERS   1

GLuint depthMapFBO;
GLuint depthCubemap[NUM_LIGHT_CASTERS];

glm::vec3 g_lightPosition[NUM_LIGHT_CASTERS];


// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;

//----------------------------------------------------------------------------------------
//
// Draw cube
void renderCube(int whichShader)
//----------------------------------------------------------------------------------------
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
				// bottom face
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
				1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

//-----------------------------------------------------------------------------------
//
// Render the test cube scene
void shd_renderScene(int whichShader)
//-----------------------------------------------------------------------------------
{
	// room cube
	glm::mat4 model;

	model = glm::scale(model, glm::vec3(5.0f));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "model"), 1, false, glm::value_ptr (model)));
	glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "reverse_normals" ), 1 ) );
	renderCube(whichShader);
	GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "reverse_normals" ), 0 ) );
	glEnable(GL_CULL_FACE);

// cubes
	model = glm::mat4 ();
	model = glm::translate (model, glm::vec3 (4.0f, -3.5f, 0.0));
	model = glm::scale (model, glm::vec3 (0.5f));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "model"), 1, false, glm::value_ptr (model)));
	renderCube (whichShader);

	model = glm::mat4 ();
	model = glm::translate (model, glm::vec3 (2.0f, 3.0f, 1.0));
	model = glm::scale (model, glm::vec3 (0.75f));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "model"), 1, false, glm::value_ptr (model)));
	renderCube (whichShader);

	model = glm::mat4 ();
	model = glm::translate (model, glm::vec3 (-3.0f, -1.0f, 0.0));
	model = glm::scale (model, glm::vec3 (0.5f));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "model"), 1, false, glm::value_ptr (model)));
	renderCube (whichShader);

	model = glm::mat4 ();
	model = glm::translate (model, glm::vec3 (-1.5f, 1.0f, 1.5));
	model = glm::scale (model, glm::vec3 (0.5f));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "model"), 1, false, glm::value_ptr (model)));
	renderCube (whichShader);

	model = glm::mat4 ();
	model = glm::translate (model, glm::vec3 (-1.5f, 2.0f, -3.0));
	model = glm::rotate (model, glm::radians (60.0f), glm::normalize (glm::vec3 (1.0, 0.0, 1.0)));
	model = glm::scale (model, glm::vec3 (0.75f));
	GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "model"), 1, false, glm::value_ptr (model)));
	renderCube (whichShader);
}

//-----------------------------------------------------------------------------------
//
// Setup up the FBO and depth cubemap
// Return status of framebuffer binding operation
bool shd_shadowMapInit(unsigned int shadowWidth, unsigned int shadowHeight)
//-----------------------------------------------------------------------------------
{
	g_shadowWidth = shadowWidth;
	g_shadowHeight = shadowHeight;

	glGenFramebuffers(1, &depthMapFBO);
	//
	// create depth cubemap texture
	glGenTextures(NUM_LIGHT_CASTERS, depthCubemap);

	for (int i = 0; i != NUM_LIGHT_CASTERS; i++)
	{
		glBindTexture (GL_TEXTURE_CUBE_MAP, depthCubemap[i]);

		for ( unsigned int j = 0; j < 6; ++j )
			glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, g_shadowWidth, g_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//
		// attach depth texture as FBO's depth buffer
		//
		glBindFramebuffer (GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap[i], 0);

		GLenum Status = glCheckFramebufferStatus (GL_FRAMEBUFFER);

		if ( Status != GL_FRAMEBUFFER_COMPLETE )
		{
			switch ( Status )
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					con_print (CON_ERROR, true, "Not all framebuffer attachment points are framebuffer attachment complete. This means that at least one attachment point with a renderbuffer or texture attached has its attached object no longer in existence or has an attached image with a width or height of zero, or the color attachment point has a non-color-renderable image attached, or the depth attachment point has a non-depth-renderable image attached, or the stencil attachment point has a non-stencil-renderable image attached.");
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					con_print (CON_ERROR, true, "No images are attached to the framebuffer.");
					break;

				case GL_FRAMEBUFFER_UNSUPPORTED:
					con_print (CON_ERROR, true, "The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.");
					break;

				default:
					con_print(CON_ERROR, true, "Unknown error binding framebuffer - status [ 0x%x ]", Status);
					break;
			}
			//
			// restore default FBO
			glBindFramebuffer (GL_FRAMEBUFFER, 0);
			return false;
		}
	}

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	con_print (CON_INFO, true, "Shadowmap framebuffer created ok.");

	g_lightPosition[0] = glm::vec3{-160.0f, 48.0f, 216.0f};

	return true;
}

//-----------------------------------------------------------------------------------
//
// clean up the Shadowmap
void shd_exitShadowMap()
//-----------------------------------------------------------------------------------
{
    if (depthMapFBO != 0)
    {
        glDeleteFramebuffers(1, &depthMapFBO);
    }

    if (depthCubemap[0] != 0)
    {
        glDeleteTextures(NUM_LIGHT_CASTERS, depthCubemap);
    }
}

//-----------------------------------------------------------------------------------
//
// Render the level to the depth cubemap textures
void shd_prepareDepthRender(glm::vec3 lightPos)
//-----------------------------------------------------------------------------------
{
	//
	// Set the perspective matrix - 90 degrees flat for the texture
	projMatrix = glm::perspective (90.0f, (float) g_shadowWidth / (float) g_shadowHeight, near_plane, far_plane);
	viewMatrix = glm::lookAt (lightPos, lightPos + glm::vec3 (0.0f, 0.0f, 3.0f),  glm::vec3 (0.0f, 1.0f, 0.0f));

	shadowTransforms.push_back (projMatrix * glm::lookAt (lightPos, lightPos + glm::vec3 (1.0f, 0.0f, 0.0f),  glm::vec3 (0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back (projMatrix * glm::lookAt (lightPos, lightPos + glm::vec3 (-1.0f, 0.0f, 0.0f), glm::vec3 (0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back (projMatrix * glm::lookAt (lightPos, lightPos + glm::vec3 (0.0f, 1.0f, 0.0f),  glm::vec3 (0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back (projMatrix * glm::lookAt (lightPos, lightPos + glm::vec3 (0.0f, -1.0f, 0.0f), glm::vec3 (0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back (projMatrix * glm::lookAt (lightPos, lightPos + glm::vec3 (0.0f, 0.0f, 1.0f),  glm::vec3 (0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back (projMatrix * glm::lookAt (lightPos, lightPos + glm::vec3 (0.0f, 0.0f, -1.0f), glm::vec3 (0.0f, -1.0f, 0.0f)));
}

//-----------------------------------------------------------------------------------
// Render the level to the depth cubemap textures
void shd_shadowMapPass(int whichShader)
//-----------------------------------------------------------------------------------
{
	int startIndex = 0;

	int i;

	bsp_bindLevelData ();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
//	glDrawBuffer(GL_NONE);  // Disable writes to the color buffer
//	glReadBuffer(GL_NONE);  // Disable reads from the color buffer

	glViewport (0, 0, g_shadowWidth, g_shadowHeight);
	glBindFramebuffer (GL_FRAMEBUFFER, depthMapFBO);
	glUseProgram(shaderProgram[whichShader].programID);

	int returnValue;

	returnValue = glGetUniformLocation ( shaderProgram[whichShader].programID, "farPlane" );
	GL_ASSERT ( glUniform1f  ( returnValue, far_plane ) );

	//--------------------- loop here -----------------

	for (i = startIndex; i != NUM_LIGHT_CASTERS; i++)
	{
		g_lightPosition[i].x += 0.001f * al_get_time();

		glFramebufferTexture (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap[i], 0);
		glClear (GL_DEPTH_BUFFER_BIT);
		shd_prepareDepthRender (g_lightPosition[i]);

		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[0]"), 1, false, glm::value_ptr (shadowTransforms[0])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[1]"), 1, false, glm::value_ptr (shadowTransforms[1])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[2]"), 1, false, glm::value_ptr (shadowTransforms[2])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[3]"), 1, false, glm::value_ptr (shadowTransforms[3])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[4]"), 1, false, glm::value_ptr (shadowTransforms[4])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[5]"), 1, false, glm::value_ptr (shadowTransforms[5])));

		GL_ASSERT (glUniform3fv (shaderGetLocation (whichShader, "lightPos"), 1, glm::value_ptr (g_lightPosition[i])));

		GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "reverse_normals" ), 0 ) );

		glm::mat4 model;
		GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "model"), 1, false, glm::value_ptr (model)));

		bsp_renderLevel ( g_lightPosition[i], whichShader, true );

		shadowTransforms.clear ();
	}
	glUseProgram(0);
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	glDisable(GL_CULL_FACE);



	// 2. render scene as normal
	// -------------------------
	glViewport(0, 0, winWidth, winHeight);
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram[SHADER_SHADOW_LIGHTING].programID);
	cam3_createProjMatrix ();
	cam3_createViewMatrix (cam3_Position);

	GL_ASSERT ( glUniformMatrix4fv ( shaderGetLocation (SHADER_SHADOW_LIGHTING, "projection" ), 1, false, glm::value_ptr ( projMatrix ) ) );
	GL_ASSERT ( glUniformMatrix4fv ( shaderGetLocation (SHADER_SHADOW_LIGHTING, "view" ), 1, false, glm::value_ptr ( viewMatrix ) ) );

	GL_ASSERT ( glUniform1f  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "far_planeTwo"), far_plane ) );

	GL_ASSERT ( glUniform1i  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "shadows"), 1 ) );
	GL_ASSERT ( glUniform3fv (shaderGetLocation (SHADER_SHADOW_LIGHTING, "viewPos"), 1, glm::value_ptr (cam3_getCameraPosition ())));
	//
	// Set the texture units
	GL_ASSERT ( glUniform1i  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "diffuseTexture"), 0 ) );   // which texture unit
	GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[SHADER_SHADOW_LIGHTING].programID, "depthMap" ), 1 ) );   // Shader type is cubeMap - funny value returned
	//
	// Bind texture ID to shader texture unit
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, checkerBoardTexture);

	bsp_bindLevelData ();

	for (i = startIndex; i != NUM_LIGHT_CASTERS; i++)
	{
		// set lighting uniforms
		GL_ASSERT (glUniform3fv (shaderGetLocation (SHADER_SHADOW_LIGHTING, "lightPos"), 1, glm::value_ptr (g_lightPosition[i])));
		GL_ASSERT (glUniform3fv (shaderGetLocation (SHADER_SHADOW_LIGHTING, "lightColor"), 1, glm::value_ptr (glm::vec3{1.0, 1.0, 0.4})));

		GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[SHADER_SHADOW_LIGHTING].programID, "model"), 1, false, glm::value_ptr (glm::mat4())));

		glActiveTexture (GL_TEXTURE1);
		glBindTexture (GL_TEXTURE_CUBE_MAP, depthCubemap[i]);

		bsp_renderLevel ( cam3_getCameraPosition (), SHADER_SHADOW_LIGHTING, true );

		glm::mat4 model = glm::mat4 ();
		model = glm::translate (model, glm::vec3 (g_lightPosition[i].x, g_lightPosition[i].y, g_lightPosition[i].z));
		model = glm::scale (model, glm::vec3 (4.0f));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (SHADER_SHADOW_LIGHTING, "model"), 1, false, glm::value_ptr (model)));
		renderCube (SHADER_SHADOW_LIGHTING);
	}

	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}