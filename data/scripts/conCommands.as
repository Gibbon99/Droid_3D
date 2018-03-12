
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
const int	MODEL_SPHERE = 3;
const int	MODEL_CONE = 4;
const int   MODEL_BSP = 5;


//-----------------------------------------------------------------------------
//
// Set variables used in the game
void as_setGameVariables()
//-----------------------------------------------------------------------------
{
	winWidth = 800;
	winHeight = 600;
	//
	// Freelook mouse speed
	mouseSpeed = 0.01f;
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
	ass_loadModel(MODEL_CRATE, 			"data/models/crate01.3ds",  		0.8f);
	ass_loadModel(MODEL_TANK, 			"data/models/triax_tracks.md2",  	0.3f);
	ass_loadModel(MODEL_TANK_WHEELS, 	"data/models/triax_wheels.md2",  	0.3f);
	ass_loadModel(MODEL_SPHERE,			"data/models/sphere2.obj",			0.4f);
	ass_loadModel(MODEL_CONE,			"data/models/cone.dae",				0.5f);
}

//-----------------------------------------------------------------------------
//
// Add all the script commands to console commands
void as_addAllScriptCommands()
//-----------------------------------------------------------------------------
{

	sys_addScriptCommand("quit", 			"Quit the program", 		"bool as_conCommandQuit()", 				false);
	sys_addScriptCommand("exit", 			"Quit the program", 		"bool as_conCommandQuit()", 				false);
	sys_addScriptCommand("glVersion", 			"Show openGL version", 		"bool as_conCommandOpenGLVer()", 				false);
	sys_addScriptCommand("loadFont", 			"loadFont size index", 		"bool ttf_initLibrary()", 				false);
}
