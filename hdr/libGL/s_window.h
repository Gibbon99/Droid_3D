#pragma once

#include "s_globals.h"

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
extern SDL_Window          *mainWindow;

// Switch locking mouse to middle of the screen
extern bool         g_lockMouse;

// Swap buffers for displaying screen
void lib_swapBuffers();

// Start the GL Helper library and open the window
bool lib_openWindow();

// Shutdown the library windowing system
void lib_destroyWindow();

// Get the version of the window library
bool lib_getVersion();

// Get the mouse position
void lib_getMousePos ( double *posX, double *posY );

// Set the mouse position
void lib_setMousePos ( int newPosX, int newPosY );

// Resize the current displayed window
void lib_resizeWindow ( int newWidth, int newHeight );

//
void lib_setMouseCursor ( bool showMouse );
