#include <AntTweakBar.h>
#include <hdr/bsp/s_lightMaps.h>
#include <hdr/bsp/s_shaderLights.h>
#include "s_globals.h"
#include "s_vsProfileLib.h"
#include "s_fontUtil.h"
#include "s_bmpFont.h"
#include "s_ttfFont.h"
#include "s_timing.h"
#include "s_window.h"
#include "s_shaders.h"

#include "s_openGL.h"
#include "s_maths.h"
#include "s_camera.h"
#include "s_openGLWrap.h"
#include "s_physics.h"
#include "s_shadowMap.h"
#include "io_textures.h"
#include "s_assimp.h"
#include "s_defRender.h"
#include "s_shaderLights.h"
#include "s_renderBSP.h"
#include "s_lightPass.h"
#include "s_bullet.h"
#include "s_doorsBSP.h"
#include "s_physicsDebug.h"
#include "s_objects.h"

#include "s_fontUtil.h"
#include "s_signedFont.h"
#include "s_camera3.h"
#include "s_particles.h"

bool        showGBuffers = false;
bool        g_renderBSP = true;

//-----------------------------------------------------------------------------
//
// Render all the models - pass in shader to use
void sys_renderModels ( int whichShader )
//-----------------------------------------------------------------------------
{
	//
	// Reset counter
	numSkippedModels = 0;
}

