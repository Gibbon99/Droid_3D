#include "s_globals.h"
#include "s_bmpFont.h"
#include "s_ttfFont.h"
#include "s_fontUtil.h"
#include "s_timing.h"
#include "s_camera.h"
#include "s_movement.h"
#include "s_maths.h"
#include "io_keyboard.h"
#include "s_assimp.h"

bool    verboseLogging = false;

//------------------------------------------------------------------
//
// Show debug information
void showDebugInfo()
//------------------------------------------------------------------
{
//    gl_setFontColor(1.0f, 1.0f, 1.0f, 0.5);
//	ttf_addText ( FONT_SMALL, 0.0f, 25.0f, "FPS [ %i ] ThinkFPS [ %i ] Frametime [ %3.3f ]", fpsPrint, thinkFpsPrint, frameTimeTakenPrint );

//    gl_setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
//    ttf_printString(FONT_SMALL, 0.0f, 50.0f, "rayDir    [ %3.3f - %3.3f - %3.3f ]", clickRayDirection.x, clickRayDirection.y, clickRayDirection.z);
//    ttf_printString(FONT_SMALL, 0.0f, 75.0f, "rayOrigin [ %3.3f - %3.3f - %3.3f ]", clickRayOrigin.x, clickRayOrigin.y, clickRayOrigin.z);
}

//------------------------------------------------------------------
//
// Draw a bounding box
void debug_showBoundingBox ( _boundingBox debugBox, glm::vec3 position, GLfloat scaleby )
//------------------------------------------------------------------
{
	glm::vec3   startPoint, endPoint;

//    position=vec3{0,0,0};
	// Point 1
	startPoint = debugBox.maxSize;
	// Point 2
	endPoint = debugBox.maxSize;
	endPoint.x = debugBox.minSize.x;
	drawDebugLine ( startPoint, endPoint, position, DRAW_LINE, 1000, true, scaleby );

	// Point 2
	startPoint = endPoint;
	// Point 3
	endPoint = debugBox.minSize;
	endPoint.y = debugBox.maxSize.y;
	drawDebugLine ( startPoint, endPoint, position, DRAW_LINE, 1000, true, scaleby );

	// Point 3
	startPoint = endPoint;
	// Point 4
	endPoint = debugBox.maxSize;
	endPoint.z = debugBox.minSize.z;
	drawDebugLine ( startPoint, endPoint, position, DRAW_LINE, 1000, true, scaleby );

	// Point 4
	startPoint = endPoint;
	// Point 1
	endPoint = debugBox.maxSize;
	drawDebugLine ( startPoint, endPoint, position, DRAW_LINE, 1000, true, scaleby );



	// Point 1
	startPoint = debugBox.minSize;
	// Point 2
	endPoint = debugBox.maxSize;
	endPoint.x = debugBox.minSize.x;
	endPoint.y = debugBox.minSize.y;
	drawDebugLine ( startPoint, endPoint, position, DRAW_LINE, 1000, true, scaleby );

	// Point 2
	startPoint = endPoint;
	// Point 3
	endPoint = debugBox.minSize;
	endPoint.y = debugBox.maxSize.y;
	endPoint.y = debugBox.minSize.y;
	drawDebugLine ( startPoint, endPoint, position, DRAW_LINE, 1000, true, scaleby );

	// Point 3
	startPoint = endPoint;
	// Point 4
	endPoint = debugBox.maxSize;
	endPoint.z = debugBox.minSize.z;
	endPoint.y = debugBox.minSize.y;
	drawDebugLine ( startPoint, endPoint, position, DRAW_LINE, 1000, true, scaleby );

	// Point 4
	startPoint = endPoint;
	// Point 1
	endPoint = debugBox.maxSize;
	endPoint.y = debugBox.minSize.y;
	drawDebugLine ( startPoint, endPoint, position, DRAW_LINE, 1000, true, scaleby );


}
