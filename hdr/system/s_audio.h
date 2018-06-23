#pragma once

#include "s_globals.h"
#include "allegro5/allegro_acodec.h"
#include "s_events.h"
#include "../../data/scripts/commonDefines.h"

#define AUDIO_INIT_ENGINE   0x00
#define AUDIO_STOP_THREAD   0x01
#define AUDIO_PLAY_SAMPLE   0x02
#define AUDIO_STOP_SAMPLE   0x03
#define AUDIO_STOP_ALL      0x04
#define AUDIO_SET_GAIN      0x05
#define AUDIO_PAUSE_STATE   0x06
#define AUDIO_LOAD_ALL      0x07

extern int 		numSoundDevices;
extern bool 	audioAvailable;
extern bool		as_useSound;
extern int		as_numSamples;
extern int		as_numMultiSamples;
extern bool		pauseSound;
extern float	volumeLevel;

struct _sounds
{
	ALLEGRO_SAMPLE			*sample;
	const char				*fileName;
	bool					loadedOk;
	ALLEGRO_SAMPLE_INSTANCE	*instance;
};

extern _sounds		sound[];

//
// Structure to hold the second or more instance
// of a sample if it is already playing
//
struct _multiSounds
{
	ALLEGRO_SAMPLE_INSTANCE	*instance;
};

extern _multiSounds	*multiSounds;

// Handle an audio user event
void aud_handleAudioUserEvent(CUSTOM_EVENT *event);

// Process the actual audio event - runs inside the AUDIO THREAD
void aud_processAudioEvent(CUSTOM_EVENT event);

// Play a sample
bool aud_playSound ( int whichSound, float pan, ALLEGRO_PLAYMODE loop );