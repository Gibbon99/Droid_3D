#include "s_camera3.h"
#include "s_bullet.h"
#include "io_keyboard.h"
#include "s_globals.h"
#include "s_events.h"
#include "s_window.h"
#include "io_mouse.h"

#define MY_EVENT_TYPE   ALLEGRO_GET_EVENT_TYPE('P','A','R','A')

ALLEGRO_EVENT_QUEUE*    eventQueue;
ALLEGRO_EVENT_SOURCE    userEventSource;
ALLEGRO_EVENT           userEvent;

//------------------------------------------------------------------------
//
// Setup user event source and event data
bool evt_registerUserEventSetup()
//------------------------------------------------------------------------
{
	eventQueue = al_create_event_queue ();
	if (!eventQueue )
	{
		al_show_native_message_box (al_mainWindow, "Error", "Error", "Failed to start event queue", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	if (al_is_keyboard_installed ())
		al_register_event_source (eventQueue, al_get_keyboard_event_source ());
	else
	{
		io_logToFile ("Error: Keyboard driver not installed.");
		al_show_native_message_box (al_mainWindow, "Error", "Error", "Error: Keyboard driver not installed.", nullptr,
		                            ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
/*
 * DON'T USE MOUSE - TOO MANY MOUSE MOVEMENT EVENTS BOGS DOWN THE EVENT QUEUE
 *
	if (al_is_mouse_installed ())
		al_register_event_source (eventQueue, al_get_mouse_event_source ());
	else
	{
		io_logToFile ("Error: Mouse driver not installed.");
		al_show_native_message_box (al_mainWindow, "Error", "Error", "Error: Mouse driver not installed.", nullptr,
		                            ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
*/
	al_register_event_source (eventQueue, al_get_display_event_source (al_mainWindow));
	//
	// Setup for user generated events
	//
	al_init_user_event_source(&userEventSource);
	al_register_event_source (eventQueue, &userEventSource);
	userEvent.type = MY_EVENT_TYPE;
	return true;
}

//------------------------------------------------------------------------
//
// Create a custom event to be sent
void evt_sendEvent(int eventType, int eventData1)
//------------------------------------------------------------------------
{
	userEvent.user.data1 = eventType;
	userEvent.user.data2 = eventData1;
	al_emit_user_event (&userEventSource, &userEvent, NULL);
}

//------------------------------------------------------------------------
//
// Handle USER events
void evt_handleUserEvents(ALLEGRO_EVENT event)
//------------------------------------------------------------------------
{
	switch (event.user.data1)
	{
		case USER_EVENT_MOUSE_BUTTON_DOWN:
			if (event.user.data2 == 1)  // First mouse button
				gam_createBullet (cam3_Front, cam3_Position, bullet_1_speed);
			break;

		case USER_EVENT_MODE_PAUSE:

			con_print(CON_INFO, true, "Got event to PAUSE");

			if (MODE_PAUSE == currentMode)
				changeMode (-1);
			else
				changeMode (MODE_PAUSE);
			break;

		default:
			break;
	}

	con_print(CON_INFO, true, "User event data [ %i ]", event.user.data1);
}

//------------------------------------------------------------------------
//
// Handle DISPLAY events
void evt_handleDisplayEvents(ALLEGRO_EVENT event)
//------------------------------------------------------------------------
{

}

//------------------------------------------------------------------------
//
// Handle JOYSTICK events
void evt_handleJoystickEvents(ALLEGRO_EVENT event)
//------------------------------------------------------------------------
{

}

//------------------------------------------------------------------------
//
// Main event routine - handle all the events coming in and
// farm them to the correct routine
void evt_handleEvents()
//------------------------------------------------------------------------
{
	ALLEGRO_EVENT   event;

	if ( !al_get_next_event(eventQueue, &event))
	{
		printf("No events in queue\n");
		return;     // no events in the queue
	}
	else
	{
		printf("Got an event in the queue....\n");
	}

	switch (event.type)
	{
		case ALLEGRO_EVENT_JOYSTICK_AXIS:
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
		case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:

			printf("Got a Joystick event\n");

			evt_handleJoystickEvents(event);
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_KEY_UP:
		case ALLEGRO_EVENT_KEY_CHAR:

			printf("Got a keyboard event\n");

			io_handleKeyboardEvent(event);
			break;

		case ALLEGRO_EVENT_TOUCH_BEGIN:
		case ALLEGRO_EVENT_TOUCH_END:
		case ALLEGRO_EVENT_TOUCH_MOVE:
		case ALLEGRO_EVENT_TOUCH_CANCEL:

			printf("Got a touch event\n");
			break;

		case ALLEGRO_EVENT_TIMER:

			printf("Got a timer event\n");
			break;

		case ALLEGRO_EVENT_DISPLAY_EXPOSE:
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
		case ALLEGRO_EVENT_DISPLAY_LOST:
		case ALLEGRO_EVENT_DISPLAY_FOUND:
		case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
		case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
		case ALLEGRO_EVENT_DISPLAY_ORIENTATION:
		case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
		case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING:
		case ALLEGRO_EVENT_DISPLAY_CONNECTED:
		case ALLEGRO_EVENT_DISPLAY_DISCONNECTED:

			printf("Got a display event\n");

			evt_handleDisplayEvents(event);
			break;

		case MY_EVENT_TYPE:

			con_print (CON_INFO, true, "Got a user generated event\n");

			evt_handleUserEvents (event);
		break;

		default:
			break;

	}
}