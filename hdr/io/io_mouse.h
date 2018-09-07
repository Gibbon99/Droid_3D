#pragma once

extern bool                 g_lockMouse;
extern int			        freelookMouseX;
extern int			        freelookMouseY;
extern float			    mouseSpeed;

// Handle a mouse user event - called by thread
int io_processMouseUserEvent ( void *ptr );

// Set the mouse position
void lib_setMousePos ( int newPosX, int newPosY );

//
void lib_setMouseCursor ( bool showMouse );

// Get the mouse position
void lib_handleMouseMotion ( int posX, int posY );

// Handle a mouse button press
void lib_handleMouseButton(int whichButton, int buttonState);

// Release the mouse
void io_releaseMouse();

// Grab the mouse
void io_grabMouse();

