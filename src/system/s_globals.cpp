#include "s_globals.h"

bool        quitProgram = false;

char		returnStr[MAX_STRING_SIZE];
//char        fontFileName[TTF_FILENAME_SIZE];       // Max size for font filename

int         currentMode = MODE_CONSOLE;

int         winWidth = 800;							// Set from startup script
int         winHeight = 600;						// Set from startup script

bool        verbose = false;

bool        cameraLockedToPhysics = false;

bool		g_memLeakLastRun;		// Was there a memory last time we ran

// Global variable to hold GL errors
int         __gl_error_code;

int blendOne, blendTwo;
int blendIndexOne, blendIndexTwo;

int blends [8] = {GL_ONE,
		GL_ONE_MINUS_DST_ALPHA,
		GL_ONE_MINUS_DST_COLOR,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_COLOR,
		GL_DST_ALPHA,
		GL_DST_COLOR};

#define NUM_BLENDS 8