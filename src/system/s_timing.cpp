#include "s_globals.h"

double          frameTimeTakenPrint, frameTimeTaken;
double          frameTimeTakenMin, frameTimeTakenAvg, frameTimeTakenMax, frameTimeTakenCount, frameTimeTakenTotal;
int             thinkFpsPrint, fpsPrint;

int              ticksPerSecond, maxFrameSkip;
float            skipTicks;
double           nextGameTick;
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
}

//-----------------------------------------------------------------------------
//
// Calculate the frame rate for the capped portion
void sys_CalculateThinkFrameRate ( Uint32 timeValue )
//-----------------------------------------------------------------------------
{
	static int	       thinkFps = 0;
	static float	   thinkTick1 = 0.0f;
	static float	   thinkTick2 = 0.0f;
	static float	   thinkDelayCounter = 0.0f;
	static float	   thinkInterval = 0.0f;

	thinkTick2 = timeValue / 1000.0f;

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
void sys_CalculateFrameRate ( Uint32 timeValue )
//-----------------------------------------------------------------------------
{
	static int	    fps = 0;	// Remove global fps variable
	static double	renderTick1 = 0.0f;
	static double	renderTick2 = 0.0f;
	static float	tickDelayCounter = 0.0f;
	static float	renderInterval = 0.0f;

	renderTick2 = timeValue / 1000.0f;

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
			frameTimeTakenPrint = frameTimeTaken / 1000.0f; // convert to milliseconds?
//            sysAddAverageToGraph(frameTimeTakenPrint);
		}
}
