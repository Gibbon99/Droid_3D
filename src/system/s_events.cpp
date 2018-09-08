#include "s_camera3.h"
#include "s_bullet.h"
#include "io_keyboard.h"
#include "s_globals.h"
#include "s_events.h"
#include <utility>
#include "s_events.h"
#include "s_lightMaps.h"
#include "s_window.h"
#include "io_mouse.h"
#include "s_audio.h"
#include "s_console.h"
#include "s_gameEvents.h"

SDL_TimerID timerCursorFlash;

Uint32      myEventType;

SDL_Thread *userEventConsoleThread;
SDL_Thread *userEventAudioThread;
SDL_Thread *userEventMouseThread;
SDL_Thread *userEventGameThread;

SDL_mutex *consoleMutex;
SDL_mutex *audioMutex;
SDL_mutex *mouseMutex;
SDL_mutex *mouseMotionMutex;
SDL_mutex *gameMutex;

queue<_myEventData>    consoleEventQueue;
queue<_myEventData>    audioEventQueue;
queue<_myEventData>    mouseEventQueue;
queue<_myEventData>    gameEventQueue;

bool     runThreads = true;

//------------------------------------------------------------------------
//
// Callback for timer function - cursor flash
Uint32 evt_cursorTimerCallback(Uint32 interval, void *param)
//------------------------------------------------------------------------
{
	con_processCursor ();

	return 0;
}

//------------------------------------------------------------------------
//
// Shutdown mutex
void evt_shutdownMutex()
//------------------------------------------------------------------------
{
	SDL_DestroyMutex (consoleMutex);
	SDL_DestroyMutex (audioMutex);
	SDL_DestroyMutex (mouseMutex);
	SDL_DestroyMutex (mouseMotionMutex);
	SDL_DestroyMutex (gameMutex);
}

//------------------------------------------------------------------------
//
// Setup user event source and event data
bool evt_registerUserEventSetup()
//------------------------------------------------------------------------
{
//	myEventType = SDL_RegisterEvents (1);   // Register user event type

	timerCursorFlash = SDL_AddTimer(500, evt_cursorTimerCallback, 0);   // Time in milliseconds

	userEventConsoleThread = SDL_CreateThread(con_processConsoleUserEvent, "userEventConsoleThread", (void *)NULL);
	if ( nullptr == userEventConsoleThread)
	{
		printf ("SDL_CreateThread - userEventConsoleThread - failed: %s\n", SDL_GetError () );
		return false;
	}

	userEventAudioThread = SDL_CreateThread(aud_processAudioEventQueue, "userEventAudioThread", (void *)NULL);
	if ( nullptr == userEventAudioThread)
	{
		printf ("SDL_CreateThread - userEventConsoleThread - failed: %s\n", SDL_GetError () );
		return false;
	}

	userEventMouseThread = SDL_CreateThread(io_processMouseUserEvent, "userEventMouseThread", (void *)NULL);
	if ( nullptr == userEventMouseThread)
	{
		printf ("SDL_CreateThread - userEventConsoleThread - failed: %s\n", SDL_GetError () );
		return false;
	}

	userEventGameThread = SDL_CreateThread(gam_processGameEventQueue, "userEventGameThread", (void *)NULL);
	if (nullptr == userEventGameThread)
	{
		printf ("SDL_CreateThread = userEventGameThread = failed: %s\n", SDL_GetError() );
		return false;
	}

	consoleMutex = SDL_CreateMutex();
	if (!consoleMutex)
	{
		con_print(CON_ERROR, true, "Couldn't create mutex - consoleMutex");
		return false;
	}

	audioMutex = SDL_CreateMutex();
	if (!audioMutex)
	{
		con_print(CON_ERROR, true, "Couldn't create mutex - audioMutex");
		return false;
	}

	mouseMutex = SDL_CreateMutex();
	if (!mouseMutex)
	{
		con_print(CON_ERROR, true, "Couldn't create mutex - mouseMutex");
		return false;
	}

	mouseMotionMutex = SDL_CreateMutex();
	if (!mouseMotionMutex)
	{
		con_print(CON_ERROR, true, "Couldn't create mutex - mouseMotionMutex");
		return false;
	}

	gameMutex = SDL_CreateMutex();
	if (!gameMutex)
	{
		con_print(CON_ERROR, true, "Couldn't create mutex - gameMutex");
		return false;
	}

	SDL_DetachThread(userEventConsoleThread);
	SDL_DetachThread(userEventAudioThread);
	SDL_DetachThread(userEventMouseThread);
	SDL_DetachThread(userEventGameThread);

	return true;
}

//------------------------------------------------------------------------
//
// Create a custom event to be sent
void evt_sendEvent(int type, int action, int data1, int data2, int data3, glm::vec3 vec3_1, glm::vec3 vec3_2, string textString)
//------------------------------------------------------------------------
{
	_myEventData       eventData;

	eventData.eventType = type;
	eventData.eventAction = action;
	eventData.data1 = data1;
	eventData.data2 = data2;
	eventData.data3 = data3;
	eventData.vec3_1 = vec3_1;
	eventData.vec3_2 = vec3_2;
	eventData.eventString = std::move (textString);

	switch (type)
	{
		case USER_EVENT_CONSOLE:
			if (SDL_LockMutex (consoleMutex) == 0)
			{
				consoleEventQueue.push(eventData);
				SDL_UnlockMutex (consoleMutex);
			}
			break;

		case USER_EVENT_AUDIO:
			if (SDL_LockMutex (audioMutex) == 0)
			{
				audioEventQueue.push(eventData);
				SDL_UnlockMutex (audioMutex);
			}
			break;

		case USER_EVENT_MOUSE:
			if (SDL_LockMutex (mouseMutex) == 0)
			{
				mouseEventQueue.push(eventData);
				SDL_UnlockMutex (mouseMutex);
			}
			break;

		case USER_EVENT_GAME:
			if (SDL_LockMutex (gameMutex) == 0)
			{
				gameEventQueue.push(eventData);
				SDL_UnlockMutex (gameMutex);
			}
			break;

		default:
			break;

	}
}

