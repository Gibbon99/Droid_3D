#include "s_globals.h"
#include "s_audio.h"

vector<ALLEGRO_EVENT>       audioEventQueue;
ALLEGRO_THREAD              *audioThread;
ALLEGRO_MUTEX               *audioQueueMutex;

//----------------------------------------------------------------
//
// Process the actual audio event
void aud_processAudioEvent(ALLEGRO_EVENT event)
//----------------------------------------------------------------
{
	switch (event.user.data2)
	{
		case AUDIO_PLAY_SAMPLE:

			con_print(CON_INFO, true, "Play a sound file");
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
			if ( al_get_thread_should_stop (thr)) // Check if the thread should stop
			{
				al_unlock_mutex (audioQueueMutex);
				break;
			}

			//
			// Process the events
			//
			for ( auto i = 0; i < audioEventQueue.size (); i++ )
			{
				aud_processAudioEvent(audioEventQueue[i]);

				audioEventQueue.erase (audioEventQueue.begin () + i);   // Remove the event from the queue
			}
		}
		al_unlock_mutex (audioQueueMutex);
	}
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
}

//----------------------------------------------------------------
//
// Add a new audio event to the queue for processing
void aud_addNewEvent(ALLEGRO_EVENT event)
//----------------------------------------------------------------
{
	audioEventQueue.push_back(event);
}

//----------------------------------------------------------------
//
// Setup the audio engine and audio thread
bool aud_setupAudioEngine()
//----------------------------------------------------------------
{
	aud_startAudioThread();
}

//----------------------------------------------------------------
//
// Handle an audio user event
void aud_handleAudioUserEvent(ALLEGRO_EVENT event)
//----------------------------------------------------------------
{
	con_print(CON_INFO, true, "Got AUDIO_EVENT [ %i ]\n", event.user.data2);

	switch (event.user.data2)
	{
		case AUDIO_INIT_ENGINE:
			con_print(CON_INFO, true, "INIT Audio Engine.");
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