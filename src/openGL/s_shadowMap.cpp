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

glm::vec3       lightPos        ( 40.0, 70.0, 10.02 );
glm::vec3       lightRotatePoint ( 40.0, 30.0, 10.01 );
glm::vec3       lightDir ( 0.0, -0.01, 0.0 );

glm::mat4       lightRotateMatrix;

bool            animateLight = true;



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
void shadowMoveLight ( float interpolate )
//-----------------------------------------------------------------------------
{
#define PI 3.14159265

	static float delayCount = 0;
	static int angleCounter;

	if ( false == animateLight )
		return;

	delayCount += 10.0f * interpolate;

	if ( delayCount > 1.0f )
		{
			delayCount = 0.0f;

			angleCounter++;

			if ( angleCounter > 359 )
				angleCounter = 1;   // Setting to 0 causes GLM to error and crash

			lightRotateMatrix = glm::rotate ( glm::mat4 ( 1.0 ), ( float ) angleCounter, glm::vec3 ( 0.0f,1.0f,0.0f ) ) * glm::translate ( glm::mat4 ( 1.0 ), glm::vec3 ( lightRotatePoint ) );
			//
			// Extract the world position from matrix values
			lightPos = glm::vec3 ( lightRotateMatrix[3] );
		}

	lightDir = glm::normalize ( lightPos - glm::vec3 ( lightRotatePoint ) );

//    lightDir = vec3(0.0,1,0);
}

//-----------------------------------------------------------------------------
//
// Adjust light position
void gl_moveLightPos ( glm::vec3 moveVector )
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
	if ( id_FBO != 0 )
		{
			glDeleteFramebuffers ( 1, &id_FBO );
		}

	if ( id_shadowMap != 0 )
		{
			glDeleteTextures ( 1, &id_shadowMap );
		}
}

//-----------------------------------------------------------------------------
//
// Setup the frame buffer and create texture
bool gl_initShadowMap ( unsigned int WindowWidth, unsigned int WindowHeight )
//-----------------------------------------------------------------------------
{
	//
	// Create the FBO
	GL_CHECK ( glGenFramebuffers ( 1, &id_FBO ) );
	GL_CHECK ( glBindFramebuffer ( GL_FRAMEBUFFER, id_FBO ) );
	//
	// Create depthTexture - uses GL_DEPTH instead of RGB color
	GL_CHECK ( glGenTextures ( 1, &id_shadowMap ) );
	GL_CHECK ( glBindTexture ( GL_TEXTURE_2D, id_shadowMap ) );
	//
	// Create Depth map texture
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	GL_CHECK ( glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	GL_CHECK ( glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );

	GL_CHECK ( glTexImage2D ( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0 ) );
	//
	// Attach the depth texture to the FBO
	GL_CHECK ( glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id_shadowMap, 0 ) );
	//
	// Create the gbuffer to hold the positions for next pass
	GL_ASSERT ( glGenTextures ( 1, &id_shadowMapPositions ) );

	GL_ASSERT ( glActiveTexture ( GL_TEXTURE_2D ) );
	GL_ASSERT ( glBindTexture ( GL_TEXTURE_2D, id_shadowMapPositions ) );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
// TODO (dberry#1#): May need to change paramaters to reflect actual window size, not shadowmap size

	GL_ASSERT ( glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL ) );
	//
	// Attach the textures to the framebuffer
	GL_ASSERT ( glFramebufferTexture2D   ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id_shadowMapPositions, 0 ) );

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0};

	GL_CHECK ( glDrawBuffers ( 1, DrawBuffers ) );

	GLenum Status = glCheckFramebufferStatus ( GL_FRAMEBUFFER );

	if ( Status != GL_FRAMEBUFFER_COMPLETE )
		{
			switch ( Status )
				{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					printf ( "Not all framebuffer attachment points are framebuffer attachment complete. This means that at least one attachment point with a renderbuffer or texture attached has its attached object no longer in existence or has an attached image with a width or height of zero, or the color attachment point has a non-color-renderable image attached, or the depth attachment point has a non-depth-renderable image attached, or the stencil attachment point has a non-stencil-renderable image attached.\n" );
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
					printf ( "Not all attached images have the same width and height.\n" );
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					printf ( "No images are attached to the framebuffer.\n" );
					break;

				case GL_FRAMEBUFFER_UNSUPPORTED:
					printf ( "The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.\n" );
					break;
				}

			con_print ( CON_ERROR, true, "Error: Failed to create shadowDepth Map - status [ 0x%x ]", Status );
			//
			// restore default FBO
			glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
			return false;
		}

	// restore default FBO
	glBindFramebuffer ( GL_FRAMEBUFFER, 0 );

	return true;
}

//-----------------------------------------------------------------------------
//
// Make the shadowmap framebuffer the one for writing too
void gl_bindWriteShadowMap()
//-----------------------------------------------------------------------------
{
	GL_CHECK ( glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, id_FBO ) );
}

//-----------------------------------------------------------------------------
//
// Bind the shadowmap texture
void gl_bindReadShadowMap()
//-----------------------------------------------------------------------------
{
	GL_CHECK ( glBindFramebuffer ( GL_READ_FRAMEBUFFER, id_FBO ) );
}

//-----------------------------------------------------------------------------
//
// Return to rendering from the default display buffer
void gl_unbindWriteShadowMap()
//-----------------------------------------------------------------------------
{
	GL_ASSERT ( glBindFramebuffer ( GL_FRAMEBUFFER, 0 ) );
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
