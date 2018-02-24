#include "s_globals.h"
#include "s_startup.h"
#include "s_render.h"
#include "s_runFrame.h"
#include "s_timing.h"

#include "s_defRender.h"

//-----------------------------------------------------------------------------
//
// Main function - called first - SDL does the winMain
int main(int argc, char *argv[])
//-----------------------------------------------------------------------------
{
	float       interpolation;
	double      singleTimeValue, frameTimeStart;
	int         loops;

	if (false == initAll())
		sys_shutdownToSystem();

	nextGameTick = glfwGetTime();

	while ( !quitProgram )
		{
			//
			// This is the game logic - which runs at a fixed rate of TICKS_PER_SECOND per second
			loops = 0;
			singleTimeValue = glfwGetTime();

			while (singleTimeValue > nextGameTick && loops < maxFrameSkip)
				{
					gameTickRun(interpolation);
					nextGameTick += skipTicks;
					loops++;
					sys_CalculateThinkFrameRate (singleTimeValue);
				}

			interpolation = float (singleTimeValue + skipTicks - nextGameTick) / float (skipTicks);
			//
			// draw all to the screen
			frameTimeStart = glfwGetTime();
			updateScreen(interpolation);
			frameTimeTaken = glfwGetTime() - frameTimeStart;

			sys_CalculateFrameRate (singleTimeValue);
		}

	sys_shutdownToSystem();
}

//-----------------------------------------------------------------------------
//
// Change game mode
void changeMode(int newMode)
//-----------------------------------------------------------------------------
{
	currentMode = newMode;
}
