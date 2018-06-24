#include "s_events.h"
#include "s_globals.h"
#include "s_console.h"
#include "s_audio.h"
#include "s_render.h"
#include "io_textures.h"
#include "s_window.h"
#include "s_entitiesBSP.h"

bool        keyForwardDown = false;
bool        keyBackwardDown = false;
bool        keyLeftDown = false;
bool        keyRightDown = false;
bool		keyUpDown = false;
bool		keyDownDown = false;

bool		keyDoorLeftDown = false;
bool		keyDoorRightDown = false;

//-----------------------------------------------------------------------------
//
// Read a unicode character
void io_readChar ( int character )
//-----------------------------------------------------------------------------
{
	if ( conCurrentCharCount < MAX_STRING_SIZE - 1 )
		{
			if ( character >= 32 )
				{
					conCurrentLine.conLine += (char)character;
					conCurrentCharCount++;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Only read the key to unpause the game
void io_readPauseModeKey(int key, int action)
//-----------------------------------------------------------------------------
{
	if (key == ALLEGRO_KEY_P)
	{
		evt_sendEvent (USER_EVENT_MODE_PAUSE, 0, 0, 0, 0, "");
	}
}

//-----------------------------------------------------------------------------
//
// Read and process keys for main game
void io_readGameSpecialKeys ( int key, int action )
//-----------------------------------------------------------------------------
{
	if ( ALLEGRO_EVENT_KEY_DOWN == action )
		{
			switch ( key )
				{
				case ALLEGRO_KEY_TILDE:
					changeMode ( MODE_CONSOLE );
					conCurrentCharCount = 0;
					break;

				case ALLEGRO_KEY_W:
					keyForwardDown = true;
					break;

				case ALLEGRO_KEY_S:
					keyBackwardDown = true;
					break;

				case ALLEGRO_KEY_A:
					keyLeftDown = true;
					break;

				case ALLEGRO_KEY_D:
					keyRightDown = true;
					break;

				case ALLEGRO_KEY_UP:
					keyUpDown = true;
					break;

				case ALLEGRO_KEY_DOWN:
					keyDownDown = true;
					break;
					
				case ALLEGRO_KEY_F11:
					g_lockMouse = !g_lockMouse;
					break;

				case ALLEGRO_KEY_R:
					bsp_placeCameraAtEntity ( "info_player_start" );
					break;

				case ALLEGRO_KEY_F12:
					io_saveScreenToFile();
					break;

				case ALLEGRO_KEY_P:
					con_print(CON_INFO, true, "Pressed the P key - action is DOWN");
					evt_sendEvent(USER_EVENT_MODE_PAUSE, 0, 0, 0, 0, "");
					break;

					case ALLEGRO_KEY_Q:
					blendIndexOne++;
					if (blendIndexOne > 8)
					{
						blendIndexOne = 0;
						blendIndexTwo++;
						if (blendIndexTwo > 8)
						{
						con_print(CON_INFO, true, "Tried all blending modes");
						blendIndexTwo = 0;
						}
					}

					blendOne = blends[blendIndexOne];
					blendTwo = blends[blendIndexTwo];

						con_print(CON_INFO, true, "Blend one now [ %i ]", blendOne);
					break;

					case ALLEGRO_KEY_E:
						blendIndexTwo++;
						if (blendIndexTwo > 8)
							blendIndexTwo = 0;

						blendTwo = blends[blendIndexTwo];

						con_print(CON_INFO, true, "Blend two now [ %i ]", blendTwo);
						break;

					case ALLEGRO_KEY_L:
						evt_sendEvent (USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_GREEN_ALERT, AUDIO_PAN_CENTER, ALLEGRO_PLAYMODE_LOOP, "");
						break;

					case ALLEGRO_KEY_O:
						evt_sendEvent (USER_EVENT_AUDIO, AUDIO_STOP_SAMPLE, SND_GREEN_ALERT, 0, 0, "" );
						break;

				default:
					break;
				}
		}

	if ( ALLEGRO_EVENT_KEY_UP == action )
		{
			switch ( key )
				{
				case ALLEGRO_KEY_W:
					keyForwardDown = false;
					break;

				case ALLEGRO_KEY_S:
					keyBackwardDown = false;
					break;

				case ALLEGRO_KEY_A:
					keyLeftDown = false;
					break;

				case ALLEGRO_KEY_D:
					keyRightDown = false;
					break;

				case ALLEGRO_KEY_UP:
					keyUpDown = false;
					break;

				case ALLEGRO_KEY_DOWN:
					keyDownDown = false;
					break;
					
				case ALLEGRO_KEY_LEFT:
					keyDoorLeftDown = false;
					break;
					
				case ALLEGRO_KEY_RIGHT:
					keyDoorRightDown = false;
					break;

				default:
					break;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Read and process the console keyboard
void io_readConsoleSpecialKeys ( int key, int action )
//-----------------------------------------------------------------------------
{
	//
	// Check all the other keys, including the backspace key being pressed down
	if ( ALLEGRO_EVENT_KEY_DOWN == action)
		{
			switch ( key )
				{
				case ALLEGRO_KEY_TILDE:
					changeMode ( MODE_GAME );
					break;

				case ALLEGRO_KEY_BACKSPACE:
					con_processBackspaceKey ( 0.0 );
//					conBackSpaceDown = 1;
					break;

				case ALLEGRO_KEY_ENTER:
					con_processCommand ( conCurrentLine.conLine );
					break;

				case ALLEGRO_KEY_TAB:
					con_completeCommand ( conCurrentLine.conLine );
					break;

				case ALLEGRO_KEY_UP:
					con_popHistoryCommand();

					//
					// Get the next command if we need it
					if ( conHistoryPtr != NUM_MAX_CON_COMMANDS )
						conHistoryPtr++;

					break;

				case ALLEGRO_KEY_DOWN:

					//
					// Get the next command if we need it
					if ( conHistoryPtr > 0 )
						conHistoryPtr--;

					con_popHistoryCommand();
					break;

				default:
					break;
				}
		}

	if ( ALLEGRO_EVENT_KEY_UP == action)
		{
			switch ( key )
				{
				case ALLEGRO_KEY_BACKSPACE:
					conBackSpaceDown = 0;
					break;

				default:
					break;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Handle a keyboard event
void io_handleKeyboardEvent ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	if ( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && event.type == ALLEGRO_EVENT_KEY_DOWN )
		quitProgram = true;

	switch (currentMode)
	{
		case MODE_CONSOLE:
			io_readChar(event.keyboard.unichar);
			io_readConsoleSpecialKeys (event.keyboard.keycode, event.type);
			break;

		case MODE_GAME:
			io_readGameSpecialKeys (event.keyboard.keycode, event.type);
			break;

		case MODE_PAUSE:
			io_readPauseModeKey(event.keyboard.keycode, event.type);
			break;

		default:
			break;
	}
}
