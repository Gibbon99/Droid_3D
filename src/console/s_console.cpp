#include "s_globals.h"
#include "s_console.h"
#include "s_openGLWrap.h"

#include <stdarg.h>

bool         	consoleIsReady;
bool			conCursorIsOn;

int             conFontSize;

_conLine		conLines[NUM_CON_LINES];
_conLine		conPrevCommands[NUM_MAX_CON_COMMANDS];
_conLine		conCurrentLine;
_conCommand		conCommands[NUM_MAX_CON_COMMANDS];

GLuint			conCurrentCharCount;
GLuint			conBackSpaceDown;

_glColor     	currentConLineColor;
GLuint			conHistoryPtr;			// Which history command are we at

GLint			conCurrentNumCommands;

//-----------------------------------------------------------------------------
//
// Init the console
void con_initConsole()
//-----------------------------------------------------------------------------
{
	int i;

	for (i = 0; i < NUM_CON_LINES; i++)
		{
			strcpy(conLines[i].conLine, "");
			conLines[i].conLineColor.red = 1.0f;
			conLines[i].conLineColor.green = 1.0f;
			conLines[i].conLineColor.blue = 1.0f;
			conLines[i].conLineColor.alpha = 1.0f;
		}

	for (i = NUM_MAX_CON_COMMANDS - 1; i != 0; i--)
		strcpy (conPrevCommands[i].conLine, "");

	conHistoryPtr = 0;

	strcpy (conCurrentLine.conLine, "");

	consoleIsReady = true;

	con_setColor (1.0f, 1.0f, 1.0f, 1.0f);

	con_addCommand("help",		"List out available commands",	(ExternFunc)conHelp);
	con_addCommand("glInfo",	"Info about openGL",			(ExternFunc)conOpenGLInfo);
//	con_addCommand("listVars",	"List accessiable variables",	(ExternFunc)conListVariables);
//	con_addCommand("getVar",     "Get the value of a variable",  (ExternFunc)conGetVariable);
//	con_addCommand("setVar",     "Set the value of a variable",  (ExternFunc)conSetVariable);
//	con_addCommand("scShowFunc", "Show all script added commands",(ExternFunc)showScriptAddedCommands);
	con_addCommand("showCounters","Show openGL wrap counters",   (ExternFunc)wrapShowCounters);
//	conAddCommand("scDo",		"Execute script function",		(ExternFunc)conScriptExecute);
}

//-----------------------------------------------------------------------------
//
// Add a script function to the console commands
bool con_addScriptCommand(int type, std::string command)
//-----------------------------------------------------------------------------
{
	// set type to Script
	conCommands[conCurrentNumCommands].type = CON_COMMAND_SCRIPT;

	switch (type)
		{
			case CON_COMMAND:	// command
				strcpy(conCommands[conCurrentNumCommands].command, command.c_str());
				break;

			case CON_USAGE: // Usage
				strcpy(conCommands[conCurrentNumCommands].usage, command.c_str());
				break;

			case CON_FUNC:	// Name of function within script to run
				strcpy(conCommands[conCurrentNumCommands].scriptFunc, command.c_str());
//			sys_addScriptConsoleFunction(conCommands[conCurrentNumCommands].command, conCommands[conCurrentNumCommands].scriptFunc);
				conCurrentNumCommands++;
				break;

			default:
				return false;
		}

	return true;
}

//-----------------------------------------------------------------------------
//
// Push a command into the console for processing
void con_pushCommand(char *param1)
//-----------------------------------------------------------------------------
{
	con_processCommand(param1);
}

//-----------------------------------------------------------------------------
//
// Execute a console command from a script
void con_pushScriptCommand(std::string command)
//-----------------------------------------------------------------------------
{
	con_processCommand((char *)command.c_str());
}

//-----------------------------------------------------------------------------
//
// Autocompletion for console commands
void con_completeCommand(char *lookFor)
//-----------------------------------------------------------------------------
{
	char	lookForKeep[MAX_STRING_SIZE];

	strcpy(lookForKeep, lookFor);

	//
	// Check each of the commands
	for (int i = 0; i != conCurrentNumCommands; i++)
		{
			if (0 == strncmp (conCommands[i].command, lookForKeep, strlen(lookForKeep)))
				{
					con_printUpdate (0.0f, false, "[ %s ]", conCommands[i].command);
					strcpy (conCurrentLine.conLine, "");
					strcpy (conCurrentLine.conLine, conCommands[i].command);
					conCurrentCharCount = (int)strlen(conCommands[i].command);
				}
		}
}

