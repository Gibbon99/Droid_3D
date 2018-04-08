#include "s_globals.h"

double          nextGameTick;		// How long did it take to draw the frame
double          frameTimeTakenPrint, frameTimeTaken;
double          frameTimeTakenMin, frameTimeTakenAvg, frameTimeTakenMax, frameTimeTakenCount, frameTimeTakenTotal;
int             ticksPerSecond, maxFrameSkip, thinkFpsPrint, fpsPrint;
float           skipTicks;

//-----------------------------------------------------------------------------
//
// Init timing variables
void sys_initTimingVars()
//-----------------------------------------------------------------------------
{
	frameTimeTakenCount = 0;
	frameTimeTakenTotal = 0;
	frameTimeTakenMin = 1000;
	frameTimeTakenMax = 0;
	frameTimeTakenAvg = 0;

	ticksPerSecond = 60;
	skipTicks = ( 1000.0f / ticksPerSecond ) * 0.001f;;
	maxFrameSkip = 2;
	nextGameTick = 0;
}


//-----------------------------------------------------------------------------
//
// Calculate the frame rate for the capped portion
void sys_CalculateThinkFrameRate ( double timeValue )
//-----------------------------------------------------------------------------
{
	static int	       thinkFps = 0;
	static double	   thinkTick1 = 0.0f;
	static double	   thinkTick2 = 0.0f;
	static float	   thinkDelayCounter = 0.0f;
	static float	   thinkInterval = 0.0f;

	thinkTick2 = timeValue;

	thinkInterval += ( ( thinkTick2 - thinkTick1 ) - thinkInterval ) * 0.1f;

	thinkTick1 = thinkTick2;

	thinkFps++;

	thinkDelayCounter += 1.0f * thinkInterval;

	if ( thinkDelayCounter > 1.0f )
		{
			thinkDelayCounter = 0.0f;
			thinkFpsPrint = thinkFps;
			thinkFps = 0;
		}
}

//-----------------------------------------------------------------------------
//
// This function calculates the overall frame rate and time taken to draw a frame
void sys_CalculateFrameRate ( double timeValue )
//-----------------------------------------------------------------------------
{
	static int	    fps = 0;	// Remove global fps variable
	static double	renderTick1 = 0.0f;
	static double	renderTick2 = 0.0f;
	static float	tickDelayCounter = 0.0f;
	static float	renderInterval = 0.0f;

	renderTick2 = timeValue;

	renderInterval += ( ( renderTick2 - renderTick1 ) - renderInterval ) * 0.1f;

	renderTick1 = renderTick2;

// Increase the frame counter
	fps++;
	frameTimeTakenCount++;
	frameTimeTakenTotal += frameTimeTaken;

	if ( frameTimeTaken < frameTimeTakenMin )
		frameTimeTakenMin = frameTimeTaken;

	if ( frameTimeTaken > frameTimeTakenMax )
		frameTimeTakenMax = frameTimeTaken;

	frameTimeTakenAvg = frameTimeTakenTotal / frameTimeTakenCount;

	tickDelayCounter += 1.0f * renderInterval;

	if ( tickDelayCounter > 1.0f )
		{
			tickDelayCounter = 0.0f;
			fpsPrint = fps;     // Value to display
			fps = 0;
			frameTimeTakenPrint = frameTimeTaken;			
//            sysAddAverageToGraph(frameTimeTakenPrint);
		}
}
