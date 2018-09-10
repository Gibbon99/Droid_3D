#include <hdr/system/s_events.h>
#include "s_globals.h"
#include "s_startup.h"
#include "s_render.h"
#include "s_runFrame.h"
#include "s_timing.h"
#include "io_mouse.h"
#include "s_defRender.h"

TwBar            *tweakBar;

const int TICKS_PER_SECOND = 30;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;


int loops;
float interpolation;

//-----------------------------------------------------------------------------
//
// Run main loop
//
// Fixed update frames per second
// Render as fast as possible
int main (int argc, char *argv[] )
//-----------------------------------------------------------------------------
{
	if ( !initAll())
		sys_shutdownToSystem ();

	Uint32 next_game_tick = SDL_GetTicks();

	while ( !quitProgram )
	{
		loops = 0;

		evt_handleEvents();

		while (SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP)
		{
			sys_gameTickRun ();
			next_game_tick += SKIP_TICKS;
			loops++;
			thinkFPS++;
		}

		interpolation = float( SDL_GetTicks() + SKIP_TICKS - next_game_tick ) / float( SKIP_TICKS );

		sys_displayScreen ( interpolation );    // TODO: Interpolate movements

		fps++;
	}

	sys_shutdownToSystem();
}

//-----------------------------------------------------------------------------
//
// Change game mode
void changeMode ( int newMode )
//-----------------------------------------------------------------------------
{
	static int previousMode = -1;

	if (-1 == newMode)
	{
		currentMode = previousMode;
		return;
	}

	previousMode = currentMode;

	if (newMode == MODE_PAUSE)
	{
		currentMode = MODE_PAUSE;
		return;
	}

	if (newMode == MODE_GAME)
	{
		if (true == g_lockMouse)
		{
			io_grabMouse ();
			lib_setMouseCursor (false);
		}
		evt_sendEvent (USER_EVENT_GAME, USER_EVENT_GAME_TIMER, USER_EVENT_GAME_TIMER_CONSOLE, USER_EVENT_GAME_TIMER_OFF, 0, glm::vec3(), glm::vec3(), "");
	}

	if (newMode == MODE_CONSOLE)
	{
		SDL_StartTextInput();

		if (true == g_lockMouse)
		{
			io_releaseMouse ();
			lib_setMouseCursor (true);
		}
		evt_sendEvent (USER_EVENT_GAME, USER_EVENT_GAME_TIMER, USER_EVENT_GAME_TIMER_CONSOLE, USER_EVENT_GAME_TIMER_ON, 0, glm::vec3(), glm::vec3(), "");
	}

	currentMode = newMode;
}
