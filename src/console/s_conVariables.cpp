#include <hdr/system/s_camera3.h>
#include "s_lightMaps.h"
#include "s_audio.h"
#include "s_globals.h"
#include "s_movement.h"
#include "s_fontUtil.h"
#include "s_ttfFont.h"
#include "s_physics.h"
#include "s_render.h"
#include "s_collideBSP.h"
#include "s_shaderLights.h"
#include "s_assimp.h"
#include "s_bullet.h"
#include "s_physicsDebug.h"
#include "s_doorsBSP.h"
#include "io_mouse.h"
#include "s_shadowMap.h"

_hostScriptFunctions hostVariables[] =
{
	{"bool quitProgram",			        &quitProgram},
	{"int winWidth",                        &winWidth},
	{"int winHeight",                       &winHeight},
	{"int currentMode",                     &currentMode},
	{"float mouseSpeed",                    &mouseSpeed},
	{"float mouseSensitivity",              &mouseSensitivity},
	{"float gravityX",                      &gravityX},
	{"float gravityY",                      &gravityY},
	{"float gravityZ",                      &gravityZ},
	{"bool showGBuffers",                   &showGBuffers},
	{"bool g_lockMouse",                    &g_lockMouse},
	{"bool g_debugLightPos",                &g_debugLightPos},
	{"bool g_debugBoundingBox",             &g_debugBoundingBox},
	{"float g_Gamma",                       &g_Gamma},
	{"float bullet_1_speed",                &bullet_1_speed},
	{"bool g_debugPhysics",					&g_debugPhysics},
	{"bool g_debugDoorTriggers",			&g_debugDoorTriggers},
	{"bool g_debugOpenGL",					&g_debugOpenGL},
	{"bool as_useSound",                    &as_useSound},
	{"int as_numMultiSamples",              &as_numMultiSamples},
	{"int as_numAudioSamples",              &as_numAudioSamples},
	{"int as_audioVolume",                  &as_audioVolume},
	{"bool g_renderTextures",               &g_renderTextures},
	{"bool g_debugVolLights",               &g_debugVolLights},
	{"bool g_renderBSP",                    &g_renderBSP},
	{"float doorSpeed",                     &doorSpeed},
	{"float doorDelay",                     &doorDelay},

	{"float lightConstant",                     &lightConstant},
	{"float lightLinear",                     &lightLinear},
	{"float lightQuadratic",                     &lightQuadratic},
	{"",                                    nullptr},
};

//-----------------------------------------------------------------------------
//
// Helper function to assign a num value to a var type string
int conGetVarType ( char *whichType )
//-----------------------------------------------------------------------------
{
	if ( strlen ( whichType ) == 0 )
		return VAR_TYPE_INT;

	if ( 0 == strcmp ( whichType, "int" ) )
		return VAR_TYPE_INT;

	if ( 0 == strcmp ( whichType, "float" ) )
		return VAR_TYPE_VEC3;

	if ( 0 == strcmp ( whichType, "bool" ) )
		return VAR_TYPE_BOOL;

	return -1;
}

//-----------------------------------------------------------
//
// Search for a variable name within all the global names
// return the index if found
// otherwise return -1
int util_getVariableIndex ( string whichVar )
//-----------------------------------------------------------
{
	const char 	*name;
	int 		typeID;
	string 		variableName;

	for ( uint i = 0; i < ( asUINT ) scriptEngine->GetGlobalPropertyCount(); i++ )
		{
			scriptEngine->GetGlobalPropertyByIndex ( i, &name, 0, &typeID );
			variableName = name;

			if ( variableName == whichVar )
				return i;	// Found a match - return index
		}

	return -1;	// Not found
}

//-----------------------------------------------------------
//
// Set the value of a global script variable
void conSetVariableValue ( string whichVar, string newValue )
//-----------------------------------------------------------
{
	int		variableIndex;

	variableIndex = util_getVariableIndex ( whichVar );

	if ( -1 == variableIndex )
		{
			con_print ( CON_INFO, false, "ERROR: Could not locate variable [ %s ]", whichVar.c_str() );
			return;
		}

	const char 	*name;
	int 		typeID;
	bool 		isConst;
	void 		*varPointer;
	string 		varType;

	//
	// Get the Global properties
	scriptEngine->GetGlobalPropertyByIndex ( variableIndex, &name, 0, &typeID, &isConst, 0, &varPointer );

	varType = scriptEngine->GetTypeDeclaration ( typeID );

	if ( varType == "int" )
		{
			con_print ( CON_INFO, false, "Set varible to value [ %i ]", atoi ( newValue.c_str() ) );
			* ( int * ) varPointer = atoi ( newValue.c_str() );
		}

	else if ( varType == "float" )
		{
			con_print ( CON_INFO, false, "Set varible to value [ %f ]", atof ( newValue.c_str() ) );
			* ( float * ) varPointer = atof ( newValue.c_str() );
		}

	else if ( varType == "bool" )
		{
			if ( newValue == "true" || newValue == "1" )
				* ( bool * ) varPointer = true;
			else
				* ( bool * ) varPointer = false;

			con_print ( CON_INFO, false, "Set variable to value [ %s ]", newValue.c_str() );
		}
}

