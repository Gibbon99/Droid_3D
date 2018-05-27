#include "s_globals.h"
#include "s_startup.h"
#include "s_render.h"
#include "s_runFrame.h"
#include "s_timing.h"

#include "s_defRender.h"


#define MAXIMUM_FRAME_RATE 60
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)
	
int main ( int argc, char *argv[] )
{
	double			frameTimeStart;
	double			currentTime;
	double			updateIterations;
	static double	lastFrameTime = 0.0;
	static double	cyclesLeftOver = 0.0;

	if ( false == initAll() )
		sys_shutdownToSystem();

	while ( !quitProgram )
		{
			currentTime = glfwGetTime();
			updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);

			if (updateIterations > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL))
				{
					updateIterations = (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL);
				}
				
			while (updateIterations > UPDATE_INTERVAL)
				{
					updateIterations -= UPDATE_INTERVAL;
					//
					// Update game state a variable number of times
					sys_gameTickRun(UPDATE_INTERVAL ); 
					sys_CalculateThinkFrameRate ( currentTime );
				}

			cyclesLeftOver = updateIterations;
			lastFrameTime = currentTime;

			frameTimeStart = glfwGetTime();
			//
			// Draw the scene only once
			sys_displayScreen ( UPDATE_INTERVAL );
			frameTimeTaken = glfwGetTime() - frameTimeStart;

			sys_CalculateFrameRate ( currentTime );
		}
		sys_shutdownToSystem();
}


/*
//-----------------------------------------------------------------------------
//
// Main function - called first
int main ( int argc, char *argv[] )
//-----------------------------------------------------------------------------
{
	float       interpolation;
	double      singleTimeValue, frameTimeStart;
	int         loops;

	if ( false == initAll() )
		sys_shutdownToSystem();

	nextGameTick = glfwGetTime();

	while ( !quitProgram )
		{
			//
			// This is the game logic - which runs at a fixed rate of TICKS_PER_SECOND per second
			loops = 0;
			singleTimeValue = glfwGetTime();

			while ( singleTimeValue > nextGameTick && loops < maxFrameSkip )
				{
					sys_gameTickRun ( interpolation );
					nextGameTick += skipTicks;
					loops++;
					sys_CalculateThinkFrameRate ( singleTimeValue );
				}

			interpolation = float ( singleTimeValue + skipTicks - nextGameTick ) / float ( skipTicks );
			//
			// draw all to the screen
			frameTimeStart = glfwGetTime();
			sys_displayScreen ( interpolation );
			frameTimeTaken = glfwGetTime() - frameTimeStart;

			sys_CalculateFrameRate ( singleTimeValue );
		}

	sys_shutdownToSystem();
}
*/
//-----------------------------------------------------------------------------
//
// Change game mode
void changeMode ( int newMode )
//-----------------------------------------------------------------------------
{
	currentMode = newMode;
}
