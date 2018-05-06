#include "s_globals.h"

bool        quitProgram = false;

char		returnStr[MAX_STRING_SIZE];
char        fontFileName[TTF_FILENAME_SIZE];       // Max size for font filename

int         currentMode = MODE_CONSOLE;

int         winWidth = 800;							// Set from startup script
int         winHeight = 600;						// Set from startup script

double      freelookMouseX = 0.0;
double      freelookMouseY = 0.0;
float       mouseSpeed = 0.001f;

bool        verbose = false;

bool        cameraLockedToPhysics = false;

// Global variable to hold GL errors
int         __gl_error_code;
