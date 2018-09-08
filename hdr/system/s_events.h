#pragma once
#include "../system/s_globals.h"
#include <string>
#include <queue>

#define THREAD_DELAY_MS                 1       // Stop threads using all the CPU time

#define USER_EVENT_MOUSE                0x00
#define USER_EVENT_AUDIO                0x01
#define USER_EVENT_CONSOLE              0x02
#define USER_EVENT_MODE_PAUSE           0x03
#define USER_EVENT_MOUSE_BUTTON_DOWN    0x04
#define USER_EVENT_MOUSE_BUTTON_UP      0x05
#define USER_EVENT_MOUSE_WHEEL          0x06
#define USER_EVENT_MOUSE_MOTION         0x07
#define USER_EVENT_GAME                 0x08

typedef struct
{
	uint    eventType;
	int     eventAction;
	int     data1;
	int     data2;
	int     data3;
	glm::vec3   vec3_1;
	glm::vec3   vec3_2;
	std::string  eventString;
} _myEventData;

extern std::queue<_myEventData>    consoleEventQueue;
extern std::queue<_myEventData>    audioEventQueue;
extern std::queue<_myEventData>    mouseEventQueue;
extern std::queue<_myEventData>    gameEventQueue;

extern SDL_mutex *consoleMutex;
extern SDL_mutex *audioMutex;
extern SDL_mutex *mouseMutex;
extern SDL_mutex *mouseMotionMutex;
extern SDL_mutex *gameMutex;

extern bool     runThreads;

// Main event routine - handle all the events coming in and
// farm them to the correct routine
//
// Called from the main thread
void evt_handleEvents();

// Shutdown mutex
void evt_shutdownMutex();

// Setup user event source and event data
bool evt_registerUserEventSetup();

// Create a custom event to be sent
void evt_sendEvent(int type, int action, int data1, int data2, int data3, glm::vec3 vec3_1, glm::vec3 vec3_2, std::string textString);