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

#include "s_camera2.h"
#include "s_camera3.h"

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

			cam3_processMouseMovement ( freelookMouseX, freelookMouseY, true );
			cam3_processMovementKeys ( interpolate );
			
			// Return the direction vector and turn this into velocity for physics
			bul_setCameraVelocity(cam3_getVelocity());

			bul_processPhysics ( 30.0f );

			vel = bul_returnCameraPosition();

			cam3_Position = vel;
			camPosition = vel + cam3_Front;
			
			cam3_CreateViewMatrix(cam3_Position);

			/*
								btTransform trans;
								doorModels[1].rigidBody->getMotionState()->getWorldTransform(trans);
								printf("Door [ %i ] Pos [ %4.2f %4.2f %4.2f ] \n", 1, trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ() );
				*/

			gam_processBulletMovement ( interpolate );
			bsp_checkPlayerVsTrigger();
			bspProcessAllDoorMovements ( interpolate );

			break;

		default:
			break;
		}
}
