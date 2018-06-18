#pragma once

extern bool                 g_lockMouse;
extern int			        freelookMouseX;
extern int			        freelookMouseY;
extern float			    mouseSpeed;

// Set the mouse position
void lib_setMousePos ( int newPosX, int newPosY );

//
void lib_setMouseCursor ( bool showMouse );

// Get the mouse position
void lib_getMouseState ( int *posX, int *posY );

// Entry to handle all mouse events
void io_handleMouseEvents(ALLEGRO_EVENT event);

// Release the mouse
void io_releaseMouse();

// Grab the mouse
void io_grabMouse();

