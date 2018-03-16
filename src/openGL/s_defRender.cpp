#include "s_globals.h"
#include "s_camera.h"
#include "s_shaders.h"
#include "s_openGLWrap.h"
#include "s_defRender.h"
#include "s_ttfFont.h"
#include "s_lightPass.h"

GLuint          id_depthTexture = 0;
GLuint          id_depthBuffer = 0;
GLuint          defRender_FBO = 0;
GLuint          id_textures[GBUFFER_NUM_TEXTURES];

//-----------------------------------------------------------------------------
//
// Return the TextureID for the depth buffer texture
GLuint gl_returnDepthTexID()
//-----------------------------------------------------------------------------
{
	return id_depthTexture;
}

//-----------------------------------------------------------------------------
//
// Return texture ID for texture array by index
GLuint gl_returnTexID ( int index )
//-----------------------------------------------------------------------------
{
	return id_textures[index];
}

//-----------------------------------------------------------------------------
//
// Bind FBO for writing
void gl_bindForWriting()
//-----------------------------------------------------------------------------
{
	GL_CHECK ( glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defRender_FBO ) );
}

//-----------------------------------------------------------------------------
//
// Bind FBO for reading
void gl_bindForReading()
//-----------------------------------------------------------------------------
{
	GL_CHECK ( glBindFramebuffer ( GL_READ_FRAMEBUFFER, defRender_FBO ) );
}

//-----------------------------------------------------------------------------
//
// Create the GBuffer
void gl_createGBufTex ( GLenum texUnit, GLenum internalFormat, GLuint &texid, GLuint width, GLuint height, GLenum format )
//-----------------------------------------------------------------------------
{
	GL_ASSERT ( glActiveTexture ( texUnit ) );
	GL_ASSERT ( glGenTextures ( 1, &texid ) );

	wrapglBindTexture ( texUnit, texid );

//    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, texid));
	GL_ASSERT ( glTexImage2D ( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, NULL ) );

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

//-----------------------------------------------------------------------------
//
// Setup the FBO and other GBuffers
bool gl_initDefRender ( int screenWidth, int screenHeight )
//-----------------------------------------------------------------------------
{
	ZERO_MEM ( id_textures );
	// Create the FBO
	GL_CHECK ( glGenFramebuffers ( 1, &defRender_FBO ) );

	GL_CHECK ( glBindFramebuffer ( GL_FRAMEBUFFER, defRender_FBO ) );
	//
	// Create the gbuffer textures and depth texture
	GL_ASSERT ( glGenTextures ( GBUFFER_NUM_TEXTURES, id_textures ) );

	GL_ASSERT ( glGenTextures ( 1, &id_depthTexture ) );
	//
	// Create the textures for position, normal and color
	gl_createGBufTex ( GL_TEXTURE0, GL_RGBA32F,  	id_textures[GBUFFER_TEXTURE_TYPE_POSITION], screenWidth, screenHeight, GL_RGBA ); // Position
	gl_createGBufTex ( GL_TEXTURE1, GL_RGBA16F,  	id_textures[GBUFFER_TEXTURE_TYPE_NORMAL],   screenWidth, screenHeight, GL_RGBA ); // Normal
	gl_createGBufTex ( GL_TEXTURE2, GL_RGBA8, 	id_textures[GBUFFER_TEXTURE_TYPE_DIFFUSE],  screenWidth, screenHeight, GL_RGBA ); // Color
	//
	// Create depthTexture - uses GL_DEPTH instead of RGB color
	GL_ASSERT ( glBindTexture ( GL_TEXTURE_2D, id_depthTexture ) );
	GL_ASSERT ( glTexImage2D ( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL ) );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	GL_CHECK ( glFramebufferTexture2D ( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id_depthTexture, 0 ) );
	//
	// Attach the textures to the framebuffer
	GL_ASSERT ( glFramebufferTexture2D   ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_POSITION], 0 ) );
	GL_ASSERT ( glFramebufferTexture2D   ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_NORMAL], 0 ) );
	GL_ASSERT ( glFramebufferTexture2D   ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, id_textures[GBUFFER_TEXTURE_TYPE_DIFFUSE], 0 ) );
	GL_ASSERT ( glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, id_depthTexture ) );

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
	                         GL_COLOR_ATTACHMENT1,
	                         GL_COLOR_ATTACHMENT2,
	                       };

	GL_CHECK ( glDrawBuffers ( GBUFFER_NUM_TEXTURES, DrawBuffers ) );

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
			con_print ( CON_ERROR, true, "Error: Failed to create GBuffers - status [ 0x%x ]", Status );
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
// Which buffer will we read from
void gl_setReadBuffer ( GBUFFER_TEXTURE_TYPE TextureType )
//-----------------------------------------------------------------------------
{
	GL_ASSERT ( glReadBuffer ( GL_COLOR_ATTACHMENT0 + TextureType ) );
}

