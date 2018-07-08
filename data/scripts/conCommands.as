#include "data/scripts/commonDefines.h"

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
const int	MODEL_FEMADROID = 3;
const int	MODEL_SPHERE = 4;
const int	MODEL_CONE = 5;
const int   MODEL_BSP = 6;

const int PARTICLE_TYPE_SMOKE = 0;
const int PARTICLE_TYPE_SPARK = 1;
const int PARTICLE_TYPE_HEAL = 2;
const int PARTICLE_TYPE_EXPLODE = 3;
const int PARTICLE_TYPE_BULLET_1 = 4;

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
	// Constrain the mouse to window size
    g_lockMouse = false;
	//
	// Change window size and set new global variables
	lib_resizeWindow ( winWidth,winHeight );

	sdf_addFontInfo(0, "Digital.ttf", 		24);
	sdf_addFontInfo(1, "oldDigital.ttf", 	28);
	sdf_addFontInfo(2, "President.ttf", 	38);

	sdf_initFontSystem();
	
	gravityX = 0.0f;
	gravityY = -8.0f;
	gravityZ = 0.0f;
	//
	// Render the BSP faces or not
    g_renderBSP = true;

    doorSpeed = 10.f;

    g_Gamma = 0.0f;
	//
	// Echo OpenGL debug callback to console and file or not
	// Stores up errors and displays in logfile at program end
	g_debugOpenGL = true;
	//
	// Show the triggers for doors
	g_debugDoorTriggers = true;
	//
	// Debug physics - show physic objects outlines
	g_debugPhysics = false;
	//
	// Show position of lights in map
	g_debugLightPos = false;
	//
	// Show Bounding boxes around models
	g_debugBoundingBox = true;
	//
	// Show the position of the light Volumes
    g_debugVolLights = true;
	//
	// Bullet travels at this speed
	bullet_1_speed = 200.0f;
	//
	// Use the sound system or not
    as_useSound = true;
	//
	// How many simultaneous sounds at once
    as_numMultiSamples = 16;
	//
	// How many sounds slots to reserve
    as_numAudioSamples = 16;
	//
	// Set volume level - 0 to 100
    aud_setAudioGain(10);
}

//-----------------------------------------------------------------------------
//
// Script console command - quit program
bool as_conCommandQuit()
//-----------------------------------------------------------------------------
{
	sys_changeMode(MODE_SHUTDOWN);
	//quitProgram = true;

	return true;
}

//-----------------------------------------------------------------------------
//
// Load all the models - Index must match header file
void as_loadAllModels()
//-----------------------------------------------------------------------------
{
	ass_loadModel ( MODEL_CRATE, 		"data/models/Crate1.3ds");
	ass_loadModel ( MODEL_TANK, 		"data/models/triax_tracks.md2");
	ass_loadModel ( MODEL_TANK_WHEELS, 	"data/models/triax_wheels.md2");
	ass_loadModel ( MODEL_SPHERE,		"data/models/sphere2.obj");
	ass_loadModel ( MODEL_CONE,			"data/models/cone.dae");
	ass_loadModel ( MODEL_FEMADROID,	"data/models/house.3ds");
}

//-----------------------------------------------------------------------------
//
// Load the textures used in the particle system
void as_loadParticleTexture()
//-----------------------------------------------------------------------------
{
    par_addParticleTexture(PARTICLE_TYPE_SMOKE, "par_smoke");
    par_addParticleTexture(PARTICLE_TYPE_SPARK, "par_smoke");
    par_addParticleTexture(PARTICLE_TYPE_HEAL, "par_smoke");
    par_addParticleTexture(PARTICLE_TYPE_EXPLODE, "par_smoke");
	par_addParticleTexture(PARTICLE_TYPE_BULLET_1, "flare");
}

//-----------------------------------------------------------------------------
//
// Add all the script commands to console commands
void as_addAllScriptCommands()
//-----------------------------------------------------------------------------
{
	sys_addScriptCommand ( "quit", 			"Quit the program", 		"bool as_conCommandQuit()", 		false );
	sys_addScriptCommand ( "exit", 			"Quit the program", 		"bool as_conCommandQuit()", 		false );
}
