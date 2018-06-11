#include <AntTweakBar.h>
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
#include "s_antBar.h"
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

bool        drawWireframe = false;
bool        showGBuffers = false;

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

				// Draw GUI
				{
					PROFILE ( "GUI" );
					char        antParams[32];
					sprintf ( antParams, "Frame size='%i %i'", antBarWidth, antBarHeight );
					TwDefine ( antParams ); // resize bar

					sprintf ( antParams, "Frame position='%i %i'", antPosX, antPosY );
					TwDefine ( antParams ); // position bar
//				TwDraw();
				}

				gl_set2DMode();
				con_showConsole();
//				sdf_addText(FONT_SMALL,  glm::vec2{50.0f, 80.0f},  glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "Font Size [ %3.3f ]", sdf_getTextWidth(FONT_SMALL, "%s", "This is to test width."));

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
				lib_setMouseCursor ( false );

				sys_renderToFBO();

				bsp_renderLevel ( cam3_getCameraPosition(), SHADER_GEOMETRY_PASS );

				if ( true == g_debugPhysics )
					bul_drawDebugWorld();

				if ( true == g_debugDoorTriggers )
					bspDrawAllDoorTriggerZones();

				obj_renderAllObjects ( SHADER_GEOMETRY_PASS );

				gam_drawBullets ( SHADER_GEOMETRY_PASS );

				par_renderParticles();

				if ( true == g_debugLightPos )
					{
						for ( int i = 0; i != numOfLights; i++ )
							{
								drawLightPos ( SHADER_COLOR, allLights[i].position );
								drawDebugLine ( allLights[i].position, gl_lightDir(), allLights[i].position, DRAW_LINE, 1000, true, 1.0f );
							}
					}

				glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
				glUseProgram ( 0 );

				if ( true == showGBuffers )
					gl_showGBuffers();

				glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
				glUseProgram ( 0 );

				wrapglEnable ( GL_DEPTH_TEST );
				wrapglDisable ( GL_CULL_FACE );

				if ( false == showGBuffers )
					{
						gl_bindForReading();
						lt_renderFullscreenQuad ( SHADER_DIR_LIGHT );

						lt_renderPointLights ( SHADER_POINT_LIGHT );
					}



//			TwDraw();
		}

	//
	// Render all text in VBO memory
	gl_set2DMode();
	sdf_addText(FONT_SMALL, glm::vec2{2.0f, winHeight - sdf_getTextHeight(FONT_LARGE)}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "CamPos [ %3.3f %3.3f %3.3f ] FPS [ %i ] ThinkFPS [ %i ] Frametime [ %3.3f ] Max [ %3.3f ]", 
				cam3_Position.x, cam3_Position.y, cam3_Position.z, fpsPrint, thinkFpsPrint, frameTimeTakenPrint, frameTimeTakenMax );

	sdf_addText(FONT_SMALL, glm::vec2{2.0f, winHeight - (sdf_getTextHeight(FONT_LARGE) * 2)}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "CamFront [ %3.3f %3.3f %3.3f ]",
	            cam3_Front.x, cam3_Front.y, cam3_Front.z );


	#if defined DEBUG
	if (g_memLeakLastRun == true)
		sdf_addText(FONT_LARGE, glm::vec2{winWidth - sdf_getTextWidth(FONT_LARGE, "%s", "MEM LEAK - DEBUG"), winHeight - sdf_getTextHeight(FONT_LARGE)}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "%s", "MEM LEAK - DEBUG" );
	else
		sdf_addText(FONT_LARGE, glm::vec2{winWidth - sdf_getTextWidth(FONT_LARGE, "%s", "DEBUG"), winHeight - sdf_getTextHeight(FONT_LARGE)}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "%s", "DEBUG" );
	#endif
	
	sdf_displayText();

	lib_swapBuffers();
}
