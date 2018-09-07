#include <hdr/system/s_events.h>
#include <hdr/system/s_audio.h>
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

//----------------------------------------------------------------
//
// Handle a mouse user event - called by thread
int io_processMouseUserEvent ( void *ptr )
//----------------------------------------------------------------
{
	_myEventData tempEventData;

	while ( runThreads )
	{
		SDL_Delay(THREAD_DELAY_MS);

		if ( !mouseEventQueue.empty ())   // stuff in the queue to process
		{
			if ( SDL_LockMutex (mouseMutex) == 0 )
			{
				tempEventData = mouseEventQueue.front();
				mouseEventQueue.pop();
				SDL_UnlockMutex (mouseMutex);
			}

			switch ( tempEventData.eventAction )
			{
				case USER_EVENT_MOUSE_MOTION:
					lib_handleMouseMotion (tempEventData.data1, tempEventData.data2);
				break;

				case USER_EVENT_MOUSE_BUTTON_DOWN:
					lib_handleMouseButton (tempEventData.data1, USER_EVENT_MOUSE_BUTTON_DOWN);
					break;

				case USER_EVENT_MOUSE_BUTTON_UP:
					lib_handleMouseButton (tempEventData.data1, USER_EVENT_MOUSE_BUTTON_UP);
					break;

				default:
					break;
			}
		}
	}
	printf("MOUSE thread stopped.\n");
	return 0;
}

//-----------------------------------------------------------------------------
//
// Grab the mouse
void io_grabMouse()
//-----------------------------------------------------------------------------
{
	SDL_CaptureMouse(SDL_TRUE);
}

//-----------------------------------------------------------------------------
//
// Release the mouse
void io_releaseMouse()
//-----------------------------------------------------------------------------
{
	SDL_CaptureMouse(SDL_FALSE);
}

//-----------------------------------------------------------------------------
//
// Set the mouse position
void lib_setMousePos ( int newPosX, int newPosY )
//-----------------------------------------------------------------------------
{
	SDL_WarpMouseInWindow(mainWindow, newPosX, newPosY);

	freelookMouseX = 0;
	freelookMouseY = 0;
}

//-----------------------------------------------------------------------------
//
void lib_setMouseCursor ( bool showMouse )
//-----------------------------------------------------------------------------
{
	if ( showMouse )
		SDL_ShowCursor(SDL_ENABLE);
	else
		SDL_ShowCursor(SDL_DISABLE);
}

//-----------------------------------------------------------------------------
//
// Handle a mouse button press
void lib_handleMouseButton(int whichButton, int buttonState)
//-----------------------------------------------------------------------------
{
	if ( whichButton & SDL_BUTTON(SDL_BUTTON_LEFT)) // Button One
	{
		switch (buttonState)
		{
			case USER_EVENT_MOUSE_BUTTON_UP:
				mouseButton1Down = false;
				break;

			case USER_EVENT_MOUSE_BUTTON_DOWN:
				mouseButton1Down = true;
				// TODO: Make this an event otherwise it runs in this thread context
				//gam_createBullet (cam3_Front, cam3_Position, bullet_1_speed);
				evt_sendEvent (USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_LASER, 1, 0, "");
				break;

			default:
				break;
		}
	}

	if ( whichButton & SDL_BUTTON(SDL_BUTTON_RIGHT))    // Check Second mouse button
	{
		switch ( buttonState )
		{
			case USER_EVENT_MOUSE_BUTTON_UP:
				mouseButton2Down = false;
				break;

			case USER_EVENT_MOUSE_BUTTON_DOWN:
				mouseButton2Down = true;
				break;

			default:
				break;
		}
	}
}

//-----------------------------------------------------------------------------
//
// Get the mouse position
void lib_handleMouseMotion ( int posX, int posY )
//-----------------------------------------------------------------------------
{
	int     tmpPosX, tmpPosY;

	tmpPosX = (winWidth / 2) - posX;
	tmpPosY = (winHeight / 2) - posY;

	if ( SDL_LockMutex (mouseMotionMutex) == 0 )
	{
		freelookMouseX = tmpPosX;
		freelookMouseY = tmpPosY;
		SDL_UnlockMutex (mouseMotionMutex);
	}
	else
		return;     // Could not get lock
}
