#include "s_globals.h"
#include "s_console.h"

//-----------------------------------------------------------------------------
//
// Show the available commands
int conHelp()
//-----------------------------------------------------------------------------
{
	int i;

	for (i = 0; i != conCurrentNumCommands; i++)
		{
			if (0 != strcmp(conCommands[i].command, "help"))
				{
					if (conCommands[i].type == CON_COMMAND_SCRIPT)
						{
							con_setColor (1.0f, 0.0f, 1.0f, 1.0f);
							con_print (CON_TEXT, false, "[ %s ] - [ %s ]", conCommands[i].command, conCommands[i].usage);

						}

					else
						{
							con_setColor (1.0f, 1.0f, 0.0f, 1.0f);
							con_print (CON_TEXT, false, "[ %s ] - [ %s ]", conCommands[i].command, conCommands[i].usage);
						}
				}
		}

	con_setColor (1.0f, 1.0f, 1.0f, 1.0f);
	return 1;
}

//-----------------------------------------------------------------------------
//
// Show the commands added from a script
int showScriptAddedCommands()
//-----------------------------------------------------------------------------
{
	int i;

	for (i = 0; i != conCurrentNumCommands; i++)
		{
			if (conCommands[i].type == CON_COMMAND_SCRIPT)
				con_print (CON_TEXT, false, "[ %s ] - [ %s ]", conCommands[i].command, conCommands[i].usage);
		}

	return 1;
}