//-----------------------------------------------------------------------------
//
// Clean up FBO and allocated GBuffers
void gl_stopDefRender()
//-----------------------------------------------------------------------------
{
	if ( defRender_FBO != 0 )
		{
			glDeleteFramebuffers ( 1, &defRender_FBO );
		}

	if ( id_textures[0] != 0 )
		{
			glDeleteTextures ( ARRAY_SIZE_IN_ELEMENTS ( id_textures ), id_textures );
		}

	if ( id_depthTexture != 0 )
		{
			glDeleteTextures ( 1, &id_depthTexture );
		}
}

//-----------------------------------------------------------------------------
//
// Show GBuffers for debugging
void gl_showGBuffers()
//-----------------------------------------------------------------------------
{
	gl_bindForReading();

	GLint halfWidth = winWidth / 2.0f;
	GLint halfHeight = winHeight / 2.0f;

	gl_setFontColor ( 1.0, 1.0, 1.0, 1.0 );

	//
	// Bottom left
	GL_ASSERT ( gl_setReadBuffer ( GBUFFER_TEXTURE_TYPE_DIFFUSE ) );
	GL_ASSERT ( glBlitFramebuffer ( 0, 0, winWidth, winHeight, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR ) );
	ttf_addText ( FONT_SMALL, 0.0f, winHeight - 40, "DIFFUSE" );
	//
	// Top Left
	GL_ASSERT ( gl_setReadBuffer ( GBUFFER_TEXTURE_TYPE_POSITION ) );
	GL_ASSERT ( glBlitFramebuffer ( 0, 0, winWidth, winHeight, 0, halfHeight, halfWidth, winHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR ) );
	ttf_addText ( FONT_SMALL, 0.0f, 5.0f, "POSITION" );
	//
	// Top right
	GL_ASSERT ( gl_setReadBuffer ( GBUFFER_TEXTURE_TYPE_NORMAL ) );
	GL_ASSERT ( glBlitFramebuffer ( 0, 0, winWidth, winHeight, halfWidth, halfHeight, winWidth, winHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR ) );
	ttf_addText ( FONT_SMALL, halfWidth, 5.0f, "NORMALS" );
	//
	// Bottom right
	lt_renderDepthQuad ( SHADER_DEPTHMAP );
	gl_setFontColor ( 1.0, 1.0, 0.0, 1.0 );
	ttf_addText ( FONT_SMALL, halfWidth, winHeight - 40.0f, "DEPTH" );
}


