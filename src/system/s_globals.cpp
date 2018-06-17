#include "s_globals.h"

bool        quitProgram = false;

char		returnStr[MAX_STRING_SIZE];
char        fontFileName[TTF_FILENAME_SIZE];       // Max size for font filename

int         currentMode = MODE_CONSOLE;

int         winWidth = 800;							// Set from startup script
int         winHeight = 600;						// Set from startup script

bool        verbose = false;

bool        cameraLockedToPhysics = false;

bool		g_memLeakLastRun;		// Was there a memory last time we ran

// Global variable to hold GL errors
int         __gl_error_code;
