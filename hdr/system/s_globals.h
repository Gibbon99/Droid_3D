#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <physfs.h>
#include <allegro5/allegro5.h>

#include "../glload/gl_3_2.h"
#include "../glload/gl_load.h"

#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>

// Include GLM
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/gtx/quaternion.hpp"
#include "../glm/gtx/intersect.hpp"
#include "../glm/gtc/matrix_inverse.hpp"

using namespace glm;


#include "s_defines.h"

using namespace std;

#include "angelscript.h"
#include "scriptstdstring.h"
#include "as_scriptarray.h"

#include "s_shutdown.h"
#include "s_debug.h"
#include "io_logfile.h"
#include "s_console.h"
#include "s_camera.h"

extern char		    returnStr[MAX_STRING_SIZE];

extern bool         quitProgram;

extern int          currentMode;

extern int          winWidth;
extern int          winHeight;

extern double       freelookMouseX;
extern double       freelookMouseY;
extern float        mouseSpeed;

extern bool         verbose;                // Additional logging

extern char         fontFileName[];       // Max size for font filename

extern bool        cameraLockedToPhysics;   // Collision detection with physics or free roam camera
// Change game mode
void changeMode( int newMode );
