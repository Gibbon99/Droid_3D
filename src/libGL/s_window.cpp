#include "s_antBar.h"
#include "s_globals.h"
#include "s_keyboard.h"
#include "s_maths.h"

#include "s_camera.h"
#include "s_physics.h"
#include "s_bullet.h"

#include "s_camera3.h"

#define API_OPENGL          "gl"
#define API_OPENGL_ES       "es"

#define PROFILE_NAME_CORE   "core"
#define PROFILE_NAME_COMPAT "compat"

#define STRATEGY_NAME_NONE "none"
#define STRATEGY_NAME_LOSE "lose"

GLFWwindow*         mainWindow;
bool                g_lockMouse = true;

//-----------------------------------------------------------------------------
//
// Set the mouse position
void lib_setMousePos ( double newPosX, double newPosY )
//-----------------------------------------------------------------------------
{
	glfwSetCursorPos ( mainWindow, newPosX, newPosY );
	freelookMouseX = 0.0;
	freelookMouseY = 0.0;
}

//-----------------------------------------------------------------------------
//
void lib_setMouseCursor ( bool showMouse )
//-----------------------------------------------------------------------------
{
	if ( true == showMouse )
		glfwSetInputMode	( mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
	else
		glfwSetInputMode	( mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
}

//-----------------------------------------------------------------------------
//
// Get the mouse position
void lib_getMousePos ( double *posX, double *posY )
//-----------------------------------------------------------------------------
{
	double tmpPosX, tmpPosY;

	glfwGetCursorPos ( mainWindow, &tmpPosX, &tmpPosY );

	*posX = tmpPosX;
	*posY = tmpPosY;
}

//-----------------------------------------------------------------------------
//
// Swap buffers for displaying screen
void lib_swapBuffers()
//-----------------------------------------------------------------------------
{
	glfwSwapBuffers ( mainWindow );
}

//-----------------------------------------------------------------------------
//
// Shutdown the library windowing system
void lib_destroyWindow()
//-----------------------------------------------------------------------------
{
	glfwDestroyWindow ( mainWindow );
}

//-----------------------------------------------------------------------------
//
// Call this when clicking mouse buttons
static void libGetMouseButtonCallback ( GLFWwindow* window, int button, int action, int mods )
//-----------------------------------------------------------------------------
{
	switch ( currentMode )
		{
		case MODE_CONSOLE:
			if ( !TwEventMouseButtonGLFW ( button, action ) )
				{

				}

			break;

		case MODE_GAME:
			if ( false == g_lockMouse )
				if ( !TwEventMouseButtonGLFW ( button, action ) ) {}

			if ( GLFW_PRESS == action )
				if ( GLFW_MOUSE_BUTTON_1 == button )
					{
						gam_createBullet ( cam3_Front, camPosition, bullet_1_speed );
					}

			break;

		default:
			break;
		}
}

//-----------------------------------------------------------------------------
//
// Call this each time the mouse moves
static void libGetMousePosCallback ( GLFWwindow* window, double xpos, double ypos )
//-----------------------------------------------------------------------------
{
	switch ( currentMode )
		{
		case MODE_CONSOLE:
			if ( !TwEventMousePosGLFW ( xpos, ypos ) ) // send event to AntTweakBar
				{
					// event has not been handled by AntTweakBar
					// your code here to handle the event
					// ...
				}

			break;

		case MODE_GAME:
			if ( false == g_lockMouse )
				{
					if ( !TwEventMousePosGLFW ( xpos, ypos ) ) // send event to AntTweakBar
						{
							// event has not been handled by AntTweakBar
							// your code here to handle the event
							// ...
						}
				}

			//
			// Get the difference in movements
			freelookMouseX = ( winWidth / 2 - xpos );
			freelookMouseY = ( winHeight / 2 - ypos );
			//
			// Reset back to the middle of the screen
			if ( true == g_lockMouse )
				glfwSetCursorPos ( window, ( winWidth / 2 ), ( winHeight / 2 ) );

			break;

		default:
			break;
		}

}

//-----------------------------------------------------------------------------
//
// Callback for handling the keyboard
static void libKeyboardCallback ( GLFWwindow* window, int key, int scancode, int action, int mods )
//-----------------------------------------------------------------------------
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		quitProgram = true;

	switch ( currentMode )
		{
		case MODE_CONSOLE:
			io_readConsoleSpecialKeys ( key, action );
			break;

		case MODE_GAME:
			io_readGameSpecialKeys ( key, action );
			break;

		default:
			break;
		}
}

//-----------------------------------------------------------------------------
//
// Callback for errors from window library
static void libErrorCallback ( int errorCode, const char* description )
//-----------------------------------------------------------------------------
{
	io_logToFile ( "Error [ %i ] - [ %s ]\n", errorCode, description );
}

static const char* get_client_api_name ( int api )
{
	if ( api == GLFW_OPENGL_API )
		return "OpenGL";

	else if ( api == GLFW_OPENGL_ES_API )
		return "OpenGL ES";

	return "Unknown API";
}

static const char* get_profile_name_gl ( GLint mask )
{
	if ( mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT )
		return PROFILE_NAME_COMPAT;

	if ( mask & GL_CONTEXT_CORE_PROFILE_BIT )
		return PROFILE_NAME_CORE;

	return "unknown";
}

static const char* get_profile_name_glfw ( int profile )
{
	if ( profile == GLFW_OPENGL_COMPAT_PROFILE )
		return PROFILE_NAME_COMPAT;

	if ( profile == GLFW_OPENGL_CORE_PROFILE )
		return PROFILE_NAME_CORE;

	return "unknown";
}

static const char* get_strategy_name_gl ( GLint strategy )
{
	if ( strategy == GL_LOSE_CONTEXT_ON_RESET_ARB )
		return STRATEGY_NAME_LOSE;

	if ( strategy == GL_NO_RESET_NOTIFICATION_ARB )
		return STRATEGY_NAME_NONE;

	return "unknown";
}

static const char* get_strategy_name_glfw ( int strategy )
{
	if ( strategy == GLFW_LOSE_CONTEXT_ON_RESET )
		return STRATEGY_NAME_LOSE;

	if ( strategy == GLFW_NO_RESET_NOTIFICATION )
		return STRATEGY_NAME_NONE;

	return "unknown";
}

//-----------------------------------------------------------------------------
//
// Get the version of the window library
bool lib_getVersion()
//-----------------------------------------------------------------------------
{
	int api = 0, major = 0, minor = 0, revision = 0;
	GLint flags = 0, mask = 0;

	glfwGetVersion ( &major, &minor, &revision );

	io_logToFile ( "GLFW header version: %u.%u.%u",
	               GLFW_VERSION_MAJOR,
	               GLFW_VERSION_MINOR,
	               GLFW_VERSION_REVISION );

	io_logToFile ( "GLFW library version: %u.%u.%u", major, minor, revision );

	if ( major != GLFW_VERSION_MAJOR )
		{
			io_logToFile ( "*** ERROR: GLFW major version mismatch! ***" );
			return false;
		}

	if ( minor != GLFW_VERSION_MINOR || revision != GLFW_VERSION_REVISION )
		io_logToFile ( "*** WARNING: GLFW version mismatch! ***" );

	io_logToFile ( "GLFW library version string: \"%s\"", glfwGetVersionString() );

	api = glfwGetWindowAttrib ( mainWindow, GLFW_CLIENT_API );
	major = glfwGetWindowAttrib ( mainWindow, GLFW_CONTEXT_VERSION_MAJOR );
	minor = glfwGetWindowAttrib ( mainWindow, GLFW_CONTEXT_VERSION_MINOR );
	revision = glfwGetWindowAttrib ( mainWindow, GLFW_CONTEXT_REVISION );

	io_logToFile ( "%s context version string: \"%s\"",
	               get_client_api_name ( api ),
	               glGetString ( GL_VERSION ) );

	io_logToFile ( "%s context version parsed by GLFW: %u.%u.%u",
	               get_client_api_name ( api ),
	               major, minor, revision );

	// Report client API context properties

	if ( api == GLFW_OPENGL_API )
		{
			if ( major >= 3 )
				{
					glGetIntegerv ( GL_CONTEXT_FLAGS, &flags );
					io_logToFile ( "%s context flags (0x%08x):", get_client_api_name ( api ), flags );

					if ( flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT )
						io_logToFile ( " forward-compatible" );

					if ( flags & GL_CONTEXT_FLAG_DEBUG_BIT )
						io_logToFile ( " debug" );

					if ( flags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB )
						io_logToFile ( " robustness" );

					putchar ( '\n' );

					io_logToFile ( "%s context flags parsed by GLFW:", get_client_api_name ( api ) );

					if ( glfwGetWindowAttrib ( mainWindow, GLFW_OPENGL_FORWARD_COMPAT ) )
						io_logToFile ( " forward-compatible" );

					if ( glfwGetWindowAttrib ( mainWindow, GLFW_OPENGL_DEBUG_CONTEXT ) )
						io_logToFile ( " debug" );

					if ( glfwGetWindowAttrib ( mainWindow, GLFW_CONTEXT_ROBUSTNESS ) != GLFW_NO_ROBUSTNESS )
						io_logToFile ( " robustness" );
				}

			if ( major > 3 || ( major == 3 && minor >= 2 ) )
				{
					int profile = glfwGetWindowAttrib ( mainWindow, GLFW_OPENGL_PROFILE );

					glGetIntegerv ( GL_CONTEXT_PROFILE_MASK, &mask );
					io_logToFile ( "%s profile mask (0x%08x): %s",
					               get_client_api_name ( api ),
					               mask,
					               get_profile_name_gl ( mask ) );

					io_logToFile ( "%s profile mask parsed by GLFW: %s",
					               get_client_api_name ( api ),
					               get_profile_name_glfw ( profile ) );
				}

			if ( glfwExtensionSupported ( "GL_ARB_robustness" ) )
				{
					int robustness;
					GLint strategy;
					glGetIntegerv ( GL_RESET_NOTIFICATION_STRATEGY_ARB, &strategy );

					io_logToFile ( "%s robustness strategy (0x%08x): %s",
					               get_client_api_name ( api ),
					               strategy,
					               get_strategy_name_gl ( strategy ) );

					robustness = glfwGetWindowAttrib ( mainWindow, GLFW_CONTEXT_ROBUSTNESS );

					io_logToFile ( "%s robustness strategy parsed by GLFW: %s",
					               get_client_api_name ( api ),
					               get_strategy_name_glfw ( robustness ) );
				}
		}

	io_logToFile ( "%s context renderer string: \"%s\"",
	               get_client_api_name ( api ),
	               glGetString ( GL_RENDERER ) );
	io_logToFile ( "%s context vendor string: \"%s\"",
	               get_client_api_name ( api ),
	               glGetString ( GL_VENDOR ) );

	if ( major > 1 )
		{
			io_logToFile ( "%s context shading language version: \"%s\"",
			               get_client_api_name ( api ),
			               glGetString ( GL_SHADING_LANGUAGE_VERSION ) );
		}

	return true;
}

//-----------------------------------------------------------------------------
//
// Resize the current displayed window
void lib_resizeWindow ( int newWidth, int newHeight )
//-----------------------------------------------------------------------------
{
	glfwSetWindowSize	( mainWindow, newWidth, newHeight );
	winWidth = newWidth;
	winHeight = newHeight;
}


//-----------------------------------------------------------------------------
//
// Start the GL Helper library and open the window
bool lib_openWindow()
//-----------------------------------------------------------------------------
{
	glfwSetErrorCallback ( libErrorCallback );

	if ( !glfwInit() )
		return false;

	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint ( GLFW_CLIENT_API, GLFW_OPENGL_API );
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	mainWindow = glfwCreateWindow ( winWidth, winHeight, "Para_GL3", NULL, NULL );

	if ( !mainWindow )
		{
			glfwTerminate();
			return false;
		}

	glfwMakeContextCurrent ( mainWindow );
	glfwSetKeyCallback ( mainWindow, libKeyboardCallback );
	glfwSetCharCallback ( mainWindow, io_readCharCallback );
	glfwSetCursorPosCallback ( mainWindow, libGetMousePosCallback );
	glfwSetMouseButtonCallback ( mainWindow,libGetMouseButtonCallback );

	/*
	int width, height;
	glfwGetWindowSize(mainWindow, &width, &height);

	printf("Windows size [ %i x %i ]\n", width, height);


	glfwGetFramebufferSize(mainWindow, &width, &height);

	printf("Framebuffer size [ %i x %i ]\n", width, height);
	*/
	/*
	int left, top, right, bottom;

	glfwGetWindowFrameSize (mainWindow, &left, &top, &right, &bottom );

	printf("Frame size left - %i top - %i right - %i bottom %i", left, top, right, bottom);

	*/
	return true;
}