//-----------------------------------------------------------------------------
//
// Convert OpenGL internal format enum to string
std::string convertInternalFormatToString ( GLenum format )
//-----------------------------------------------------------------------------
{
	std::string formatName;

	switch ( format )
		{
		case GL_STENCIL_INDEX:      // 0x1901
			formatName = "GL_STENCIL_INDEX";
			break;

		case GL_DEPTH_COMPONENT:    // 0x1902
			formatName = "GL_DEPTH_COMPONENT";
			break;

		case GL_ALPHA:              // 0x1906
			formatName = "GL_ALPHA";
			break;

		case GL_RGB:                // 0x1907
			formatName = "GL_RGB";
			break;

		case GL_RGBA:               // 0x1908
			formatName = "GL_RGBA";
			break;

		/*    case GL_LUMINANCE:          // 0x1909
		        formatName = "GL_LUMINANCE";
		        break;
		    case GL_LUMINANCE_ALPHA:    // 0x190A
		        formatName = "GL_LUMINANCE_ALPHA";
		        break;
		  */
		case GL_R3_G3_B2:           // 0x2A10
			formatName = "GL_R3_G3_B2";
			break;

		/*
		    case GL_ALPHA4:             // 0x803B
		        formatName = "GL_ALPHA4";
		        break;
		    case GL_ALPHA8:             // 0x803C
		        formatName = "GL_ALPHA8";
		        break;
		    case GL_ALPHA12:            // 0x803D
		        formatName = "GL_ALPHA12";
		        break;
		    case GL_ALPHA16:            // 0x803E
		        formatName = "GL_ALPHA16";
		        break;
		    case GL_LUMINANCE4:         // 0x803F
		        formatName = "GL_LUMINANCE4";
		        break;
		    case GL_LUMINANCE8:         // 0x8040
		        formatName = "GL_LUMINANCE8";
		        break;
		    case GL_LUMINANCE12:        // 0x8041
		        formatName = "GL_LUMINANCE12";
		        break;
		    case GL_LUMINANCE16:        // 0x8042
		        formatName = "GL_LUMINANCE16";
		        break;
		    case GL_LUMINANCE4_ALPHA4:  // 0x8043
		        formatName = "GL_LUMINANCE4_ALPHA4";
		        break;
		    case GL_LUMINANCE6_ALPHA2:  // 0x8044
		        formatName = "GL_LUMINANCE6_ALPHA2";
		        break;
		    case GL_LUMINANCE8_ALPHA8:  // 0x8045
		        formatName = "GL_LUMINANCE8_ALPHA8";
		        break;
		    case GL_LUMINANCE12_ALPHA4: // 0x8046
		        formatName = "GL_LUMINANCE12_ALPHA4";
		        break;
		    case GL_LUMINANCE12_ALPHA12:// 0x8047
		        formatName = "GL_LUMINANCE12_ALPHA12";
		        break;
		    case GL_LUMINANCE16_ALPHA16:// 0x8048
		        formatName = "GL_LUMINANCE16_ALPHA16";
		        break;
		    case GL_INTENSITY:          // 0x8049
		        formatName = "GL_INTENSITY";
		        break;
		    case GL_INTENSITY4:         // 0x804A
		        formatName = "GL_INTENSITY4";
		        break;
		    case GL_INTENSITY8:         // 0x804B
		        formatName = "GL_INTENSITY8";
		        break;
		    case GL_INTENSITY12:        // 0x804C
		        formatName = "GL_INTENSITY12";
		        break;
		    case GL_INTENSITY16:        // 0x804D
		        formatName = "GL_INTENSITY16";
		        break;
		        */
		case GL_RGB4:               // 0x804F
			formatName = "GL_RGB4";
			break;

		case GL_RGB5:               // 0x8050
			formatName = "GL_RGB5";
			break;

		case GL_RGB8:               // 0x8051
			formatName = "GL_RGB8";
			break;

		case GL_RGB10:              // 0x8052
			formatName = "GL_RGB10";
			break;

		case GL_RGB12:              // 0x8053
			formatName = "GL_RGB12";
			break;

		case GL_RGB16:              // 0x8054
			formatName = "GL_RGB16";
			break;

		case GL_RGBA2:              // 0x8055
			formatName = "GL_RGBA2";
			break;

		case GL_RGBA4:              // 0x8056
			formatName = "GL_RGBA4";
			break;

		case GL_RGB5_A1:            // 0x8057
			formatName = "GL_RGB5_A1";
			break;

		case GL_RGBA8:              // 0x8058
			formatName = "GL_RGBA8";
			break;

		case GL_RGB10_A2:           // 0x8059
			formatName = "GL_RGB10_A2";
			break;

		case GL_RGBA12:             // 0x805A
			formatName = "GL_RGBA12";
			break;

		case GL_RGBA16:             // 0x805B
			formatName = "GL_RGBA16";
			break;

		case GL_DEPTH_COMPONENT16:  // 0x81A5
			formatName = "GL_DEPTH_COMPONENT16";
			break;

		case GL_DEPTH_COMPONENT24:  // 0x81A6
			formatName = "GL_DEPTH_COMPONENT24";
			break;

		case GL_DEPTH_COMPONENT32:  // 0x81A7
			formatName = "GL_DEPTH_COMPONENT32";
			break;

		case GL_DEPTH_STENCIL:      // 0x84F9
			formatName = "GL_DEPTH_STENCIL";
			break;

		case GL_RGBA32F:            // 0x8814
			formatName = "GL_RGBA32F";
			break;

		case GL_RGB32F:             // 0x8815
			formatName = "GL_RGB32F";
			break;

		case GL_RGBA16F:            // 0x881A
			formatName = "GL_RGBA16F";
			break;

		case GL_RGB16F:             // 0x881B
			formatName = "GL_RGB16F";
			break;

		case GL_DEPTH24_STENCIL8:   // 0x88F0
			formatName = "GL_DEPTH24_STENCIL8";
			break;

		default:
			formatName = "Unknown format";
		}

	return formatName;
}


//-----------------------------------------------------------------------------
//
// return texture parameters as string using glGetTexLevelParameteriv()
std::string getTextureParameters ( GLuint id )
//-----------------------------------------------------------------------------
{
	if ( glIsTexture ( id ) == GL_FALSE )
		return "Not texture object";

	int width, height, format;
	std::string formatName;
	GL_CHECK ( glBindTexture ( GL_TEXTURE_2D, id ) );
	glGetTexLevelParameteriv ( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );         // get texture width
	glGetTexLevelParameteriv ( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );       // get texture height
	glGetTexLevelParameteriv ( GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format ); // get texture internal format
	GL_CHECK ( glBindTexture ( GL_TEXTURE_2D, 0 ) );

	formatName = convertInternalFormatToString ( format );

	std::string ss;
	ss = formatName;

	return ss;
}


