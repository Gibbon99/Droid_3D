#include <hdr/system/s_events.h>
#include "s_globals.h"
#include "s_audio.h"

#define                     numAudioSamples  16   // Make from script
bool                        audioAvailable = false;

vector<CUSTOM_EVENT>        audioEventQueue;
ALLEGRO_THREAD              *audioThread;
ALLEGRO_MUTEX               *audioQueueMutex;

//----------------------------------------------------------------
//
// Process the actual audio event
void aud_processAudioEvent(CUSTOM_EVENT event)
//----------------------------------------------------------------
{
	switch (event.action)
	{
		case AUDIO_PLAY_SAMPLE:
			con_print(CON_INFO, true, "Play a sound file [ %i ] Vol [ %i ]", event.data1, event.data2);
			break;

		case AUDIO_STOP_SAMPLE:
			break;

		case AUDIO_STOP_ALL:
			break;

		default:
			break;
	}
}

//----------------------------------------------------------------
//
// Audio thread function
static void *audioThreadFunc(ALLEGRO_THREAD *thr, void *arg)
//----------------------------------------------------------------
{
	while ( !al_get_thread_should_stop (thr))
	{
		al_lock_mutex (audioQueueMutex);

		if ( !audioEventQueue.empty ())
		{
			//
			// Process the events
			//
			for ( auto i = 0; i < audioEventQueue.size (); i++ )
			{
				if ( al_get_thread_should_stop (thr)) // Check if the thread should stop
				{
					al_unlock_mutex (audioQueueMutex);
					break;
				}

				aud_processAudioEvent(audioEventQueue[i]);

				audioEventQueue.erase (audioEventQueue.begin () + i);   // Remove the event from the queue
			}
		}
		al_unlock_mutex (audioQueueMutex);
	}
	return nullptr;
}

//----------------------------------------------------------------
//
// Stop the audio thread
void aud_stopAudioThread()
//----------------------------------------------------------------
{
	al_destroy_thread (audioThread);
	printf("Audio thread destroyed.\n");        // TODO: Not called because we are not processing events on shutdown
}

//----------------------------------------------------------------
//
// Setup the audio thread - it reads events from the audioEventQueue
// and removes them for processing
bool aud_startAudioThread()
//----------------------------------------------------------------
{
	audioQueueMutex = al_create_mutex ();
	audioThread = al_create_thread (audioThreadFunc, nullptr);
	al_start_thread(audioThread);
	return true;        // Check thread error codes
}

//----------------------------------------------------------------
//
// Add a new audio event to the queue for processing
void aud_addNewEvent( CUSTOM_EVENT *event)
//----------------------------------------------------------------
{
	audioEventQueue.push_back(*event);
}

//----------------------------------------------------------------
//
// Setup the audio engine and audio thread
bool aud_setupAudioEngine()
//----------------------------------------------------------------
{
	if (!al_install_audio())
	{
		audioAvailable = false;
		con_print(CON_ERROR, true, "Error installing the audio system. Sound is NOT available.");
		return false;
	}

	if (!al_reserve_samples(numAudioSamples))
	{
		audioAvailable = false;
		con_print(CON_ERROR, true, "Error getting required number of audio samples. Sound is NOT available.");
		return false;
	}

	if (!al_init_acodec_addon())
	{
		audioAvailable = false;
		con_print(CON_ERROR, true, "Error starting codec loading. Sound is NOT available.");
		return false;
	}

	audioAvailable = true;
	con_print(CON_INFO, true, "Audio engine started.");

	uint32_t version = al_get_allegro_acodec_version();
	int major = version >> 24;
	int minor = ( version >> 16 ) & 255;
	int revision = ( version >> 8 ) & 255;
	int release = version & 255;

	con_print (CON_INFO, true, "Sound started: %i.%i.%i Release: %i", major, minor, revision, release );

	aud_startAudioThread();

	return true;    // Check return codes
}

//----------------------------------------------------------------
//
// Handle an audio user event
void aud_handleAudioUserEvent(CUSTOM_EVENT *event)
//----------------------------------------------------------------
{
	con_print(CON_INFO, true, "Got AUDIO_EVENT [ %i ]\n", event->action);

	switch (event->action)
	{
		case AUDIO_INIT_ENGINE:
			aud_setupAudioEngine();
			aud_addNewEvent (event);
			break;

		case AUDIO_PLAY_SAMPLE:
			aud_addNewEvent (event);
			break;

		default:
			break;
	}
}