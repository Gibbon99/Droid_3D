
const int	CON_COMMAND	= 1;
const int	CON_USAGE	= 2;
const int	CON_FUNC	= 3;
const int   CON_PARAM   = 4;

const int   TEXT_MULTI_ARRAY   = 0;
const int   TEXT_DRAW_ARRAY    = 1;

//
// Need to be in sync with assimp.h
const int   MODEL_CRATE = 0;
const int   MODEL_TANK = 1;
const int   MODEL_TANK_WHEELS = 2;
const int   MODEL_BSP = 3;

//-----------------------------------------------------------------------------
//
// Set variables used in the game
void as_setGameVariables()
//-----------------------------------------------------------------------------
{
    winWidth = 1024;
    winHeight = 768;
    //
    // Freelook mouse speed
    mouseSpeed = 0.001f;
    //
    // Player acceleration
    acceleration = 0.5f;
    //
    // No faster than this
    maxSpeed = 2.5f;
    //
    // How much to slowdown by
    friction = 0.5f;
    //
    // Change window size and set new global variables
    lib_resizeWindow(winWidth,winHeight);
    //
    // Debug AntBar GUI
    antBarWidth = 550;
    antBarHeight = 200;
    antPosX = winWidth - (antBarWidth + 2);
    antPosY = 2;
    //
    // Set font sizes
    fontSizeSmall = 18.0f;
    fontSizeMedium = 20.0f;
    fontSizeLarge = 24.0f;

    renderText = TEXT_DRAW_ARRAY;

    ttf_setFontName("data/President.ttf");
    ttf_initLibrary(fontSizeSmall, 0);

    gravityX = 0.0f;
    gravityY = 2.0f;
    gravityZ = 0.0f;

    materialShininess = 140.0f;
    globalAmbient = 0.3;
    globalGammaFactor = 0.2;
    //
    // Free roam camera
    cameraLockedToPhysics = false;
    //
    // BSP collision
    bspCollisionDetectOn = true;
    //
    // Show position of lights in map
    g_debugLightPos = false;
    //
    // Show Bounding boxes around models
    g_debugBoundingBox = true;
    //
    // Bullet travels at this speed
    bullet_1_speed = 1.3f;
}

//-----------------------------------------------------------------------------
//
// Script console command - quit program
bool as_conCommandQuit()
//-----------------------------------------------------------------------------
{
	quitProgram = true;

	return true;
}

//-----------------------------------------------------------------------------
//
// Script console command - exit program
bool as_conCommandExit()
//-----------------------------------------------------------------------------
{
	quitProgram = true;

	return true;
}

//-----------------------------------------------------------------------------
//
// Script console command - show openGL version information
bool as_conCommandOpenGLVer()
//-----------------------------------------------------------------------------
{
//	printConStr_AS("Vendor",	glGetString_AS(GL_VENDOR));
//	conPrintStr("Renderer", "test"); //glGetString_AS(GL_RENDERER));
//	conPrintStr("Version",	"test"); //glGetString_AS(GL_VERSION));

return true;
}

//-----------------------------------------------------------------------------
//
// Load all the models - Index must match header file
void as_loadAllModels()
//-----------------------------------------------------------------------------
{
    ass_loadModel(MODEL_CRATE, "data/models/crate01.3ds",  0.3f);
    ass_loadModel(MODEL_TANK, "data/models/triax_tracks.md2",  0.3f);
    ass_loadModel(MODEL_TANK_WHEELS, "data/models/triax_wheels.md2",  0.3f);
}

//-----------------------------------------------------------------------------
//
// Add all the script commands to console commands
void as_addAllScriptCommands()
//-----------------------------------------------------------------------------
{
	addScriptCommand_AS(CON_COMMAND,	"quit");
	addScriptCommand_AS(CON_USAGE,		"Quit the program");
	addScriptCommand_AS(CON_PARAM,      "");
	addScriptCommand_AS(CON_FUNC,		"bool as_conCommandQuit()");

	addScriptCommand_AS(CON_COMMAND,	"exit");
	addScriptCommand_AS(CON_USAGE,		"Exit the program");
	addScriptCommand_AS(CON_PARAM,      "");
	addScriptCommand_AS(CON_FUNC,		"bool as_conCommandExit()");

	addScriptCommand_AS(CON_COMMAND,	"glVersion");
	addScriptCommand_AS(CON_USAGE,		"Show openGL version");
	addScriptCommand_AS(CON_PARAM,      "");
	addScriptCommand_AS(CON_FUNC,		"bool as_conCommandOpenGLVer()");

	addScriptCommand_AS(CON_COMMAND,    "loadFont");
	addScriptCommand_AS(CON_USAGE,      "loadFont size index");
	addScriptCommand_AS(CON_PARAM,      "");
	addScriptCommand_AS(CON_FUNC,       "bool ttf_initLibrary()");
}
