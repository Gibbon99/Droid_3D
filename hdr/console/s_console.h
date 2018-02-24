#pragma once

#include "s_globals.h"
#include "s_openGL.h"

#define CON_COMMAND_HOST		1
#define CON_COMMAND_SCRIPT		2

#define CON_COMMAND	1
#define CON_USAGE	2
#define CON_FUNC	3

//
//-----------------------------------------------------------------------------
// Generic callback function pointer
// needs the (...) to allow for any parameter passing without hardcoding
// different types for different functions
//-----------------------------------------------------------------------------
typedef int	(*ExternFunc)(...);

#define NUM_CON_LINES			40
#define NUM_MAX_CON_COMMANDS	30

extern int conFontSize;

/*
typedef struct
{
	char		command[MAX_STRING_SIZE];
	char		usage[MAX_STRING_SIZE];
	ExternFunc	conFunc;
} _conCommand;
*/

//-----------------------------------------------------------------------------
//
// AngelScript engine
//
//-----------------------------------------------------------------------------
extern asIScriptEngine *scriptEngine;

//-----------------------------------------------------------------------------
//
// Struct to hold host function mapping to script function names
//
//-----------------------------------------------------------------------------
typedef struct
{
	string			scriptFunctionName;
	void			*hostFunctionPtr;
} _hostScriptFunctions;

extern _hostScriptFunctions hostVariables[];

typedef struct
{
	int			type;
	char		command[MAX_STRING_SIZE];
	char		usage[MAX_STRING_SIZE];
	char		scriptFunc[MAX_STRING_SIZE];
	ExternFunc	conFunc;
} _conCommand;

extern _conCommand	conCommands[NUM_MAX_CON_COMMANDS];
extern GLint		conCurrentNumCommands;

typedef struct
{
	char		conLine[MAX_STRING_SIZE];
	_glColor	conLineColor;
} _conLine;

extern bool         consoleIsReady;
extern _conLine		conLines[NUM_CON_LINES];
extern _conLine		conPrevCommands[NUM_MAX_CON_COMMANDS];
extern _conLine		conCurrentLine;

extern GLuint		conCurrentCharCount;
extern GLuint		conBackSpaceDown;

extern _glColor     currentConLineColor;

#define CON_CURSOR_MAX_COUNT	100

extern bool			conCursorIsOn;

//extern float		conStartY;
//extern float		conBackSpaceDelay;

extern GLuint		conHistoryPtr;			// Which history command are we at

// Draw the console screen
void gl_drawScreen(GLboolean updateNow);

// Add a line to the console
void con_print (int type, bool fileLog, const char *printText, ...);

// Add a line to the console and update the display
void con_printUpdate (int type, bool fileLog, const char *printText, ...);

// Init the console
void con_initConsole();

// Process a entered command
void con_processCommand(char *comLine);

// Push a command into the console for processing
void con_pushCommand(char *param1);

// Add a command to the console command list
bool con_addCommand(const char *command, const char *usage, ExternFunc functionPtr);

// Display the current prompt
void con_processBackspaceKey(float interploate);

// Process the cursor
void con_processCursor(float frameInterval);

// Add a valid command to the history buffer
void con_addHistoryCommand(char *command);

// Pop a command from the history buffer
void con_popHistoryCommand();

// Autocompletion for console commands
void con_completeCommand(char *lookFor);

// Release the memory for the script file
void con_scriptShutdown();

// Set the current color for printing lines to the console
void con_setColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

// Add a script console command to the executable list of functions
bool sys_addScriptConsoleFunction(char *funcName, char *funcPtr);

// Add a script function to the console commands
bool con_addScriptCommand(int type, std::string command);

// Execute a console command from a script
void con_pushScriptCommand(std::string command);

// Print to console from the scripts - String version
void sys_scriptPrintStr(std::string *msgText, std::string *msgParam);

// Execute a function from the script
bool util_executeScriptFunction(string functionName, string funcParam );

void conListVariables();

void conListFunctions();

void conGetVariableValue(string whichVar);

// Set the value of a global script variable
void conSetVariableValue(string whichVar, string newValue);

//-----------------------------------------------------------------------------
//
// Commands executed from the console
//
//-----------------------------------------------------------------------------

// Show the available commands
int conHelp();

// Display information about the version of OpenGL we are running
int conOpenGLInfo();

// Execute external file
int conExecFile(const char *param1);

// Show the commands added from a script
int showScriptAddedCommands();

// Execute a function from the script
int conScriptExecute(const char *param1);

// List the variables we can change
bool conListVariables(const char *param1, const char *param2);

// Get the value of a single variable
bool conGetVariable(const char *param1, const char *param2);

// Set the value of a single variable
bool conSetVariable(const char *param1, const char *param2);
