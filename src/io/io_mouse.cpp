#include <hdr/system/s_events.h>
#include "s_bullet.h"
#include "s_globals.h"
#include "io_mouse.h"
#include "s_window.h"
#include "s_camera3.h"

bool                g_lockMouse = true;
int			        freelookMouseX;
int			        freelookMouseY;
float			    mouseSpeed;
bool                mouseButton1Down = false;
bool                mouseButton2Down = false;

//-----------------------------------------------------------------------------
//
// Grab the mouse
void io_grabMouse()
//-----------------------------------------------------------------------------
{
	al_grab_mouse (al_mainWindow);
}

//-----------------------------------------------------------------------------
//
// Release the mouse
void io_releaseMouse()
//-----------------------------------------------------------------------------
{
	al_ungrab_mouse ();
}

//-----------------------------------------------------------------------------
//
// Set the mouse position
void lib_setMousePos ( int newPosX, int newPosY )
//-----------------------------------------------------------------------------
{
	al_set_mouse_xy (al_mainWindow, newPosX, newPosY);

	freelookMouseX = 0;
	freelookMouseY = 0;
}

//-----------------------------------------------------------------------------
//
void lib_setMouseCursor ( bool showMouse )
//-----------------------------------------------------------------------------
{
	if ( showMouse )
		al_show_mouse_cursor (al_mainWindow);
	else
		al_hide_mouse_cursor (al_mainWindow);
}

//-----------------------------------------------------------------------------
//
// Get the mouse position
void lib_getMouseState ( int *posX, int *posY )
//-----------------------------------------------------------------------------
{
	ALLEGRO_MOUSE_STATE     mouseState;
	int                     tmpPosX, tmpPosY;

	al_get_mouse_state (&mouseState);

	tmpPosX = (winWidth / 2) - mouseState.x;
	tmpPosY = (winHeight / 2) - mouseState.y;

	*posX = tmpPosX;
	*posY = tmpPosY;
	//
	// Check the state of the first mouse button
	// If it's pressed, fire off a user event to register the press
	//
	if ( al_mouse_button_down (&mouseState, 1) )
	{
		if ( !mouseButton1Down )
		{
			mouseButton1Down = true;
			evt_sendEvent(USER_EVENT_MOUSE_BUTTON_DOWN, 0, 1, 0, 0, "");
		}
	}
	else
	{
		if (mouseButton1Down)
		{
			mouseButton1Down = false;
			evt_sendEvent (USER_EVENT_MOUSE_BUTTON_UP, 0, 1, 0, 0, "");
		}
	}
	//
	// Check Second mouse button
	//
	if ( al_mouse_button_down (&mouseState, 2) )
	{
		if ( !mouseButton2Down )
		{
			mouseButton2Down = true;
			evt_sendEvent(USER_EVENT_MOUSE_BUTTON_DOWN, 0, 2, 0, 0, "");
		}
	}
	else
	{
		if (mouseButton2Down)
		{
			mouseButton2Down = false;
			evt_sendEvent (USER_EVENT_MOUSE_BUTTON_UP, 0, 2, 0, 0, "");
		}
	}
}
