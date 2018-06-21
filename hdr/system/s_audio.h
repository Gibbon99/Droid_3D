#pragma once

#include "s_globals.h"

#define AUDIO_INIT_ENGINE   0x00
#define AUDIO_STOP_THREAD   0x01
#define AUDIO_PLAY_SAMPLE   0x02
#define AUDIO_STOP_SAMPLE   0x04
#define AUDIO_STOP_ALL      0x08

// Handle an audio user event
void aud_handleAudioUserEvent(ALLEGRO_EVENT event);