//-----------------------------------------------------------------------------
//
// Process the cursor
void con_processCursor(float frameInterval)
//-----------------------------------------------------------------------------
{
	static float conCursorCount = 0.0f;

	conCursorCount += (GLfloat)(10.0f * frameInterval);

	if (conCursorCount > CON_CURSOR_MAX_COUNT)
		{
			conCursorCount = 0.0f;
			conCursorIsOn =!conCursorIsOn;
		}
}

//-----------------------------------------------------------------------------
//
// Display the current prompt
void con_processBackspaceKey(float interploate)
//-----------------------------------------------------------------------------
{
	_conLine	conTempLine;
	static 		float	conBackSpaceDelay = 0.0f;

	//
	// Process the backspace key as it repeats
	if (true == conBackSpaceDown)
		{
			conBackSpaceDelay += 150.0f * interploate;

			if (conBackSpaceDelay > 400.0f)
				{
					if (conCurrentCharCount > 0)
						conCurrentCharCount--;

					conBackSpaceDelay = 0.0f;
				}
		}

	//
	// Update the conCurrentLine due to backspace repeat
	strcpy (conTempLine.conLine, "");
	conCurrentLine.conLine[conCurrentCharCount] = (char)NULL;
	strcpy (conTempLine.conLine, conCurrentLine.conLine);
	strcpy (conCurrentLine.conLine, "");
	strcpy (conCurrentLine.conLine, conTempLine.conLine);
}

//-----------------------------------------------------------------------------
//
// Pop a command from the history buffer
void con_popHistoryCommand()
//-----------------------------------------------------------------------------
{
	strcpy (conCurrentLine.conLine, conPrevCommands[conHistoryPtr].conLine);
	conCurrentCharCount = (int)strlen(conPrevCommands[conHistoryPtr].conLine);
}

//-----------------------------------------------------------------------------
//
// Add a valid command to the history buffer
void con_addHistoryCommand(char *command)
//-----------------------------------------------------------------------------
{
	for (int i = NUM_MAX_CON_COMMANDS - 1; i != 0; i--)
		{
			strcpy (conPrevCommands[i].conLine, conPrevCommands[i - 1].conLine);
		}

	strcpy (conPrevCommands[0].conLine, (char *)command);
	conHistoryPtr = 0;
}

//-----------------------------------------------------------------------------
//
// Add a command to the console command list
bool con_addCommand(const char *command, const char *usage, ExternFunc functionPtr)
//-----------------------------------------------------------------------------
{
	//
	// Check for available space first
	if (conCurrentNumCommands == NUM_MAX_CON_COMMANDS)
		{
			con_print (CON_TEXT, true, "Unable to add command. Command buffer is full. Limit is [ %i ]", NUM_MAX_CON_COMMANDS);
			return false;
		}

	//
	// Check values going in
	//
	if (0 == strlen(command))
		return false;

	if (0 == strlen(usage))
		strcpy (usage, (char *)"");	// redundant ??

	//
	// Next slot
	strcpy (conCommands[conCurrentNumCommands].command, command);
	strcpy (conCommands[conCurrentNumCommands].usage, usage);
	conCommands[conCurrentNumCommands].conFunc = functionPtr;
	conCommands[conCurrentNumCommands].type = CON_COMMAND_HOST;
	conCurrentNumCommands++;

	return true;
}

//-----------------------------------------------------------------------------
//
// Add a new line to the console - move all the others up
// 0 is the new line added
void conIncLine(char *newLine)
//-----------------------------------------------------------------------------
{
	int i;

	if (strlen(newLine) > (MAX_STRING_SIZE - 2))
		return;

	for (i = NUM_CON_LINES - 1; i != 0; i--)
		{
//	    printf("Copying console lines [ %i ]\n");
			strcpy (conLines[i].conLine, conLines[i - 1].conLine);
			conLines[i].conLineColor = conLines[i - 1].conLineColor;
		}

	strcpy (conLines[0].conLine, (const char *)newLine);
	conLines[0].conLineColor = currentConLineColor;
}

