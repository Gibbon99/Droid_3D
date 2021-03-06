#include "s_globals.h"
#include "s_window.h"

#include "s_ttfFont.h"
#include "s_physics.h"
#include "s_shadowMap.h"
#include "s_defRender.h"
#include "io_textures.h"
#include "s_loadBsp.h"
#include "s_shaderLights.h"
#include "s_leakDetector.h"
#include "s_events.h"
#include "s_audio.h"

//-----------------------------------------------------------------------------
//
// Clean up and shutdown everything
void sys_shutdownToSystem()
//-----------------------------------------------------------------------------
{
	evt_sendEvent(USER_EVENT_AUDIO, AUDIO_STOP_ALL, 0, 0, 0, glm::vec3(), glm::vec3(), "");
	bsp_freeLightArray();
//	gl_freeShadowMap();
	gl_stopDefRender();
	io_freeTextureArray();
	bsp_freeMem();
	bul_stopPhysics();
	evt_sendEvent(USER_EVENT_AUDIO, AUDIO_STOP_ENGINE, 0, 0, 0, glm::vec3(), glm::vec3(), "");
	lib_destroyWindow();
	gl_displayErrors();
	io_closeLogFile();
	sys_reportMemLeak ( "leakReport.txt" );
	runThreads = false;
	SDL_Delay(1000);
	evt_shutdownMutex();
//	exit(EXIT_SUCCESS);
}

//-----------------------------------------------
// Display a message box indicating non-fatal error
void sysErrorNormal ( const char *sourcefile, int sourceline, const char *text, ... )
// ---------------------------------------------------------------------------
{
	static		char textBuffer[MAX_STRING_SIZE];
	static		char tempBuffer[MAX_STRING_SIZE];
	va_list		args;

	va_start ( args, text );
	vsprintf ( textBuffer, text, args );
	va_end ( args );

	if ( strlen ( textBuffer ) > MAX_STRING_SIZE - 1 )
		strcpy ( textBuffer, "The string passed to ErrorNormal is too long." );

	sprintf ( tempBuffer, "File: [ %s ] Line: [ %i ] - %s", sourcefile, sourceline, textBuffer );
#if defined(WIN32)
	MessageBox ( NULL, tempBuffer, "Error", MB_OK | MB_ICONWARNING );
#endif
	io_logToFile ( tempBuffer );
}

// ---------------------------------------------------------------------------
// Display a message box indicating a fatal error
void sysErrorFatal ( const char *sourcefile, int sourceline, const char *text, ... )
// ---------------------------------------------------------------------------
{
	static		char textBuffer[MAX_STRING_SIZE];
	static		char tempBuffer[MAX_STRING_SIZE];
	va_list		args;

	va_start ( args, text );
	vsprintf ( textBuffer, text, args );
	va_end ( args );

	if ( strlen ( textBuffer ) > MAX_STRING_SIZE - 1 )
		strcpy ( textBuffer, "The string passed to ErrorFatal is too long." );

	// TODO: Fix overflow

	sprintf ( tempBuffer, "File: [ %s ] Line: [ %i ] - %s", sourcefile, sourceline, textBuffer );
#if defined(WIN32)
	MessageBox ( NULL, textBuffer, "Error", MB_OK | MB_ICONWARNING | MB_ICONSTOP | MB_ICONERROR );
#endif

	io_logToFile ( tempBuffer );
}
