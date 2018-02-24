#include "s_globals.h"
#include "s_camera.h"
#include "s_shaders.h"
#include "s_openGLWrap.h"
#include "s_lightPass.h"
#include "s_defRender.h"

#include "s_ttfFont.h"  //testing only

GLuint          id_FBO          = 0;
GLuint          id_shadowMap    = 0;

GLuint          id_shadowMapPositions = 0;

glm::vec3       lightPos        (40.0, 70.0, 10.02);
glm::vec3       lightRotatePoint(40.0, 30.0, 10.01);
glm::vec3       lightDir(0.0, -0.01, 0.0);

glm::mat4       lightRotateMatrix;

bool            animateLight = true;

unsigned int faceIndexShadow[] =
{
	0,1,2,0,2,3,
	4,5,6,4,6,7,
	2,5,4,2,4,3,
	2,1,6,2,5,6,
	0,6,1,0,7,6,
	3,0,7,3,7,4
};

glm::vec3   vertsShadow[8];

//-----------------------------------------------------------------------------
//
// Return the texture ID of the shadowDepthMap
GLuint gl_getShadowMapTextureID()
//-----------------------------------------------------------------------------
{
	return id_shadowMap;
}

//-----------------------------------------------------------------------------
//
// Return the texture ID of the shadowMap positions
GLuint gl_getShadowMapPositionsID()
//-----------------------------------------------------------------------------
{
	return id_shadowMapPositions;
}

//-----------------------------------------------------------------------------
//
// Move the light in a circle
void shadowMoveLight(float interpolate)
//-----------------------------------------------------------------------------
{
#define PI 3.14159265

	static float delayCount = 0;
	static int angleCounter;

	int radius = 30;

	if (false == animateLight)
		return;

	delayCount += 10.0f * interpolate;

	if (delayCount > 1.0f)
		{
			delayCount = 0.0f;

			angleCounter++;

			if (angleCounter > 359)
				angleCounter = 1;   // Setting to 0 causes GLM to error and crash

			lightRotateMatrix = glm::rotate(glm::mat4(1.0), (float)angleCounter, glm::vec3(0.0f,1.0f,0.0f)) * glm::translate(glm::mat4(1.0), glm::vec3(lightRotatePoint));
			//
			// Extract the world position from matrix values
			lightPos = glm::vec3(lightRotateMatrix[3]);
		}

	lightDir = glm::normalize(lightPos - glm::vec3(lightRotatePoint));

//    lightDir = vec3(0.0,1,0);
}


//-----------------------------------------------------------------------------
//
// Generate verts for voxel - pass in size
void testGenerateVertsShadow(float voxelSize, glm::vec3 position)
//-----------------------------------------------------------------------------
{
	float halfSize = voxelSize / 2;

	vertsShadow[0].x = position[0] - halfSize;
	vertsShadow[0].y = position[1] + halfSize;
	vertsShadow[0].z = position[2] - halfSize;
	vertsShadow[1].x = position[0] - halfSize;
	vertsShadow[1].y = position[1] - halfSize;
	vertsShadow[1].z = position[2] - halfSize;
	vertsShadow[2].x = position[0] + halfSize;
	vertsShadow[2].y = position[1] - halfSize;
	vertsShadow[2].z = position[2] - halfSize;
	vertsShadow[3].x = position[0] + halfSize;
	vertsShadow[3].y = position[1] + halfSize;
	vertsShadow[3].z = position[2] - halfSize;

	vertsShadow[4].x = position[0] + halfSize;
	vertsShadow[4].y = position[1] + halfSize;
	vertsShadow[4].z = position[2] + halfSize;
	vertsShadow[5].x = position[0] + halfSize;
	vertsShadow[5].y = position[1] - halfSize;
	vertsShadow[5].z = position[2] + halfSize;
	vertsShadow[6].x = position[0] - halfSize;
	vertsShadow[6].y = position[1] - halfSize;
	vertsShadow[6].z = position[2] + halfSize;
	vertsShadow[7].x = position[0] - halfSize;
	vertsShadow[7].y = position[1] + halfSize;
	vertsShadow[7].z = position[2] + halfSize;
}

//-----------------------------------------------------------------------------
//
// Draw position of the light
void drawLightPos(int whichShader, glm::vec3 position)
//-----------------------------------------------------------------------------
{
	static GLuint           vao;
	int                     faceCount = 12;
	static GLuint           buffers[2];
	static bool             initDone = false;

	if (false == initDone)
		{

			testGenerateVertsShadow(2.0f, position);

			// create the VAO
			GL_ASSERT(glGenVertexArrays(1, &vao));
			GL_CHECK(glBindVertexArray(vao));
			//
			// create buffers for our vertex data
			GL_ASSERT(glGenBuffers(2, buffers));

			GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));
			//
			//vertex coordinates buffer
			GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, buffers[0]));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertsShadow), vertsShadow, GL_STATIC_DRAW));
			GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
			GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inVertsID,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0)));
			//
			//index buffer
			GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]));
			GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndexShadow), faceIndexShadow, GL_STATIC_DRAW));
			//
			// unbind the VAO
			glBindVertexArray(0);
