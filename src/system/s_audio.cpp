#include <hdr/system/s_events.h>
#include "s_globals.h"
#include "s_audio.h"
#include <allegro5/allegro_physfs.h>

#define                     numAudioSamples  16   // Make from script


vector<CUSTOM_EVENT>        audioEventQueue;
ALLEGRO_THREAD              *audioThread;
ALLEGRO_MUTEX               *audioQueueMutex;

_sounds sound[] = { { nullptr, "collosion1.wav", false },
                    { nullptr, "endTransmission1.wav", false },
                    { nullptr, "greenAlert.wav", false },
                    { nullptr, "lift1.wav", false },
                    { nullptr, "scrollBeeps.wav", false },
                    { nullptr, "transferdeadlock.wav", false },
                    { nullptr, "yellowAlert.wav", false },
                    { nullptr, "console1.wav", false },
                    { nullptr, "endTransmission2.wav", false },
                    { nullptr, "keypressBad.wav", false },
                    { nullptr, "lift2.wav", false },
                    { nullptr, "start1.wav", false },
                    { nullptr, "transferMove.wav", false },
                    { nullptr, "console2.wav", false },
                    { nullptr, "energyHeal.wav", false },
                    { nullptr, "keyPressGood.wav", false },
                    { nullptr, "lift3.wav", false },
                    { nullptr, "start2.wav", false },
                    { nullptr, "transferStage1.wav", false },
                    { nullptr, "damage.wav", false },
                    { nullptr, "explode1.wav", false },
                    { nullptr, "laser.wav", false },
                    { nullptr, "lowEnergy.wav", false },
                    { nullptr, "startAll.wav", false },
                    { nullptr, "transferStage2.wav", false },
                    { nullptr, "disruptor.wav", false },
                    { nullptr, "explode2.wav", false },
                    { nullptr, "levelShutdown.wav", false },
                    { nullptr, "redAlert.wav", false },
                    { nullptr, "transfer1.wav", false },
                    { nullptr, "transferStart.wav", false },
                    { nullptr, "door.wav", false }
};

int                 numSoundDevices = 1;
int                 as_numSamples;
int                 as_numMultiSamples = 16;
bool                pauseSound = false;
bool                audioAvailable = false;
float               volumeLevel = 1.0f;
_multiSounds*       multiSounds;


//-------------------------------------------------------------------------
//
// Load each sound sample
void aud_loadSoundSamples()
//-------------------------------------------------------------------------
{
	if ( !audioAvailable )
		return;
	//
	// Allow PHYSFS reading from this current thread
	//
	al_set_physfs_file_interface();

	for ( int i = 0; i != NUM_SOUNDS; i++ )
	{
		sound[i].sample = al_load_sample (sound[i].fileName);
		if ( nullptr == sound[i].sample )
		{
			con_print (CON_ERROR, true, "Error: Failed to load sample [ %s ]", sound[i].fileName);
			sound[i].loadedOk = false;
		}
		else
		{
			con_print (CON_ERROR, true, "Loaded sample [ %s ]", sound[i].fileName);
			sound[i].loadedOk = true;
			sound[i].instance = al_create_sample_instance (sound[i].sample);
			al_attach_sample_instance_to_mixer (sound[i].instance, al_get_default_mixer ());
			al_set_sample_instance_gain (sound[i].instance, volumeLevel);
		}
	}

	for ( int i = 0; i != as_numMultiSamples; i++ )
	{
		multiSounds[i].instance = al_create_sample_instance (sound[0].sample);
	}
}

//-------------------------------------------------------------
//
// Pause the sound system
void aud_pauseSoundSystem ( int newState )
//-------------------------------------------------------------
{
	if ( !audioAvailable )
		return;

	pauseSound = newState;
}

//-------------------------------------------------------------------------
//
// Tried to play an already playing sample - create a new instance and play it
//
// Check the current number of multiSamples and if one is not playing, use this slot
// to create the new instance and start it playing
void aud_playMultiSample ( int whichSound, float pan, ALLEGRO_PLAYMODE loop )
//-------------------------------------------------------------------------
{
	for ( int indexCount = 0; indexCount != as_numMultiSamples; indexCount++ )
	{
		con_print(CON_INFO, true, "Looking for multi slot [ %i ]", indexCount);

		if ( false == al_get_sample_instance_playing ( multiSounds[indexCount].instance ) )
		{
			// Get new instance
			multiSounds[indexCount].instance = al_create_sample_instance ( sound[whichSound].sample );
			al_set_sample_instance_gain ( multiSounds[indexCount].instance, volumeLevel );

			al_attach_sample_instance_to_mixer ( multiSounds[indexCount].instance, al_get_default_mixer() );

			al_set_sample_instance_pan ( multiSounds[indexCount].instance, pan );
			al_set_sample_instance_playmode ( multiSounds[indexCount].instance, loop );
			al_play_sample_instance ( multiSounds[indexCount].instance );
			return;
		}
	}
	con_print ( CON_INFO, true, "WARN: Need to increase as_numMultiSamples. Script setting is to low. Currently [ %i ]", as_numMultiSamples );
}

//-------------------------------------------------------------------------
//
// Check if a sample is still playing - return bool
bool aud_isSoundPlaying ( int whichSound )
//-------------------------------------------------------------------------
{
	if ( false == audioAvailable )
		return false;

	if ( nullptr == sound[whichSound].instance )
		return false;

	return al_get_sample_instance_playing ( sound[whichSound].instance );
}