//-----------------------------------------------------------------------------
//
// Add a line to the console
// Pass in type to change the color
void con_print (int type, bool fileLog, const char *printText, ...)
//-----------------------------------------------------------------------------
{
	va_list		args;
	char		conText[MAX_STRING_SIZE * 2];

//int i = 0;

// TODO (dberry#1#): Fix up string length not working when printing entities string from loadBSP

//while (*printText)
//{
//    printText++;
//    i++;
//    printf(" [ %c ]\n", printText[i]);
//}


//    printf("String is [ %i ] chars long\n", i);
	//
	// check and make sure we don't overflow our string buffer
	//
	if (strlen(printText) >= MAX_STRING_SIZE - 1)
		sysErrorNormal (__FILE__, __LINE__, "String passed to console is too long", (MAX_STRING_SIZE - 1), strlen(printText) - (MAX_STRING_SIZE - 1));

	//
	// get out the passed in parameters
	//
	va_start(args, printText);
	vsprintf(conText, printText, args);
	va_end(args);

	conIncLine(conText);

	if (true == fileLog)
		io_logToFile ("%s", conText);

	switch (type)
		{
			case CON_NOCHANGE:
				break;

			case CON_TEXT:
				con_setColor(1.0f, 1.0f, 1.0f, 1.0f);
				break;

			case CON_INFO:
				con_setColor(1.0f, 1.f, 0.0f, 1.0f);
				break;

			case CON_ERROR:
				con_setColor(1.0f, 0.0f, 0.0f, 0.0f);
				break;

			default:
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Process a entered command
void con_processCommand(char *comLine)
//-----------------------------------------------------------------------------
{
	int i;
	char command[MAX_STRING_SIZE];
	char param[MAX_STRING_SIZE];
	char param1[MAX_STRING_SIZE];
	bool conMatchFound = false;
	//
	// Start the string with known empty value
	strcpy (command, "");
	strcpy (param, "");
	strcpy (param1, "");

	//
	// Get the command and any parameters
	if (EOF == sscanf(comLine, "%s %s %s", command, param, param1))
		return;

	//
	// Set parameter to known value if it's not used
	if (0 == strlen(param))
		strcpy (param, "");

	if (0 == strlen(param1))
		strcpy(param1,"");

	//
	// Check each of the commands
	for (i = 0; i != conCurrentNumCommands; i++)
		{
			if (0 == strcmp(conCommands[i].command, command))
				{
					con_addHistoryCommand(comLine);
					conIncLine(comLine);

					if (CON_COMMAND_HOST == conCommands[i].type)
						conCommands[i].conFunc(param, param1);

					else
						util_executeScriptFunction(conCommands[i].scriptFunc, "");

					conMatchFound = true;
					// TODO: Break out early when found ?
				}
		}

	if (false == conMatchFound)
		con_print (CON_TEXT, false, "Command [ %s ] not found.", comLine);

	strcpy (conCurrentLine.conLine, "");
	conCurrentCharCount = 0;
}

//-----------------------------------------------------------------------------
//
// Add a line to the console and update the display
void con_printUpdate (int type, bool fileLog, const char *printText, ...)
//-----------------------------------------------------------------------------
{
	va_list		args;
	char		conText[MAX_STRING_SIZE];

	if (false == consoleIsReady)
		return;

	//
	// check and make sure we don't overflow our string buffer
	//
	if (strlen(printText) >= MAX_STRING_SIZE - 5)
		sysErrorNormal (__FILE__, __LINE__, "String passed to logfile too long", (MAX_STRING_SIZE - 1), strlen(printText) - (MAX_STRING_SIZE - 1));

	//
	// get out the passed in parameters
	//
	va_start(args, printText);
	vsprintf(conText, printText, args);
	va_end(args);

	conIncLine(conText);

	if (true == fileLog)
		io_logToFile ("%s", conText);

	switch (type)
		{
			case CON_NOCHANGE:
				break;

			case CON_TEXT:
				con_setColor(1.0f, 1.0f, 1.0f, 1.0f);
				break;

			case CON_INFO:
				con_setColor(1.0f, 1.f, 0.0f, 1.0f);
				break;

			case CON_ERROR:
				con_setColor(1.0f, 0.0f, 0.0f, 0.0f);
				break;

			default:
				break;
		}

	if (currentMode == MODE_CONSOLE)
		gl_drawScreen(true);
}

//-----------------------------------------------------------------------------
//
// Set the current color for printing lines to the console
void con_setColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
//-----------------------------------------------------------------------------
{
	currentConLineColor.red = red;
	currentConLineColor.green = green;
	currentConLineColor.blue = blue;
	currentConLineColor.alpha = alpha;
}