//-----------------------------------------------------------
//
// Get the value of a global script variable
void conGetVariableValue ( string whichVar )
//-----------------------------------------------------------
{
	int		variableIndex;

	variableIndex = util_getVariableIndex ( whichVar );

	if ( -1 == variableIndex )
		{
			con_print ( CON_INFO, false, "ERROR: Could not locate variable [ %s ]", whichVar.c_str() );
			return;
		}

	const char 	*name;
	int 		typeID;
	bool 		isConst;
	void 		*varPointer;
	string 		varType;

	//
	// Get the Global properties
	scriptEngine->GetGlobalPropertyByIndex ( variableIndex, &name, 0, &typeID, &isConst, 0, &varPointer );

	varType = scriptEngine->GetTypeDeclaration ( typeID );

	if ( varType == "int" )
		{
			con_print ( CON_INFO, false, "Value of %s %s is %i", varType.c_str(), whichVar.c_str(), * ( int * ) varPointer );
			return;

		}

	else if ( varType == "float" )
		{
			con_print ( CON_INFO, false, "Value of %s %s is %5.5f", varType.c_str(), whichVar.c_str(), * ( float * ) varPointer );
			return;

		}

	else if ( varType == "bool" )
		{
			con_print ( CON_INFO, false, "Value of %s %s is %s", varType.c_str(), whichVar.c_str(), * ( bool * ) varPointer ? "true" : "false" );
			return;

		}

	else if ( varType == "string" )
		{
			string printString;

			printString = * ( string * ) varPointer;
			con_print ( CON_INFO, false, "Value of %s %s is %s", varType.c_str(), whichVar.c_str(), printString.c_str() );

		}

	else
		con_print ( CON_INFO, false, "Unknown type [ %s ] for [ %s ]", varType.c_str(), whichVar.c_str() );
}

//-----------------------------------------------------------
//
// List all the variables registered to the script module
// TODO: Pass in module name and select variables from module name
void conListVariables()
//-----------------------------------------------------------
{
	asUINT n;

	// List the application registered variables
	con_print ( CON_INFO, false, "Application variables" );

	for ( n = 0; n < ( asUINT ) scriptEngine->GetGlobalPropertyCount(); n++ )
		{
			const char *name;
			int typeId;
			bool isConst;
			scriptEngine->GetGlobalPropertyByIndex ( n, &name, 0, &typeId, &isConst );
			string decl = isConst ? " const " : " ";
			decl += scriptEngine->GetTypeDeclaration ( typeId );
			decl += " ";
			decl += name;
			con_print ( CON_INFO, false, "[ %s ]", decl.c_str() );
		}

	// List the user variables in the module
	asIScriptModule *mod = scriptEngine->GetModule ( "startup" );

	if ( mod )
		{
			//cout << endl;
			con_print ( CON_INFO, false, "User variables" );

			for ( n = 0; n < ( asUINT ) mod->GetGlobalVarCount(); n++ )
				{
					con_print ( CON_INFO, false, "[ %s ]", mod->GetGlobalVarDeclaration ( n ) );
				}
		}
}

//-----------------------------------------------------------
//
// List all the functions registed with the script engine
// TODO: Make module name a paramter to list functions from it
void conListFunctions()
//-----------------------------------------------------------
{
	asUINT n;

	// List the application registered functions
	con_print ( CON_INFO, false, "Application functions" );

	for ( n = 0; n < ( asUINT ) scriptEngine->GetGlobalFunctionCount(); n++ )
		{
			asIScriptFunction *func = scriptEngine->GetGlobalFunctionByIndex ( n );

			// Skip the functions that start with _ as these are not meant to be called explicitly by the user
			if ( func->GetName() [0] != '_' )
				con_print ( CON_INFO, false, "[ %s ]", func->GetDeclaration() );
		}

	// List the user functions in the module
	asIScriptModule *mod = scriptEngine->GetModule ( "startup" );

	if ( mod )
		{
			con_print ( CON_INFO, false, "User functions" );

			for ( n = 0; n < ( asUINT ) mod->GetFunctionCount(); n++ )
				{
					asIScriptFunction *func = mod->GetFunctionByIndex ( n );
					con_print ( CON_INFO, false, "[ %s ]", func->GetDeclaration() );
				}
		}
}
