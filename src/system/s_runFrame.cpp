#include "s_globals.h"
#include "s_console.h"
#include "s_maths.h"
#include "s_movement.h"
#include "s_camera.h"
#include "s_shaderLights.h"

#include "s_physics.h"
#include "s_shadowMap.h"
#include "s_bullet.h"
#include "s_doorsBSP.h"


//-----------------------------------------------------------------------------
//
// Process everything not drawing related
void gameTickRun ( float interpolate )
//-----------------------------------------------------------------------------
{
	glm::vec3   vel;

	glfwPollEvents();

	switch ( currentMode )
		{
		case MODE_CONSOLE:
			con_processBackspaceKey ( interpolate );
			con_processCursor ( interpolate );
			break;

		case MODE_GAME:

//                cam_update( interpolate );
//                cam_moveTo( wantedCamPosition );      // Handle BSP collision event here - modify wantedCamPosition if required

//cameraLockedToPhysics = true;

			if ( true == cameraLockedToPhysics )
				{
					//vel.y += gravityY;
					bul_setCameraVelocity ( ( wantedCamPosition ) );
				}

//				bul_setGravity();
//				bul_processPhysics(30.0f);

			if ( true == cameraLockedToPhysics )
				{
					vel = bul_returnCameraPosition();   // Get position after running physics
					vel.y = 0;
					wantedCamPosition = vel;
					cam_update ( interpolate );
				}

			else
				{
					cam_update ( interpolate );
				}

			cam_look ( camPosition, camDirection );

			cam_setViewByMouse ( freelookMouseX, freelookMouseY );

//				bsp_processLightEffect(interpolate);
//				shadowMoveLight(interpolate);
			gam_processBulletMovement ( interpolate );
			bsp_checkPlayerVsTrigger();
			bspProcessAllDoorMovements ( interpolate );

			break;

		default:
			break;
		}
}
