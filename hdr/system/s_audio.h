#pragma once

#include "s_globals.h"
#include "allegro5/allegro_acodec.h"
#include "s_events.h"

#define AUDIO_INIT_ENGINE   0x00
#define AUDIO_STOP_THREAD   0x01
#define AUDIO_PLAY_SAMPLE   0x02
#define AUDIO_STOP_SAMPLE   0x03
#define AUDIO_STOP_ALL      0x04
#define AUDIO_SET_GAIN      0x05
#define AUDIO_PAUSE_STATE   0x06
#define AUDIO_LOAD_ALL      0x07
#define AUDIO_STOP_ENGINE   0x08

extern bool		pauseSound;
extern bool 	audioAvailable;
extern int 		as_numAudioSamples;
extern bool		as_useSound;
extern int		as_numMultiSamples;
extern int      as_audioVolume;

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

// Set the global volume to a new level
void aud_setAudioGain(int newLevel);