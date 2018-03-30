#include "s_timing.h"
#include "s_shutdown.h"
#include "s_window.h"
#include "s_bmpFont.h"
#include "io_fileSystem.h"
#include "s_fontUtil.h"
#include "s_shaders.h"
#include "s_ttfFont.h"
#include "s_conScript.h"
#include "s_camera.h"
#include "s_antBar.h"
#include "s_physics.h"
#include "s_shadowMap.h"
#include "io_textures.h"
#include "s_defRender.h"
#include "s_assimp.h"
#include "s_loadBsp.h"

//-----------------------------------------------------------------------------
//
// Init all variables and systems here
bool initAll()
//-----------------------------------------------------------------------------
{
	if ( false == io_startLogFile ( "logfile.log" ) )
		sys_shutdownToSystem();

	con_initConsole();

	sys_initTimingVars();

	if ( false == lib_openWindow() )
		sys_shutdownToSystem();

	if ( ogl_LOAD_FAILED == ogl_LoadFunctions() )
		{
			io_logToFile ( "Error: Failed to load openGL functions" );
			sys_shutdownToSystem();
		}

	lib_getVersion();

	// Start the packfile system
	if ( false == io_startFileSystem() )
		sys_shutdownToSystem();


	if ( false == util_startScriptEngine() )
		{
			printf ( "Error: Unable to start scripting engine\n" );
			//sys_errorFatal("sys_runOnce()", __LINE__, "Unable to start the scripting engine.\n");
			return false;
		}

	sys_registerVariables();
	util_registerFunctions();
	util_loadAndCompileScripts();
	util_cacheFunctionIDs();


	util_executeScriptFunction ( "scr_setGameVariables", "" );
	util_executeScriptFunction ( "scr_addAllScriptCommands", "" );

	for ( int i = 0; i != NUM_SHADERS; i++ )
		{
			if ( false == gl_loadCompileShaders ( i ) )
				{
					con_print ( CON_TEXT, true, "ERROR: Compile error with shader [ %s ]", shaderProgram[i].fragFileName );
					return false;

				}

			else
				{
					con_print ( CON_TEXT, true, "Compiled shader [ %s ] - [ %s ] - ID [ %i ]", shaderProgram[i].fragFileName, shaderProgram[i].vertFileName, shaderProgram[i].programID );
				}
		}

	//
	// TTF Library
	if ( false == ttf_initLibrary ( fontSizeSmall, 0 ) )
		{
			io_logToFile ( "Unable to start TTF Library. Missing file?" );
			sys_shutdownToSystem();
		}

	if ( true == io_loadAllTextures() )
		{
			con_print ( CON_INFO, true, "Loaded textures." );

		}
	else
		{
			con_print ( CON_ERROR, true, "Error loading one or more textures." );
		}

	//
	// Load Models
	util_executeScriptFunction ( "scr_loadAllModels", "" );
	ass_loadModelTextures();

	if ( true == bul_startPhysics() )
		{
			con_print ( CON_INFO, true, "Physics system started..." );
		}
	else
		{
			con_print ( CON_ERROR, true, "Physics system failed to start." );
			sysErrorFatal ( __FILE__, __LINE__, "Physics system failed." );
		}


	if ( true == gl_initDefRender ( winWidth, winHeight ) )
		{
			con_print ( CON_INFO, true, "Forward rendering framebuffer created..." );
		}
	else
		{
			gl_stopDefRender();
			con_print ( CON_ERROR, true, "Forward rendering framebuffer FAILED." );
			sysErrorFatal ( __FILE__, __LINE__, "Forward rendering framebuffer failed." );
//            sys_shutdownToSystem();
		}

	bsp_loadBSP ( "test.bsp", true );

	lib_initAntBar();

	gl_registerDebugCallback();

//	bul_setPlayerPosition(cam_getPosition(), glm::vec3(0.0, 0.0, 0.0));

	return true;
}
