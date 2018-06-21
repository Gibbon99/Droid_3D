#pragma once

#define USER_EVENT_MOUSE_BUTTON_DOWN    0x0
#define USER_EVENT_MOUSE_BUTTON_UP      0x1
#define USER_EVENT_MODE_PAUSE           0x2
#define USER_EVENT_AUDIO                0x3

// Main event routine - handle all the events coming in and
// farm them to the correct routine
void evt_handleEvents();

// Setup user event source and event data
bool evt_registerUserEventSetup();

// Create a custom event to be sent
void evt_sendEvent(int eventType, int eventData1);