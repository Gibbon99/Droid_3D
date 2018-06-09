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

#include "s_camera3.h"
#include "s_physicsCollision.h"
#include "s_particles.h"

//-----------------------------------------------------------------------------
//
// Process everything not drawing related
void sys_gameTickRun ( float interpolate )
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

			bul_processPhysics ( 60.0f );

			vel = bul_returnCameraPosition();
			bul_setPlayerPosition(cam3_Position, glm::vec3(0,0,0));
//			con_print(CON_INFO, true, "vel [ %3.3f %3.3f %3.3f ]", vel.x, vel.y, vel.z);

			cam3_Position = vel;
			
			cam3_CreateViewMatrix(cam3_Position);

			gam_processBulletMovement ( interpolate );
			bsp_checkPlayerVsTrigger();
			bspProcessAllDoorMovements ( interpolate );
			phy_doCollisionDetection();

			par_processParticles(interpolate);
			break;

		default:
			break;
		}
}
