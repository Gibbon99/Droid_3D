#include "s_globals.h"
#include "s_console.h"
#include "s_openGLWrap.h"
#include "s_ttfFont.h"
#include "s_fontUtil.h"
#include "s_physicsCollision.h"

#include <cstdarg>
#include <sstream>
#include <iostream>
#include <utility>
#include <hdr/system/s_events.h>
#include "s_events.h"

bool			conCursorIsOn;

_conLine		conLines[NUM_CON_LINES];
_conLine		conPrevCommands[NUM_MAX_CON_COMMANDS];

vector<_conCommand>			conCommands;
int							conCurrentNumCommands;

GLuint			conCurrentCharCount;
GLuint			conBackSpaceDown;

_glColor     	currentConLineColor;
GLuint			conHistoryPtr;			// Which history command are we at

_conLine		conCurrentLine;
_conLine		conCurrentPrompt;

float			conBackSpaceDelay;

int				conNumInHistory = 0;

vector<CUSTOM_EVENT>        consoleEventQueue;
ALLEGRO_THREAD              *consoleThread;
ALLEGRO_MUTEX               *consoleQueueMutex;

//-----------------------------------------------------------------------------
//
// Add all the host based console commands here
void con_addConsoleCommands()
//-----------------------------------------------------------------------------
{
	con_addCommand ( "help",			"List out available commands",	( ExternFunc ) conHelp );
	con_addCommand ( "glInfo",			"Info about openGL",			( ExternFunc ) conOpenGLInfo );
	con_addCommand ( "listVars",		"List script variables",		( ExternFunc ) conListVariables );
	con_addCommand ( "listFunctions",	"List script functions",		( ExternFunc ) conListFunctions );
	con_addCommand ( "getVar",     		"Get the value of a variable",  ( ExternFunc ) conGetVariableValue );
	con_addCommand ( "setVar",     		"Set the value of a variable",  ( ExternFunc ) conSetVariableValue );
	con_addCommand ( "scShowFunc", 		"Show all script added commands", ( ExternFunc ) showScriptAddedCommands );
	con_addCommand ( "showCounters",	"Show openGL wrap counters",   ( ExternFunc ) wrapShowCounters );
	con_addCommand ( "debugColObjects",	"Show collision objects",		( ExternFunc ) phy_debugShowCollisionData );
	
//	conAddCommand("scDo",		"Execute script function",		(ExternFunc)conScriptExecute);
}

