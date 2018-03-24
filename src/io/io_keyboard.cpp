#include "s_globals.h"
#include "s_console.h"
#include "s_camera.h"
#include "s_render.h"
#include "io_textures.h"
#include "s_shadowMap.h"
#include "s_window.h"
#include "s_entitiesBSP.h"

bool        keyForwardDown = false;
bool        keyBackwardDown = false;
bool        keyLeftDown = false;
bool        keyRightDown = false;
bool		keyUpDown = false;
bool		keyDownDown = false;

//-----------------------------------------------------------------------------
//
// Read a unicode character
void io_readCharCallback ( GLFWwindow* window, unsigned int character )
//-----------------------------------------------------------------------------
{
	switch ( currentMode )
		{
		case MODE_CONSOLE:
			if ( conCurrentCharCount < MAX_STRING_SIZE - 1 )
				{
					if ( character >= 32 )
						{
							conCurrentLine.conLine += character;
							conCurrentCharCount++;
						}
				}

			break;

		default:
			break;
		}
}

//-----------------------------------------------------------------------------
//
// Read and process keys for main game
void io_readGameSpecialKeys ( int key, int action )
//-----------------------------------------------------------------------------
{
	if ( GLFW_PRESS == action )
		{
			switch ( key )
				{
				case GLFW_KEY_GRAVE_ACCENT:
					changeMode ( MODE_CONSOLE );
					conCurrentCharCount = 0;
					break;

				case GLFW_KEY_W:
					keyForwardDown = true;
					break;

				case GLFW_KEY_S:
					keyBackwardDown = true;
					break;

				case GLFW_KEY_A:
					keyLeftDown = true;
					break;

				case GLFW_KEY_D:
					keyRightDown = true;
					break;

				case GLFW_KEY_UP:
					keyUpDown = true;
					break;

				case GLFW_KEY_DOWN:
					keyDownDown = true;
					break;

				case GLFW_KEY_F11:
					g_lockMouse = !g_lockMouse;
					break;

				case GLFW_KEY_R:
					bsp_placeCameraAtEntity ( "info_player_start" );
					break;

				case GLFW_KEY_F12:
					io_saveScreenToFile();
					break;

				case GLFW_KEY_SPACE:
					animateLight = !animateLight;
					break;

				default:
					break;
				}
		}

	if ( GLFW_RELEASE == action )
		{
			switch ( key )
				{
				case GLFW_KEY_W:
					keyForwardDown = false;
					break;

				case GLFW_KEY_S:
					keyBackwardDown = false;
					break;

				case GLFW_KEY_A:
					keyLeftDown = false;
					break;

				case GLFW_KEY_D:
					keyRightDown = false;
					break;

				case GLFW_KEY_UP:
					keyUpDown = false;
					break;

				case GLFW_KEY_DOWN:
					keyDownDown = false;
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
	if ( action == GLFW_PRESS )
		{
			switch ( key )
				{
				case GLFW_KEY_GRAVE_ACCENT:
					changeMode ( MODE_GAME );
					break;

				case GLFW_KEY_BACKSPACE:
					conBackSpaceDown = 1;
					break;

				case GLFW_KEY_ENTER:
					con_processCommand ( conCurrentLine.conLine );
					break;

				case GLFW_KEY_TAB:
					con_completeCommand ( conCurrentLine.conLine );
					break;

				case GLFW_KEY_UP:
					con_popHistoryCommand();

					//
					// Get the next command if we need it
					if ( conHistoryPtr != NUM_MAX_CON_COMMANDS )
						conHistoryPtr++;

					break;

				case GLFW_KEY_DOWN:

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

	if ( action == GLFW_RELEASE )
		{
			switch ( key )
				{
				case GLFW_KEY_BACKSPACE:
					conBackSpaceDown = 0;
					break;

				default:
					break;
				}
		}
}