//-----------------------------------------------------------------------------
//
// Render models to the bound FBO textures
void sys_renderToFBO()
//-----------------------------------------------------------------------------
{
	glDepthMask ( true );
	glClearDepth ( 1.0f );
	//
	// Enable depth test
	wrapglEnable ( GL_DEPTH_TEST );
	glDepthFunc ( GL_LEQUAL );

	//
	// Disable blending
	wrapglDisable ( GL_BLEND );
	wrapglDisable ( GL_CULL_FACE );

	gl_set3DMode();

	gl_bindForWriting();

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	GL_CHECK ( glUseProgram ( shaderProgram[SHADER_GEOMETRY_PASS].programID ) );

	GL_CHECK ( glUniformMatrix4fv ( glGetUniformLocation ( shaderProgram[SHADER_GEOMETRY_PASS].programID, "u_viewProjectionMat" ), 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );

}

//-----------------------------------------------------------------------------
//
// Draw everything to the screen
void sys_displayScreen ( float interpolate )
//-----------------------------------------------------------------------------
{
//    char profileText[64];

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	glViewport ( 0, 0, winWidth, winHeight );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	switch ( currentMode )
		{
			case MODE_CONSOLE:
			{

				lib_setMouseCursor ( true );
				PROFILE ( "Frame" );

				gl_set2DMode();
				con_showConsole();

//#define SHOW_PROFILE_STATS 1

#ifdef SHOW_PROFILE_STATS
				std::string s = VSProfileLib::DumpLevels();

				char tempString[128];
				int j = 0;
				int posY = 300;
				int yCount = 0;
				strcpy ( tempString, "" );

				for ( int i = 0; i != s.length(); i++ )
					{
						tempString[j++] = s.c_str() [i];

						if ( s.c_str() [i] == '\n' )
							{
								j = 0;
								yCount++;
								sdf_addText ( FONT_SMALL, glm::vec2{50, posY - ( sdf_getTextHeight(FONT_SMALL) * yCount )}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "%s", tempString );
							}
					}

#endif
			}	// end of case
			break;

			case MODE_GAME:
			case MODE_PAUSE:

				/*
				shd_shadowMapDepthStartRender(cam3_getCameraPosition (), SHADER_DEPTH_SHADOWMAP);  // Create depth map from light viewpoint
				bsp_renderLevel (cam3_getCameraPosition (), SHADER_DEPTH_SHADOWMAP);


				shd_shadowRenderNormal(SHADER_SHADOWMAP, cam3_getCameraPosition ());
				bsp_renderLevel (cam3_getCameraPosition (), SHADER_SHADOWMAP);
*/


				sys_renderToFBO();

				if ( g_renderBSP )
				{
					if ( g_renderTextures )
					{
						bsp_prepareFaceRender (SHADER_GEOMETRY_PASS);
						bsp_renderLevel (cam3_getCameraPosition (), SHADER_GEOMETRY_PASS);
					}
					else
					{
						bsp_prepareFaceRender (SHADER_MODEL_PASS);
						bsp_renderLevel (cam3_getCameraPosition (), SHADER_MODEL_PASS);
					}
				}

//				bsp_renderLevel(cam3_getCameraPosition (), SHADER_DEPTH_SHADOWMAP);

//				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				// 2. render scene as normal
//				shd_shadowRenderNormal(SHADER_SHADOWMAP, allLights[0].position);
//				bsp_renderLevel(cam3_getCameraPosition (), SHADER_MODEL_PASS);

//				lt_renderDepthQuad ( SHADER_DEPTHMAP );



				if ( g_debugPhysics )
					bul_drawDebugWorld();

				if ( g_debugDoorTriggers )
					bsp_drawAllDoorTriggerZones ();

				if ( g_debugVolLights )
					bsp_showLightVolPositions ( SHADER_MODEL_PASS );

				obj_renderAllObjects ( SHADER_MODEL_PASS );

				lt_renderPointLights ( SHADER_MODEL_PASS );

				gam_drawBullets ( SHADER_MODEL_PASS );

				par_renderParticles();

				if ( !showGBuffers )
				{
					glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
					glUseProgram ( 0 );

					gl_bindForReading();
					lt_renderFullscreenQuad ( SHADER_DIR_LIGHT );

					lt_renderPointLights ( SHADER_POINT_LIGHT );
				}

		}

	//
	// Render all text in VBO memory
	gl_set2DMode();
	sdf_addText(FONT_SMALL, glm::vec2{2.0f, winHeight - sdf_getTextHeight(FONT_SMALL)}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "CamPos [ %3.3f %3.3f %3.3f ] FPS [ %i ] ThinkFPS [ %i ] Frametime [ %3.3f ] Max [ %3.3f ]",
				cam3_Position.x, cam3_Position.y, cam3_Position.z, fpsPrint, thinkFpsPrint, frameTimeTakenPrint, frameTimeTakenMax );

	sdf_addText(FONT_SMALL, glm::vec2{2.0f, winHeight - (sdf_getTextHeight(FONT_SMALL) * 2)}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "CamFront [ %3.3f %3.3f %3.3f ]",
	            cam3_Front.x, cam3_Front.y, cam3_Front.z );

/*
	printf("About to print recticle\n");

	sdf_addText (FONT_SMALL, glm::vec2{(winWidth / 2) - (sdf_getTextWidth (FONT_SMALL, "%s", "+")), (winHeight / 2) - sdf_getTextHeight (FONT_SMALL)}, glm::vec4{0.4f, 0.4f, 0.4f, 0.4f}, "%s", "--");

	printf("AFter recticle\n");
*/

	#ifdef DEBUG

	if ( g_memLeakLastRun )
		sdf_addText(FONT_MEDIUM, glm::vec2{winWidth - sdf_getTextWidth(FONT_MEDIUM, "%s", "MEM LEAK - DEBUG"), sdf_getTextHeight(FONT_MEDIUM)}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "%s", "MEM LEAK - DEBUG" );
	else
		sdf_addText(FONT_MEDIUM, glm::vec2{winWidth - sdf_getTextWidth(FONT_MEDIUM, "%s", "DEBUG"), sdf_getTextHeight(FONT_MEDIUM)}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "%s", "DEBUG" );

	#endif

	sdf_displayText();

//	gl_draw2DQuad(glm::vec2{winWidth / 2.0f, 0.0}, glm::vec2{winWidth / 2.0f, winHeight / 2.0f}, SHADER_QUAD_2D, shd_getDepthTextureID(), true);


	if ( showGBuffers )
		gl_showGBuffers();

	lib_swapBuffers();
}
