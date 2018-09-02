#include <hdr/openGL/s_shadowMap.h>
#include <hdr/system/s_camera3.h>
#include <hdr/openGL/s_shadowLighting.h>
#include <hdr/bsp/s_renderBSP.h>
#include <hdr/system/s_maths.h>
#include <hdr/io/io_textures.h>
#include <hdr/openGL/s_openGLWrap.h>
#include <hdr/game/s_objects.h>
#include <hdr/bsp/s_shaderLights.h>
#include "s_shaders.h"
#include "s_globals.h"

int 	g_shadowWidth;
int     g_shadowHeight;

float near_plane = 1.0f;
float far_plane = 100.0f;

std::vector<glm::mat4> shadowTransforms;

#define NUM_LIGHT_CASTERS   5

GLuint depthMapFBO;
GLuint depthCubemap[NUM_LIGHT_CASTERS];

typedef struct
{
	glm::vec3   position;           // 0, 1, 2
	float       size;               // Pad to 3
	glm::vec3   color;
} _lightInfo;

_lightInfo lightInfo[NUM_LIGHT_CASTERS];

GLuint uboBlock_ID;

float lightConstant = 0.5f;
float lightLinear = 0.05f;
float lightQuadratic = 0.0004f; //0.0032f;     // Light casting size - lower is bigger


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

	//
	// Uniform block for light data array
	uboBlock_ID = wrapglGenBuffers(1, __func__);
	glBindBuffer(GL_UNIFORM_BUFFER, uboBlock_ID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_lightInfo) * NUM_LIGHT_CASTERS, (void *)&lightInfo[0].position, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboBlock_ID, 0, sizeof(_lightInfo));

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
	int i;

	bsp_bindLevelData ();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
//	glDrawBuffer(GL_NONE);  // Disable writes to the color buffer
//	glReadBuffer(GL_NONE);  // Disable reads from the color buffer

	glViewport (0, 0, g_shadowWidth, g_shadowHeight);
	glBindFramebuffer (GL_FRAMEBUFFER, depthMapFBO);
	glUseProgram(shaderProgram[whichShader].programID);

	int returnValue;

	returnValue = glGetUniformLocation ( shaderProgram[whichShader].programID, "farPlane" );
	GL_ASSERT ( glUniform1f  ( returnValue, far_plane ) );

	//--------------------- loop here -----------------

	for (i = 0; i != NUM_LIGHT_CASTERS; i++)
	{
		allLights[i].position.z += sin(al_get_time()) * 0.5f; //0.001f * al_get_time();

		glFramebufferTexture (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap[i], 0);
		glClear (GL_DEPTH_BUFFER_BIT);
		shd_prepareDepthRender (allLights[i].position);

		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[0]"), 1, false, glm::value_ptr (shadowTransforms[0])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[1]"), 1, false, glm::value_ptr (shadowTransforms[1])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[2]"), 1, false, glm::value_ptr (shadowTransforms[2])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[3]"), 1, false, glm::value_ptr (shadowTransforms[3])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[4]"), 1, false, glm::value_ptr (shadowTransforms[4])));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (whichShader, "shadowMatrices[5]"), 1, false, glm::value_ptr (shadowTransforms[5])));

		GL_ASSERT (glUniform3fv (shaderGetLocation (whichShader, "lightPos"), 1, glm::value_ptr (allLights[i].position)));

		GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[whichShader].programID, "model"), 1, false, glm::value_ptr (glm::mat4())));

		bsp_renderLevel ( allLights[i].position, whichShader, true );

		obj_renderAllObjects(whichShader);

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

	GL_ASSERT ( glUniform1f  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "shadowLevel"), 0.9f ) );
	GL_ASSERT ( glUniform3fv (shaderGetLocation (SHADER_SHADOW_LIGHTING, "viewPos"), 1, glm::value_ptr (cam3_getCameraPosition ())));
	//
	// Set the texture units
	GL_ASSERT ( glUniform1i  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "diffuseTexture"), 0 ) );   // which texture unit
	GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[SHADER_SHADOW_LIGHTING].programID, "depthMap" ), 1 ) );   // Shader type is cubeMap - funny value returned

	GL_ASSERT ( glUniform1i  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "loopCount"), NUM_LIGHT_CASTERS ) );   // which texture unit

	//
	// Bind texture ID to shader texture unit
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, checkerBoardTexture);

	bsp_bindLevelData ();


	GLuint lightInfoBlockIndex = glGetUniformBlockIndex(shaderProgram[SHADER_SHADOW_LIGHTING].programID, "lightDataBlock");
	GL_ASSERT ( glUniformBlockBinding(shaderProgram[SHADER_SHADOW_LIGHTING].programID, lightInfoBlockIndex, 0) );
	GL_ASSERT ( glBindBuffer(GL_UNIFORM_BUFFER, uboBlock_ID) );
	GL_ASSERT ( glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(_lightInfo) * NUM_LIGHT_CASTERS, (void *)&lightInfo[0].position) );
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (int k = 0; k != NUM_LIGHT_CASTERS; k++)
	{
		lightInfo[k].position = allLights[k].position;
		lightInfo[k].size = 0.0003f;
		lightInfo[k].color.r = 0.0f; //allLights[k].color.r / 255;
		lightInfo[k].color.g = 1.0f; //allLights[k].color.g / 255;
		lightInfo[k].color.b = 0.0f; //allLights[k].color.b / 255;

		printf("[ %i ] Color [ %3.3f %3.3f %3.3f ]\n", k, lightInfo[k].color.r, lightInfo[k].color.g, lightInfo[k].color.b);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, uboBlock_ID);
	GLvoid *p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &lightInfo[0].position, sizeof(_lightInfo) * NUM_LIGHT_CASTERS);
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	for (i = 0; i != NUM_LIGHT_CASTERS; i++)
	{
		glActiveTexture (GL_TEXTURE1);
		glBindTexture (GL_TEXTURE_CUBE_MAP, depthCubemap[i]);

		GL_ASSERT (glUniformMatrix4fv (glGetUniformLocation (shaderProgram[SHADER_SHADOW_LIGHTING].programID, "model"), 1, false, glm::value_ptr (glm::mat4())));

		GL_ASSERT ( glUniform1f  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "lightConstant"), lightConstant ) );
		GL_ASSERT ( glUniform1f  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "lightLinear"), lightLinear ) );
		GL_ASSERT ( glUniform1f  (shaderGetLocation (SHADER_SHADOW_LIGHTING, "lightQuadratic"), lightQuadratic ) );

		bsp_renderLevel ( cam3_getCameraPosition (), SHADER_SHADOW_LIGHTING, true );

		obj_renderAllObjects(SHADER_SHADOW_LIGHTING);

		glm::mat4 model = glm::mat4 ();
		model = glm::translate (model, glm::vec3 (lightInfo[i].position.x, lightInfo[i].position.y, lightInfo[i].position.z));
		model = glm::scale (model, glm::vec3 (4.0f));
		GL_ASSERT (glUniformMatrix4fv (shaderGetLocation (SHADER_SHADOW_LIGHTING, "model"), 1, false, glm::value_ptr (model)));
		renderCube (SHADER_SHADOW_LIGHTING);
	}

	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}