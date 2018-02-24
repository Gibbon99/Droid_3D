#pragma once

extern double           frameTimeTaken;		// How long did it take to draw the frame
extern double           frameTimeTakenPrint;
extern double           frameTimeTakenMin, frameTimeTakenAvg, frameTimeTakenMax, frameTimeTakenCount, frameTimeTakenTotal;
extern int              ticksPerSecond, maxFrameSkip, thinkFpsPrint, fpsPrint;
extern float            skipTicks;
extern double           nextGameTick;

// Calculate the frame rate for the capped portion
void sys_CalculateThinkFrameRate(double timeValue);

// This function calculates the overall frame rate and time taken to draw a frame
void sys_CalculateFrameRate(double timeValue);

// Init timing variables
void sys_initTimingVars();
