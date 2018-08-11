#include <hdr/system/s_camera3.h>
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
#include "s_physics.h"
#include "s_shadowMap.h"
#include "io_textures.h"
#include "s_defRender.h"
#include "s_assimp.h"
#include "s_loadBsp.h"
#include "s_objects.h"
#include "s_signedFont.h"
#include "s_particles.h"
#include "s_events.h"
#include "s_audio.h"
#include "glad.h"

//#define GLAD_DEBUG 1

#ifdef GLAD_DEBUG

typedef void (* GLADcallback)(const char *name, void *funcptr, int len_args, ...);
//
// Sets a callback which will be called before every function call to a function loaded by glad.
void pre_gl_call(const char *name, void *funcptr, int len_args, ...)
	{
   // printf("Calling: %s (%d arguments)\n", name, len_args);
	}

#endif

//-----------------------------------------------------------------------------
//
// Init all variables and systems here
bool initAll()
//-----------------------------------------------------------------------------
{
	if ( !io_startLogFile ("logfile.log" ))
		sys_shutdownToSystem();

	con_initConsole();
	
	if ( sys_checkMemLeak("leakReport.txt" ))
	{
		con_print(CON_INFO, true, "No memory leak from last run.");
		g_memLeakLastRun = false;
	}
	else
	{
		con_print(CON_ERROR, true, "MEMORY LEAK: Check logfile for details.");
		g_memLeakLastRun = true;
	}

	sys_initTimingVars();

	if ( !lib_openWindow())
	{
		io_logToFile("Error: Error with starting windowing system.");
		sys_shutdownToSystem();
	}

	if (!gladLoadGL ())
	{
		con_print(CON_ERROR, true, "Failed to initialize GLAD");
		return false;
	}

#ifdef GLAD_DEBUG
	// before every opengl call call pre_gl_call
    glad_set_pre_callback(pre_gl_call);
    // don't use the callback for glClear
    // (glClear could be replaced with your own function)
    //glad_debug_glClear = glad_glClear;
#endif

	con_print(CON_INFO, true, "OpenGL version [ %d.%d ]", GLVersion.major, GLVersion.minor);
	if (GLVersion.major < 2)
	{
		con_print(CON_ERROR, true, "Error: Your system doesn't support OpenGL >= 2.");
		return false;
	}

//-------------------------- CONSOLE IS RUNNING AFTER HERE ---------------------------

	// Start the packfile system
	if ( !io_startFileSystem())
		sys_shutdownToSystem();

	for ( int i = 0; i != NUM_SHADERS; i++ )
		{
			if ( !gl_loadCompileShaders (i ))
				{
					con_print ( CON_TEXT, true, "ERROR: Compile error with shader [ %s ]", shaderProgram[i].fragFileName );
					return false;
				}
			else
				{
					con_print ( CON_TEXT, true, "Compiled shader [ %s ] - [ %s ] - ID [ %i ]", shaderProgram[i].fragFileName, shaderProgram[i].vertFileName, shaderProgram[i].programID );
				}
		}

	lib_getVersion();

	if ( !util_startScriptEngine())
		{
			printf ( "Error: Unable to start scripting engine.\n" );
			//sys_errorFatal("sys_runOnce()", __LINE__, "Unable to start the scripting engine.\n");
			return false;
		}

	util_registerVariables();
	util_registerFunctions();
	util_loadAndCompileScripts();
	util_cacheFunctionIDs();

	util_executeScriptFunction ( "scr_setGameVariables", "" );
	util_executeScriptFunction ( "scr_addAllScriptCommands", "" );

	if ( bul_startPhysics())
			con_print ( CON_INFO, true, "Physics system started..." );
	else
		{
			con_print ( CON_ERROR, true, "Physics system failed to start." );
			sysErrorFatal ( __FILE__, __LINE__, "Physics system failed." );
		}

	if ( gl_initDefRender (winWidth, winHeight ))
			con_print ( CON_INFO, true, "Forward rendering framebuffer created..." );
	else
		{
			gl_stopDefRender();
			con_print ( CON_ERROR, true, "Forward rendering framebuffer FAILED." );
			sysErrorFatal ( __FILE__, __LINE__, "Forward rendering framebuffer failed." );
		}

	bsp_loadBSP ( "volTest.bsp", true );
	par_initParticleSystem();
	//
	// Load Models
	util_executeScriptFunction ( "scr_loadAllModels", "" );
	ass_loadModelTextures();

	if ( io_loadAllTextures())
		con_print ( CON_INFO, true, "Loaded textures." );
	else
		con_print ( CON_ERROR, true, "Error loading one or more textures." );

	gl_registerDebugCallback();

	obj_addSomeObjects();
	//
	// Start Audio Engine and associated thread
	if (as_useSound)
		evt_sendEvent (USER_EVENT_AUDIO, AUDIO_INIT_ENGINE, 0, 0, 0, "");
	//
	// Start the timer to animate the console cursor
	evt_sendEvent (USER_EVENT_TIMER, EVENT_TIMER_CONSOLE_CURSOR, START_CONSOLE_CURSOR, 0, 0, "");

	blendIndexOne = 0;
	blendIndexTwo = 0;

	blendOne = blends[blendIndexOne];
	blendTwo = blends[blendIndexTwo];

	if (!shd_shadowMapInit(1024, 1024))
	{
		con_print(CON_ERROR, true, "Error: Could not start shadow map buffers.");
		sys_shutdownToSystem ();
	}

	cam3_initCamera ( glm::vec3{0.0f, 0.0f, 3.0f} );
	bul_setPlayerPosition(glm::vec3{0.0f, 0.0f, 3.0f}, glm::vec3{0.0f, 0.0f, 0.0f});

	return true;
}
