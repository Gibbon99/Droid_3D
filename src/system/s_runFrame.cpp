#include "s_events.h"
#include "s_globals.h"
#include "s_console.h"
#include "s_movement.h"
#include "s_camera.h"

#include "s_physics.h"
#include "s_bullet.h"
#include "s_doorsBSP.h"

#include "s_camera3.h"
#include "s_physicsCollision.h"
#include "s_particles.h"
#include "io_mouse.h"

//-----------------------------------------------------------------------------
//
// Process everything not drawing related
void sys_gameTickRun ( float interpolate )
//-----------------------------------------------------------------------------
{
	glm::vec3   vel;

	evt_handleEvents();

	switch ( currentMode )
		{
		case MODE_SHUTDOWN:
			sys_shutdownToSystem();
			quitProgram = true;
			break;

		case MODE_PAUSE:
			break;

		case MODE_CONSOLE:
			break;

		case MODE_GAME:
			lib_getMouseState (&freelookMouseX, &freelookMouseY);

			cam3_processMouseMovement ( freelookMouseX, freelookMouseY, GL_TRUE );
			cam3_processMovementKeys ( interpolate );
			
			// Return the direction vector and turn this into velocity for physics
			bul_setCameraVelocity(cam3_getVelocity());

			bul_processPhysics ( 60.0f );

			vel = bul_returnCameraPosition();
			bul_setPlayerPosition(cam3_Position, glm::vec3(0,0,0));

			cam3_Position = vel;
			
			cam3_createViewMatrix(cam3_Position);

			gam_processBulletMovement ( );
			bsp_checkPlayerVsTrigger();
			bsp_processAllDoorMovements (interpolate);
			phy_doCollisionDetection();

			par_processParticles(interpolate);
			break;

		default:
			break;
		}
}
