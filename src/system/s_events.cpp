#include "s_camera3.h"
#include "s_bullet.h"
#include "io_keyboard.h"
#include "s_globals.h"
#include "s_events.h"
#include <utility>
#include "s_window.h"
#include "io_mouse.h"
#include "s_audio.h"

#define MY_EVENT_TYPE            ALLEGRO_GET_EVENT_TYPE('P','A','R','A')

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
	return true;
}

//------------------------------------------------------------------------
//
// Called when it's time to free the custom event - reference count is 0
static void my_event_dtor(ALLEGRO_USER_EVENT *event)
//------------------------------------------------------------------------
{
	delete((CUSTOM_EVENT *)event->data1);
}

//------------------------------------------------------------------------
//
// Create new custom event and populate with the required data
static CUSTOM_EVENT *new_event(int type, int action, double timeStamp, int data1, int data2, int data3, string textString)
//------------------------------------------------------------------------
{
	auto *event = new CUSTOM_EVENT;

	event->type =       type;
	event->action =     action;
	event->text =       std::move (textString);
	event->timeStamp =  timeStamp;
	event->data1 =      data1;
	event->data2 =      data2;
	event->data3 =      data3;
    return event;
}

//------------------------------------------------------------------------
//
// Create a custom event to be sent
void evt_sendEvent(int type, int action, int data1, int data2, int data3, string textString)
//------------------------------------------------------------------------
{
	userEvent.user.type = MY_EVENT_TYPE;
	userEvent.user.data1 = (intptr_t)new_event(type, action, userEvent.user.timestamp, data1, data2, data3,
	                                            std::move (textString));
	al_emit_user_event(&userEventSource, &userEvent, my_event_dtor);
}

//------------------------------------------------------------------------
//
// Handle USER events
void evt_handleUserEvents(CUSTOM_EVENT *event)
//------------------------------------------------------------------------
{
	static double previousTimeStamp = -1;

	switch (event->type)
	{
		case USER_EVENT_MOUSE_BUTTON_DOWN:

			if (event->data1 == 1)  // First mouse button
				gam_createBullet (cam3_Front, cam3_Position, bullet_1_speed);
			break;

		case USER_EVENT_MODE_PAUSE:

			if ( -1 == previousTimeStamp)
			{
				previousTimeStamp = event->timeStamp;
				return;
			}

			if ((event->timeStamp - previousTimeStamp) < 2.0)
				return;

			previousTimeStamp = event->timeStamp;

			con_print(CON_INFO, true, "Got event to PAUSE. Timestamp [ %3.3f ]", event->timeStamp);

			if (MODE_PAUSE == currentMode)
				changeMode (-1);
			else
				changeMode (MODE_PAUSE);
			break;

		case USER_EVENT_AUDIO:
			aud_handleAudioUserEvent(event);
			break;

		default:
			break;
	}
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
	CUSTOM_EVENT    *my_event;

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
			evt_handleJoystickEvents(event);
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_KEY_UP:
		case ALLEGRO_EVENT_KEY_CHAR:
			io_handleKeyboardEvent(event);
			break;

		case ALLEGRO_EVENT_TOUCH_BEGIN:
		case ALLEGRO_EVENT_TOUCH_END:
		case ALLEGRO_EVENT_TOUCH_MOVE:
		case ALLEGRO_EVENT_TOUCH_CANCEL:
			break;

		case ALLEGRO_EVENT_TIMER:
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
			evt_handleDisplayEvents(event);
			break;

		case MY_EVENT_TYPE:
			my_event = (CUSTOM_EVENT *)event.user.data1;
			evt_handleUserEvents (my_event);
			al_unref_user_event(&event.user);
		break;

		default:
			break;
	}
}