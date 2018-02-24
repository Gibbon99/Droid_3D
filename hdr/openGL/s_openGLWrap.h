
#pragma once

#include "s_globals.h"

//----------------------------------------------------------------------------
//
// Wrap OpenGL calls to minmise state changes
//
//----------------------------------------------------------------------------

extern int wrapCountglBindTexture;
extern int wrapCountglBindTextureChanges;

//----------------------------------------------------------------------------
//
// Display openGLWrap counters
void wrapShowCounters(bool writeToLog);

// Warap glEnable
void wrapglEnable(int whichState);

// Wrap glDisable
void wrapglDisable(int whichState);

// Wrap glBindTexture
void wrapglBindTexture(int whichTextureUnit, int whichTexture);