//----------------------------------------------------------------
//
// Handle a console user event
void con_handleConsoleUserEvent ( CUSTOM_EVENT *event )
//----------------------------------------------------------------
{
	switch ( event->action )
	{
		case CONSOLE_ADD_LINE:
			con_addNewEvent(event);
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Show the console
void con_showConsole()
//-----------------------------------------------------------------------------
{
#define EMBEDDED_FONT_HEIGHT 17.0f

	float 			currentY;
	glm::vec4		lineColor;
	glm::vec2		linePosition;
	auto loopCount = (int)(winHeight / EMBEDDED_FONT_HEIGHT);
	_conLine		conTempLine;

	al_lock_mutex (consoleQueueMutex);  // Wait until thread is not updating console contents

	currentY = EMBEDDED_FONT_HEIGHT;

	for ( int i = 0; i != loopCount; i++ )
		{
			lineColor.r = conLines[i].conLineColor.red;
			lineColor.g = conLines[i].conLineColor.green;
			lineColor.b = conLines[i].conLineColor.blue;
			lineColor.a = 1.0f; //conLines[i].conLineColor.alpha;

			linePosition.x = 1.0f;
			linePosition.y = currentY;
			currentY = currentY + (EMBEDDED_FONT_HEIGHT);

			fnt_printText(linePosition, lineColor, "%s", conLines[i].conLine.c_str() );
		}
		
	conTempLine.conLine = conCurrentLine.conLine;

	if ( conCursorIsOn )
		conTempLine.conLine += "<";	//TODO: Check timing and animation
	else
		conTempLine.conLine += " ";
		
	linePosition.x = 1.0f;
	linePosition.y = 0.0f;
	
	fnt_printText(linePosition, lineColor, "%s", conTempLine.conLine.c_str() );

	al_unlock_mutex (consoleQueueMutex);
}

//-----------------------------------------------------------------------------
//
// Add a new line to the console - move all the others up
// 0 is the new line added
//
// Called from CONSOLE THREAD
//
void con_incLine ( string newLine )
//-----------------------------------------------------------------------------
{
	int i;

	for ( i = NUM_CON_LINES - 1; i != 0; i-- )
	{
		conLines[i].conLine = conLines[i - 1].conLine;
		conLines[i].conLineColor = conLines[i - 1].conLineColor;
	}

	conLines[0].conLine = std::move (newLine);
	conLines[0].conLineColor = currentConLineColor;
}

//----------------------------------------------------------------
//
// Console thread function
static void *consoleThreadFunc(ALLEGRO_THREAD *thr, void *arg)
//----------------------------------------------------------------
{
	while ( !al_get_thread_should_stop (thr))
	{
		al_lock_mutex (consoleQueueMutex);

		if ( !consoleEventQueue.empty ())
		{
			//
			// Process the events
			//
			for ( uint i = 0; i < consoleEventQueue.size (); i++ )
			{
				if ( al_get_thread_should_stop (thr)) // Check if the thread should stop
				{
					al_unlock_mutex (consoleQueueMutex);
					break;
				}

				con_incLine ( consoleEventQueue[i].text );

//				printf("In queue [ %s ]\n", consoleEventQueue[i].text.c_str()); // This stops program from crashing

				consoleEventQueue.erase (consoleEventQueue.begin () + i);   // Remove the event from the queue
			}
		}
		al_unlock_mutex (consoleQueueMutex);
	}
	return nullptr;
}

//----------------------------------------------------------------
//
// Add a new console event to the queue for processing
void con_addNewEvent( CUSTOM_EVENT *event)
//----------------------------------------------------------------
{
	al_lock_mutex (consoleQueueMutex);
		consoleEventQueue.push_back(*event);
	al_unlock_mutex (consoleQueueMutex);
}

//----------------------------------------------------------------
//
// Setup the console thread - it reads events from the consoleEventQueue
// and removes them for processing
bool con_startConsoleThread()
//----------------------------------------------------------------
{
	consoleQueueMutex = al_create_mutex ();
	consoleThread = al_create_thread (consoleThreadFunc, nullptr);
	al_start_thread(consoleThread);
	return true;        // Check thread error codes
}

//-----------------------------------------------------------------------------
//
// Init the console
void con_initConsole()
//-----------------------------------------------------------------------------
{
	int i;

	for ( i = 0; i < NUM_CON_LINES; i++ )
		{
			conLines[i].conLine = "";
			conLines[i].conLineColor.red = 1.0f;
			conLines[i].conLineColor.green = 1.0f;
			conLines[i].conLineColor.blue = 1.0f;
			conLines[i].conLineColor.alpha = 1.0f;
		}

	currentConLineColor.red = 1.0f;
	currentConLineColor.green = 1.0f;
	currentConLineColor.blue = 1.0f;
	currentConLineColor.alpha = 1.0f;

	for ( i = NUM_MAX_CON_COMMANDS - 1; i != 0; i-- )
		conPrevCommands[i].conLine = "";

	conHistoryPtr = 0;
	conNumInHistory = 0;

	conCurrentPrompt.conLine = "]_";
	conCurrentLine.conLine = "";

	conCurrentCharCount = 0;

	con_addConsoleCommands();

	con_startConsoleThread();
}

//-----------------------------------------------------------------------------
//
// Add a script function to the console commands
void con_addScriptCommand ( string command, string usage, string funcPtr, bool setParam )
//-----------------------------------------------------------------------------
{
	_conCommand			tempConCommand;

	tempConCommand.type =		CON_COMMAND_SCRIPT;
	tempConCommand.command = 	std::move(command);
	tempConCommand.usage =      std::move (usage);
	tempConCommand.scriptFunc = std::move(funcPtr);

	conCommands.push_back ( tempConCommand );
	sys_addScriptConsoleFunction ( conCommands.back().command, conCommands.back().scriptFunc, setParam );
	conCurrentNumCommands++;
}

//-----------------------------------------------------------------------------
//
// Push a command into the console for processing
void con_pushCommand ( char *param1 )
//-----------------------------------------------------------------------------
{
	con_processCommand ( param1 );
}

//-----------------------------------------------------------------------------
//
// Execute a console command from a script
void con_pushScriptCommand ( std::string command )
//-----------------------------------------------------------------------------
{
	con_processCommand ( ( char * ) command.c_str() );
}

//-----------------------------------------------------------------------------
//
// Autocompletion for console commands
void con_completeCommand ( string lookFor )
//-----------------------------------------------------------------------------
{
	string	lookForKeep;

	lookForKeep = lookFor;

	//
	// Check each of the commands
	for ( int i = 0; i != conCurrentNumCommands; i++ )
		{
			if ( conCommands[i].command.find ( lookFor, 0 ) != string::npos )
				{
					con_print ( false, true, "[ %s ]", conCommands[i].command.c_str() );
					conCurrentLine.conLine = "";
					conCurrentLine.conLine = conCommands[i].command;
					conCurrentCharCount = (int)conCommands[i].command.length();
				}
		}
}

//-----------------------------------------------------------------------------
//
// Process the cursor
void con_processCursor ( float frameInterval )
//-----------------------------------------------------------------------------
{
	static float conCursorCount = 0.0f;

	conCursorCount += ( GLfloat ) ( 180.0f * frameInterval );

	if ( conCursorCount > CON_CURSOR_MAX_COUNT )
		{
			conCursorCount = 0.0f;
			conCursorIsOn =! conCursorIsOn;
		}
}

//-----------------------------------------------------------------------------
//
// Display the current prompt
void con_processBackspaceKey ( float frameInterval )
//-----------------------------------------------------------------------------
{
	_conLine	conTempLine;

	//
	// Process the backspace key as it repeats
	if ( 1 == conBackSpaceDown )
		{
			conBackSpaceDelay += 30.0f * frameInterval;

			if ( conBackSpaceDelay > 1.0f )
				{
					if ( conCurrentCharCount > 0 )
						conCurrentCharCount--;

					conBackSpaceDelay = 0.0f;

					conTempLine.conLine = conCurrentLine.conLine.substr ( 0, conCurrentCharCount );
					conCurrentLine.conLine = conTempLine.conLine;

					conBackSpaceDown = 0;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Pop a command from the history buffer
void con_popHistoryCommand()
//-----------------------------------------------------------------------------
{
	conCurrentLine.conLine = conPrevCommands[conHistoryPtr].conLine;
	conCurrentCharCount = (int)conPrevCommands[conHistoryPtr].conLine.length();
}

//-----------------------------------------------------------------------------
//
// Add a valid command to the history buffer
void con_addHistoryCommand ( string command )
//-----------------------------------------------------------------------------
{
	conNumInHistory++;

	if ( conNumInHistory > NUM_MAX_CON_COMMANDS )
		conNumInHistory = NUM_MAX_CON_COMMANDS;

	for ( int i = NUM_MAX_CON_COMMANDS - 1; i != 0; i-- )
		{
			conPrevCommands[i].conLine = conPrevCommands[i - 1].conLine;
		}

	conPrevCommands[0].conLine = std::move (command);
	conHistoryPtr = 0;
}

//-----------------------------------------------------------------------------
//
// Add a command to the console command list
bool con_addCommand ( string command, string usage, ExternFunc functionPtr )
//-----------------------------------------------------------------------------
{
	_conCommand			tempConCommand;

	//
	// Check values going in
	//
	if ( 0 == command.length() )
		return false;

	if ( 0 == usage.length() )
		usage = "";

	//
	// Next slot
	//
	tempConCommand.command = 		command;
	tempConCommand.usage = 			usage;
	tempConCommand.conFunc = 		functionPtr;
	tempConCommand.type = 			CON_COMMAND_HOST;

	conCommands.push_back ( tempConCommand );

	conCurrentNumCommands++;

	return true;
}

//-----------------------------------------------------------------------------
//
// Add a line to the console
// Pass in type to change the color
void con_print ( int type, bool fileLog, const char *printText, ... )
//-----------------------------------------------------------------------------
{
	va_list		args;
	char		conText[MAX_STRING_SIZE * 2];

	//
	// check and make sure we don't overflow our string buffer
	//
	if ( strlen ( printText ) >= MAX_STRING_SIZE - 1 )
		sysErrorNormal ( __FILE__, __LINE__, "String passed to console is too long", ( MAX_STRING_SIZE - 1 ), strlen ( printText ) - ( MAX_STRING_SIZE - 1 ) );

	//
	// get out the passed in parameters
	//
	va_start ( args, printText );
	vsprintf ( conText, printText, args );
	va_end ( args );

	if ( fileLog )
		io_logToFile ( "Console : %s", conText );

	switch ( type )
		{
			case CON_NOCHANGE:
				break;

			case CON_TEXT:
				con_setColor ( 1.0f, 1.0f, 1.0f, 1.0f );
				break;

			case CON_INFO:
				con_setColor ( 1.0f, 1.f, 0.0f, 1.0f );
				break;

			case CON_ERROR:
				con_setColor ( 1.0f, 0.0f, 0.0f, 0.0f );
				break;

			default:
				break;
		}

		evt_sendEvent(USER_EVENT_CONSOLE, CONSOLE_ADD_LINE, 0, 0, 0, conText);
}

//-----------------------------------------------------------------------------
//
// Process an entered command
void con_processCommand ( string comLine )
//-----------------------------------------------------------------------------
{
	int                 i;
	vector<string>     	tokens;   // one command and one param
	string              buffer;

	string              command;
	string              param;
	string				param2;
	bool    			conMatchFound = false;

	if ( comLine.empty ())
		return;

	//
	// Start the string with known empty value
	command = "";
	param = "";
	//
	// Get the command and any parameters
	//
	// Insert the string into a stream
	istringstream iss ( comLine, istringstream::in );

	//
	// Put each word into the vector
	while ( iss >> buffer )
		{
			tokens.push_back ( buffer );
		}

	command = tokens[0];

	//
	// Make sure there is a paramater to use
	if ( tokens.size() > 1 )
		param = tokens[1];

	else
		param = "";

	if ( tokens.size() > 2 )
		param2 = tokens[2];

	else
		param2 = "";

	//
	// Check each of the commands
	for ( i = 0; i != conCurrentNumCommands; i++ )
		{
			if ( conCommands[i].command == command )
				{
					con_addHistoryCommand ( comLine );
					con_incLine ( comLine );

					if ( CON_COMMAND_HOST == conCommands[i].type )
						conCommands[i].conFunc ( param, param2 );

					else
						{
							util_executeScriptFunction ( conCommands[i].command, param );
						}

					conMatchFound = true;
					break;
				}
		}

	if ( !conMatchFound )
		con_print ( false, true, "Command [ %s ] not found.", comLine.c_str() );

	//
	// Clear out the string so it doesn't show
	conCurrentLine.conLine = "";

	conCurrentCharCount = 0;
}

//-----------------------------------------------------------------------------
//
// Set the current color for printing lines to the console
void con_setColor ( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
//-----------------------------------------------------------------------------
{
	currentConLineColor.red = red;
	currentConLineColor.green = green;
	currentConLineColor.blue = blue;
	currentConLineColor.alpha = alpha;
}