//------------------------------------------------------------------------
//
// Handle DISPLAY events
void evt_handleDisplayEvents(SDL_Event event)
//------------------------------------------------------------------------
{
	switch (event.window.event)
	{
		case SDL_WINDOWEVENT_SHOWN:
			con_print(CON_INFO, true, "Window %d shown", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_HIDDEN:
			con_print(CON_INFO, true, "Window %d hidden", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_EXPOSED:
			con_print(CON_INFO, true, "Window %d exposed", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_MOVED:
			con_print(CON_INFO, true, "Window %d moved to %d,%d",
			        event.window.windowID, event.window.data1,
			        event.window.data2);
			break;

		case SDL_WINDOWEVENT_RESIZED:
			con_print(CON_INFO, true, "Window %d resized to %dx%d",
			        event.window.windowID, event.window.data1,
			        event.window.data2);
			break;

		case SDL_WINDOWEVENT_SIZE_CHANGED:
			con_print(CON_INFO, true, "Window %d size changed to %dx%d",
			        event.window.windowID, event.window.data1,
			        event.window.data2);
			break;

		case SDL_WINDOWEVENT_MINIMIZED:
			con_print(CON_INFO, true, "Window %d minimized", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
			con_print(CON_INFO, true, "Window %d maximized", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_RESTORED:
			con_print(CON_INFO, true, "Window %d restored", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_ENTER:
			con_print(CON_INFO, true, "Mouse entered window %d", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_LEAVE:
			con_print(CON_INFO, true, "Mouse left window %d", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED:
			con_print(CON_INFO, true, "Window %d gained keyboard focus", event.window.windowID);
//			changeMode(-1);
			break;

		case SDL_WINDOWEVENT_FOCUS_LOST:
			con_print(CON_INFO, true, "Window %d lost keyboard focus", event.window.windowID);
			changeMode(MODE_PAUSE);
			break;

		case SDL_WINDOWEVENT_CLOSE:
			changeMode(MODE_SHUTDOWN);
			break;

#if SDL_VERSION_ATLEAST(2, 0, 5)
		case SDL_WINDOWEVENT_TAKE_FOCUS:
			con_print(CON_INFO, true, "Window %d is offered a focus", event.window.windowID);
			break;

		case SDL_WINDOWEVENT_HIT_TEST:
			con_print(CON_INFO, true, "Window %d has a special hit test", event.window.windowID);
			break;
#endif
		default:
			con_print(CON_INFO, true, "Window %d got unknown event %d",
			        event.window.windowID, event.window.event);
			break;
	}
}

//------------------------------------------------------------------------
//
// Handle JOYSTICK events
void evt_handleJoystickEvents(SDL_Event event)
//------------------------------------------------------------------------
{

}

//------------------------------------------------------------------------
//
// Main event routine - handle all the events coming in and
// farm them to the correct routine
//
// Called from the main thread
void evt_handleEvents()
//------------------------------------------------------------------------
{
	SDL_Event event;

	while ( SDL_PollEvent (&event))
	{
		switch ( event.type )
		{
			case SDL_MOUSEMOTION:
				evt_sendEvent(USER_EVENT_MOUSE, USER_EVENT_MOUSE_MOTION, event.motion.x, event.motion.y, event.motion.state, glm::vec3(), glm::vec3(), "");
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
					gam_createBullet (cam3_Front, cam3_Position, bullet_1_speed);

				evt_sendEvent(USER_EVENT_MOUSE, USER_EVENT_MOUSE_BUTTON_DOWN, event.motion.state, 0, 0, glm::vec3(), glm::vec3(), "");
				break;

			case SDL_MOUSEBUTTONUP:
				evt_sendEvent(USER_EVENT_MOUSE, USER_EVENT_MOUSE_BUTTON_UP, event.motion.state, 0, 0, glm::vec3(), glm::vec3(), "");
				break;

			case SDL_MOUSEWHEEL:

			break;

			case SDL_JOYAXISMOTION:     // Joystick axis motion
			case SDL_JOYBALLMOTION:     // Joystick trackball motion
			case SDL_JOYHATMOTION:      // Joystick hat position change
			case SDL_JOYBUTTONDOWN:     // Joystick button pressed
			case SDL_JOYBUTTONUP:       // Joystick button released
			case SDL_JOYDEVICEADDED:    // Joystick connected
			case SDL_JOYDEVICEREMOVED:  // Joystick disconnected
				evt_handleJoystickEvents (event);
			break;

			case SDL_KEYDOWN:           // key pressed
			case SDL_KEYUP:             // Key released
			case SDL_TEXTEDITING:       // Keyboard text editing (composition)
			case SDL_TEXTINPUT:         // Keyboard text input
			case SDL_KEYMAPCHANGED:     // Keymap changed due to a system event such as an input language or keyboard layout change (>= SDL 2.
				io_handleKeyboardEvent (event);
			break;

			case SDL_FINGERDOWN:        // User has touched input device
			case SDL_FINGERUP:          // User stopped touching input device
			case SDL_FINGERMOTION:      // User is dragging finger on input device
				break;

			case SDL_WINDOWEVENT:
				evt_handleDisplayEvents (event);
				break;

			case SDL_USEREVENT:     // A user-specified event
				break;

			default:
				break;
		}
	}
}