#pragma once

#include <string>

#define USER_EVENT_MOUSE_BUTTON_DOWN    0x00
#define USER_EVENT_MOUSE_BUTTON_UP      0x01
#define USER_EVENT_MODE_PAUSE           0x02
#define USER_EVENT_AUDIO                0x03
#define USER_EVENT_CONSOLE              0x04
#define USER_EVENT_TIMER                0x05
#define EVENT_TIMER_START               0x06
#define EVENT_TIMER_CONSOLE_CURSOR      0x07
#define START_CONSOLE_CURSOR            0x08
#define STOP_CONSOLE_CURSOR             0x09

typedef struct
{
	int         type{};
	int         action{};
	int         data1{};
	int         data2{};
	int         data3{};
	std::string      text;
	double      timeStamp{};
} CUSTOM_EVENT;

// Main event routine - handle all the events coming in and
// farm them to the correct routine
void evt_handleEvents();

// Setup user event source and event data
bool evt_registerUserEventSetup();

// Create a custom event to be sent
void evt_sendEvent(int type, int action, int data1, int data2, int data3, std::string textString);