//-----------------------------------------------------------------------------
//
// return renderbuffer parameters as string using glGetRenderbufferParameteriv
std::string getRenderbufferParameters ( GLuint id )
//-----------------------------------------------------------------------------
{
	if ( glIsRenderbuffer ( id ) == GL_FALSE )
		return "Not Renderbuffer object";

	int width, height, format;
	std::string formatName;
	glBindRenderbuffer ( GL_RENDERBUFFER, id );
	glGetRenderbufferParameteriv ( GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width ); // get renderbuffer width
	glGetRenderbufferParameteriv ( GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height ); // get renderbuffer height
	glGetRenderbufferParameteriv ( GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format ); // get renderbuffer internal format
	glBindRenderbuffer ( GL_RENDERBUFFER, 0 );

	formatName = convertInternalFormatToString ( format );

	std::string ss;
	ss = formatName;

	return ss;
}


//-----------------------------------------------------------------------------
//
// print out the FBO infos
void printFramebufferInfo()
//-----------------------------------------------------------------------------
{

	con_print ( CON_INFO, true, "===== FBO STATUS =====" );

	// print max # of colorbuffers supported by FBO
	int colorBufferCount = 0;
	glGetIntegerv ( GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount );
	con_print ( CON_INFO, true, "Max Number of Color Buffer Attachment Points: [ %i ]",colorBufferCount );

	int objectType;
	int objectId;

	// print info of the colorbuffer attachable image
	for ( int i = 0; i < colorBufferCount; ++i )
		{
			glGetFramebufferAttachmentParameteriv ( GL_FRAMEBUFFER,
			                                        GL_COLOR_ATTACHMENT0+i,
			                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
			                                        &objectType );

			if ( objectType != GL_NONE )
				{
					glGetFramebufferAttachmentParameteriv ( GL_FRAMEBUFFER,
					                                        GL_COLOR_ATTACHMENT0+i,
					                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
					                                        &objectId );

					std::string formatName;

					con_print ( CON_INFO, true, "Color Attachment [ %i ]" );

					if ( objectType == GL_TEXTURE )
						{
							con_print ( CON_INFO, true, "GL_TEXTURE [ %s ]", getTextureParameters ( objectId ).c_str() );

						}

					else if ( objectType == GL_RENDERBUFFER )
						{
							con_print ( CON_INFO, true, "GL_RENDERBUFFER [ %s ]", getRenderbufferParameters ( objectId ).c_str() );
						}

					con_print ( CON_INFO, true, "objectType [ %i ]", objectType );
				}
		}

	// print info of the depthbuffer attachable image
	glGetFramebufferAttachmentParameteriv ( GL_FRAMEBUFFER,
	                                        GL_DEPTH_ATTACHMENT,
	                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
	                                        &objectType );

	if ( objectType != GL_NONE )
		{
			glGetFramebufferAttachmentParameteriv ( GL_FRAMEBUFFER,
			                                        GL_DEPTH_ATTACHMENT,
			                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
			                                        &objectId );

			con_print ( CON_INFO, true, "Depth Attachment" );

			switch ( objectType )
				{
				case GL_TEXTURE:
					con_print ( CON_INFO, true, "GL_TEXTURE [ %s ]", getTextureParameters ( objectId ).c_str() );
					break;

				case GL_RENDERBUFFER:
					con_print ( CON_INFO, true, "GL_RENDERBUFFER [ %s ]",getRenderbufferParameters ( objectId ).c_str() );
					break;
				}
		}

	// print info of the stencilbuffer attachable image
	glGetFramebufferAttachmentParameteriv ( GL_FRAMEBUFFER,
	                                        GL_STENCIL_ATTACHMENT,
	                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
	                                        &objectType );

	if ( objectType != GL_NONE )
		{
			glGetFramebufferAttachmentParameteriv ( GL_FRAMEBUFFER,
			                                        GL_STENCIL_ATTACHMENT,
			                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
			                                        &objectId );

			con_print ( CON_INFO, true, "Stencil Attachment" );

			switch ( objectType )
				{
				case GL_TEXTURE:
					con_print ( CON_INFO, true, "GL_TEXTURE [ %s ]", getTextureParameters ( objectId ).c_str() );
					break;

				case GL_RENDERBUFFER:
					con_print ( CON_INFO, true, "GL_RENDERBUFFER [ %s ]", getRenderbufferParameters ( objectId ).c_str() );
					break;
				}
		}
}
