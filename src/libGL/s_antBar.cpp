#include "s_antBar.h"
#include "s_globals.h"
#include "s_timing.h"
#include "s_maths.h"

#include "s_collideBSP.h"
#include "s_varsBSP.h"
#include "s_shaderLights.h"
#include "s_doorsBSP.h"

TwBar       *antBarGUI;
int         antBarWidth, antBarHeight, antPosX, antPosY;

//--------------------------------------------------------------------------
//
// Add a variable to the antbar from a script
void antAddVariable ( const char* varName, int varType, void *whichVar )
//--------------------------------------------------------------------------
{

}

//--------------------------------------------------------------------------
//
// Start Antbar library
void lib_initAntBar()
//--------------------------------------------------------------------------
{
	char        antParams[32];

	// Initialize the GUI
	TwInit ( TW_OPENGL_CORE, NULL );
	TwWindowSize ( winWidth, winHeight );
	antBarGUI = TwNewBar ( "Frame" );
	TwSetParam ( antBarGUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1" );

	TwAddVarRW ( antBarGUI, "FPS ", TW_TYPE_INT32, &fpsPrint, NULL );
	TwAddVarRW ( antBarGUI, "Time", TW_TYPE_DOUBLE, &frameTimeTakenPrint, NULL );
	TwAddVarRW ( antBarGUI, "Skip Models", TW_TYPE_INT32, &numSkippedModels, NULL );
	TwAddVarRW ( antBarGUI, "Doors skip", TW_TYPE_INT32, &numOfDoorsNotDrawn, NULL );
	TwAddVarRW ( antBarGUI, "Doors drawn", TW_TYPE_INT32, &numOfDoorsDrawn, NULL );

	TwAddVarRW ( antBarGUI, "Faces Drawn",  TW_TYPE_INT16, &g_numFacesDrawn, NULL );
	TwAddVarRW ( antBarGUI, "Faces Not Drawn", TW_TYPE_INT16, &g_numFacesNotDrawn, NULL );

	TwAddVarRW ( antBarGUI,   "Shininess",    TW_TYPE_FLOAT, &materialShininess, "label='Reflectiness'       min=0.0 max=300.0 keyIncr=c keyDecr=C help='Show reflection.' " );

	TwAddVarRW ( antBarGUI,   "Attenuation",  TW_TYPE_FLOAT, &allLights[0].attenuation, "label='Attenuation' step=0.01 min=-10.0 max=10.0  keyIncr=1 keyDecr=2 help='Attenuation.' " );

	TwAddVarRW ( antBarGUI,   "Ambient_Coeff",    TW_TYPE_FLOAT, &allLights[0].ambientCoefficient, "label='AmbientCof' step=0.01 min=-10.0 max=10.0 help='Ambient Coefficent.' " );

//	TwAddVarRW(antBarGUI, "Ray Org", TW_TYPE_DIR3F, &clickRayOrigin, NULL);
//	TwAddVarRW(antBarGUI, "Ray Dir", TW_TYPE_DIR3F, &clickRayDirection, NULL);

	sprintf ( antParams, "Frame size='%i %i'", antBarWidth, antBarHeight );
	TwDefine ( antParams ); // resize bar

	sprintf ( antParams, "Frame position='%i %i'", antPosX, antPosY );
	TwDefine ( antParams ); // position bar
}