//-------------------------------------------------------------------------
//
// Stop playing a sound - pass in sound index to get SAMPLE_ID
void aud_stopSound ( int whichSound )
//-------------------------------------------------------------------------
{
	if ( false == audioAvailable )
		return;

	al_stop_sample_instance ( sound[whichSound].instance );
}

//-------------------------------------------------------------------------
//
// Stop playing all sounds
void aud_stopAllSounds()
//--------------------------------------------------------------------------
{
	if ( false == audioAvailable )
		return;

	for ( int i = 0; i != NUM_SOUNDS; i++ )
		al_stop_sample_instance ( sound[i].instance );

	for ( int indexCount = 0; indexCount != as_numMultiSamples; indexCount++ )
	{
		al_stop_sample_instance ( multiSounds[indexCount].instance );
	}
}


//-------------------------------------------------------------------------
//
// Release the sound systems and samples
void aud_releaseSound()
//--------------------------------------------------------------------------
{
	if ( false == audioAvailable )
		return;

	aud_stopAllSounds();

	for ( int i = 0; i != NUM_SOUNDS; i++ )
		al_destroy_sample_instance ( sound[i].instance );

	al_free ( multiSounds );

	al_uninstall_audio();
}

//-------------------------------------------------------------------------
//
// Play a sample
bool aud_playSound ( int whichSound, float pan, ALLEGRO_PLAYMODE loop )
//-------------------------------------------------------------------------
{
	if ( ( false == audioAvailable ) || ( true == pauseSound ) )
		return false;

	if (false == sound[whichSound].loadedOk)
		return false;

	if ( true == aud_isSoundPlaying ( whichSound ) )
	{
		aud_playMultiSample ( whichSound, pan, loop );

		con_print(CON_INFO, true, "sound is already playing - make it multi");

		return true;
	}

	al_set_sample_instance_pan ( sound[whichSound].instance, pan );
	al_set_sample_instance_playmode ( sound[whichSound].instance, loop );
	//
	// ALLEGRO_SAMPLE_ID in place of nullptr for sample ID
	al_play_sample_instance ( sound[whichSound].instance );

	return true;
}


//-------------------------------------------------------------------------
//
// Set volume (gain) for a sample - needs to be already playing
void aud_setVolume ( int whichSound, float volLevel )
//-------------------------------------------------------------------------
{
	if ( false == audioAvailable )
		return;

	if ( false == aud_isSoundPlaying ( whichSound ) )
		return;

	if ( false == al_set_sample_instance_gain ( sound[whichSound].instance, volLevel ) )
	{
		printf ( "Couldn't set sample gain\n" );
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
			for ( uint i = 0; i < audioEventQueue.size (); i++ )
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
	al_lock_mutex(audioQueueMutex);
		audioEventQueue.push_back(*event);
	al_unlock_mutex(audioQueueMutex);
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

	multiSounds = ( _multiSounds * ) al_malloc ( sizeof ( _multiSounds ) * as_numMultiSamples );
	if ( multiSounds == nullptr )
	{
		con_print ( CON_ERROR, true, "ERROR: Failed to get memory to hold multiSamples." );
		audioAvailable = false;
		return false;   // TOTO Free memory
	}

	con_print(CON_INFO, true, "Audio engine started.");

	uint32_t version = al_get_allegro_acodec_version();
	int major = version >> 24;
	int minor = ( version >> 16 ) & 255;
	int revision = ( version >> 8 ) & 255;
	int release = version & 255;

	con_print (CON_INFO, true, "Sound started: %i.%i.%i Release: %i", major, minor, revision, release );

	aud_startAudioThread();

	audioAvailable = true;

	return true;    // Check return codes
}

//----------------------------------------------------------------
//
// Handle an audio user event
void aud_handleAudioUserEvent(CUSTOM_EVENT *event)
//----------------------------------------------------------------
{
	switch (event->action)
	{
		case AUDIO_INIT_ENGINE:
			aud_setupAudioEngine();
			evt_sendEvent (USER_EVENT_AUDIO, AUDIO_LOAD_ALL, 0, 0, 0, "");
			break;

		case AUDIO_STOP_THREAD:
			aud_stopAudioThread ();
			break;

		case AUDIO_PLAY_SAMPLE:
		case AUDIO_STOP_SAMPLE:
		case AUDIO_STOP_ALL:
		case AUDIO_SET_GAIN:
		case AUDIO_PAUSE_STATE:
		case AUDIO_LOAD_ALL:
			aud_addNewEvent (event);
			break;

		default:
			break;
	}
}

//----------------------------------------------------------------
//
// Process the actual audio event - runs inside the AUDIO THREAD
void aud_processAudioEvent(CUSTOM_EVENT event)
//----------------------------------------------------------------
{
	switch (event.action)
	{
		case AUDIO_PLAY_SAMPLE:
			aud_playSound ( event.data1, event.data2, (ALLEGRO_PLAYMODE)event.data3 );
			con_print(CON_INFO, true, "Play a sound file [ %i ] Vol [ %i ]", event.data1, event.data2);
			break;

		case AUDIO_LOAD_ALL:
			aud_loadSoundSamples ();
			break;

		case AUDIO_STOP_SAMPLE:
			aud_stopSound ( event.data1 );
			break;

		case AUDIO_STOP_ALL:
			aud_stopAllSounds();
			break;

		case AUDIO_SET_GAIN:
			aud_setVolume ( event.data1, event.data2 );
			break;

		case AUDIO_PAUSE_STATE:
			aud_pauseSoundSystem ( event.data1 );
			break;

		default:
			break;
	}
}