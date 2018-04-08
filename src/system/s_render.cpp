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

#include "s_camera3.h"
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

	ass_renderMesh ( MODEL_TANK,  whichShader, glm::vec3 ( 10.0, 35.0, 50.0 ), 0.3f, glm::vec3() );
	ass_renderMesh ( MODEL_TANK,  whichShader, glm::vec3 ( 50.0, 35.0, 100.0 ), 0.3f, glm::vec3() );
	ass_renderMesh ( MODEL_TANK,  whichShader, glm::vec3 ( 90.0, 35.0, 150.0 ), 0.3f, glm::vec3() );
	ass_renderMesh ( MODEL_CRATE, whichShader, glm::vec3 ( -150.0, 40.0, 10.0 ), 0.1f, glm::vec3() );
	ass_renderMesh ( MODEL_CRATE, whichShader, glm::vec3 ( -180.0, 50.0, 30.0 ), 0.1f, glm::vec3() );
	ass_renderMesh ( MODEL_CRATE, whichShader, glm::vec3 ( -210.0, 60.0, 50.0 ), 0.1f, glm::vec3() );

	gam_drawBullets ( whichShader );
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
void updateScreen ( float interpolate )
//-----------------------------------------------------------------------------
{
//    char profileText[64];

	glClearColor ( 0.0f, 0.0f, 0.6f, 0.0f );
	glViewport ( 0, 0, winWidth, winHeight );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//
	// Get memory to hold vertex and char information
	ttf_startText();

	switch ( currentMode )
		{
		case MODE_CONSOLE:
		{

			lib_setMouseCursor ( true );
			PROFILE ( "Frame" );

			gl_set2DMode();

			gl_setFontColor ( 0.7f, 0.7f, 0.0f, 0.5 );
			ttf_addText ( FONT_SMALL, 0.0f, 16.0f, "FPS [ %i ] ThinkFPS [ %i ] Frametime [ %3.3f ]", fpsPrint, thinkFpsPrint, frameTimeTakenPrint );

			gl_setFontColor ( 1.0f, 0.0f, 1.0f, 1.0f );
			gl_drawScreen ( );

			// Draw GUI
			{
				PROFILE ( "GUI" );
				char        antParams[32];
				sprintf ( antParams, "Frame size='%i %i'", antBarWidth, antBarHeight );
				TwDefine ( antParams ); // resize bar

				sprintf ( antParams, "Frame position='%i %i'", antPosX, antPosY );
				TwDefine ( antParams ); // position bar
				TwDraw();
			}


#ifdef SHOW_PROFILE_STATS
			std::string s = VSProfileLib::DumpLevels();

			char tempString[128];
			int j = 0;
			int posY = 300;
			int yCount = 0;
			strcpy ( tempString, "" );

			//    ttf_printString(FONT_SMALL, 300, 100, s.c_str());
			ttf_addText ( FONT_SMALL, 50, 200, s.c_str() );
			io_logToFile ( "Render \n %s", s.c_str() );


			for ( int i = 0; i != s.length(); i++ )
				{
					tempString[j++] = s.c_str() [i];
					if ( s.c_str() [i] == '\n' )
						{
							j = 0;
							yCount++;
							ttf_addText ( 0, 450, posY + ( conFontSize * yCount ), tempString );
						}
				}
#endif
		}	// end of case
		break;

		case MODE_GAME:
			lib_setMouseCursor ( false );

			sys_renderToFBO();

			bsp_renderLevel ( cam_getPosition(), SHADER_GEOMETRY_PASS );
			
			if ( true == g_debugPhysics )
				bul_drawDebugWorld();

			if ( true == g_debugDoorTriggers )
				bspDrawAllDoorTriggerZones();

			sys_renderModels ( SHADER_GEOMETRY_PASS );

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

#define LINE_SPACE 21.0f 
			gl_setFontColor ( 0.7f, 0.7f, 0.0f, 1.0f );
			ttf_addText ( FONT_SMALL, 0.0f, LINE_SPACE * 1, "FPS [ %i ] ThinkFPS [ %i ] Frametime [ %3.3f ] Average [ %3.3f ]", fpsPrint, thinkFpsPrint, frameTimeTakenPrint, frameTimeTakenAvg );
			ttf_addText ( FONT_SMALL, 0.0f, LINE_SPACE * 2, "cam3_Yaw [ %3.3f ] Pitch [ %3.3f ]", cam3_Yaw, cam3_Pitch);
			ttf_addText ( FONT_SMALL, 0.0f, LINE_SPACE * 3, "cam3_Position[ %3.3f %3.3f %3.3f ]", cam3_Position.x, cam3_Position.y, cam3_Position.z );
			ttf_addText ( FONT_SMALL, 0.0f, LINE_SPACE * 4, "cam3_Front [ %3.3f %3.3f %3.3f ]", cam3_Front.x, cam3_Front.y, cam3_Front.z );
		}
			//
			// Render all text in VBO memory
			gl_set2DMode();
			ttf_displayText ( FONT_SMALL );

			lib_swapBuffers();
		
}