// TODO (dberry#1#): Change to true
			initDone = false;
		}

	gl_set3DMode();
	cam_look(camPosition, camDirection);
	modelMatrix = glm::mat4();

	GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

	GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr(modelMatrix)));
	GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr(projMatrix * viewMatrix)));

	GL_CHECK(glBindVertexArray(vao));
	//
	// Enable attribute to hold vertex information
	GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));

	GL_CHECK(glDrawElements(GL_TRIANGLES, faceCount*3, GL_UNSIGNED_INT, 0));

	glUseProgram(0);
	glBindVertexArray(0);
}

//-----------------------------------------------------------------------------
//
// Adjust light position
void gl_moveLightPos(glm::vec3 moveVector)
//-----------------------------------------------------------------------------
{
	lightRotatePoint += moveVector;
}

//-----------------------------------------------------------------------------
//
// Return lightPosition
glm::vec3 gl_lightPos()
//-----------------------------------------------------------------------------
{
	return lightPos;
}

//-----------------------------------------------------------------------------
//
// Return lightDirection
glm::vec3 gl_lightDir()
//-----------------------------------------------------------------------------
{
	return lightDir;
}

//-----------------------------------------------------------------------------
//
// Delete buffers and allocated textures
void gl_freeShadowMap()
//-----------------------------------------------------------------------------
{
	if (id_FBO != 0)
		{
			glDeleteFramebuffers(1, &id_FBO);
		}

	if (id_shadowMap != 0)
		{
			glDeleteTextures(1, &id_shadowMap);
		}
}

//-----------------------------------------------------------------------------
//
// Setup the frame buffer and create texture
bool gl_initShadowMap(unsigned int WindowWidth, unsigned int WindowHeight)
//-----------------------------------------------------------------------------
{
	//
	// Create the FBO
	GL_CHECK(glGenFramebuffers(1, &id_FBO));
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, id_FBO));
	//
	// Create depthTexture - uses GL_DEPTH instead of RGB color
	GL_CHECK(glGenTextures(1, &id_shadowMap));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_shadowMap));
	//
	// Create Depth map texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0));
	//
	// Attach the depth texture to the FBO
	GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id_shadowMap, 0));
	//
	// Create the gbuffer to hold the positions for next pass
	GL_ASSERT(glGenTextures(1, &id_shadowMapPositions));

	GL_ASSERT(glActiveTexture(GL_TEXTURE_2D));
	GL_ASSERT(glBindTexture(GL_TEXTURE_2D, id_shadowMapPositions));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// TODO (dberry#1#): May need to change paramaters to reflect actual window size, not shadowmap size

	GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL));
	//
	// Attach the textures to the framebuffer
	GL_ASSERT(glFramebufferTexture2D   (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id_shadowMapPositions, 0));

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0};

	GL_CHECK(glDrawBuffers(1, DrawBuffers));

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE)
		{
			switch (Status)
				{
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
						printf("Not all framebuffer attachment points are framebuffer attachment complete. This means that at least one attachment point with a renderbuffer or texture attached has its attached object no longer in existence or has an attached image with a width or height of zero, or the color attachment point has a non-color-renderable image attached, or the depth attachment point has a non-depth-renderable image attached, or the stencil attachment point has a non-stencil-renderable image attached.\n");
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
						printf("Not all attached images have the same width and height.\n");
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
						printf("No images are attached to the framebuffer.\n");
						break;

					case GL_FRAMEBUFFER_UNSUPPORTED:
						printf("The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.\n");
						break;
				}

			con_print(CON_ERROR, true, "Error: Failed to create shadowDepth Map - status [ 0x%x ]", Status);
			//
			// restore default FBO
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return false;
		}

	// restore default FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

//-----------------------------------------------------------------------------
//
// Make the shadowmap framebuffer the one for writing too
void gl_bindWriteShadowMap()
//-----------------------------------------------------------------------------
{
	GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id_FBO));
}

//-----------------------------------------------------------------------------
//
// Bind the shadowmap texture
void gl_bindReadShadowMap()
//-----------------------------------------------------------------------------
{
	GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, id_FBO));
}

//-----------------------------------------------------------------------------
//
// Return to rendering from the default display buffer
void gl_unbindWriteShadowMap()
//-----------------------------------------------------------------------------
{
	GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

//-----------------------------------------------------------------------------
//
// Finished with the creation of the depth texture
// Return Matrixes to normal camera view
// Return to normal display buffer
void gl_stopShadowMap()
//-----------------------------------------------------------------------------
{
	gl_unbindWriteShadowMap();
}

//-----------------------------------------------------------------------------
//
// Bind framebuffer for writing
// Set matrix to light position
void gl_startShadowMap()
//-----------------------------------------------------------------------------
{
	gl_bindWriteShadowMap();
}
