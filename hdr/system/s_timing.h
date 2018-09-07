#pragma once

extern double           frameTimeTaken;		// How long did it take to draw the frame
extern float           frameTimeTakenPrint;
extern float           frameTimeTakenMin, frameTimeTakenAvg, frameTimeTakenMax, frameTimeTakenCount, frameTimeTakenTotal;
extern int              ticksPerSecond, maxFrameSkip, thinkFpsPrint, fpsPrint;
extern float            skipTicks;
extern double           nextGameTick;

// Calculate the frame rate for the capped portion
void sys_CalculateThinkFrameRate ( Uint32 timeValue );

// This function calculates the overall frame rate and time taken to draw a frame
void sys_CalculateFrameRate ( Uint32 timeValue );

// Init timing variables
void sys_initTimingVars();
