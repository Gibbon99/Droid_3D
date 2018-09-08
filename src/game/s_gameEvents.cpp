
#include <hdr/game/s_bullet.h>
#include "s_gameEvents.h"
#include "s_events.h"

//----------------------------------------------------------------
//
// Handle a game event - called by thread
int gam_processGameEventQueue ( void *ptr )
//----------------------------------------------------------------
{
	_myEventData tempEventData;

	while ( runThreads )
	{
		SDL_Delay(THREAD_DELAY_MS);

		if ( !gameEventQueue.empty ())   // stuff in the queue to process
		{
			if ( SDL_LockMutex (gameMutex) == 0 )
			{
				tempEventData = gameEventQueue.front();
				gameEventQueue.pop();
				SDL_UnlockMutex (gameMutex);
			}

			switch ( tempEventData.eventAction )
			{

				default:
					break;
			}
		}
	}
	printf("GAME thread stopped.\n");
	return 0;
}
