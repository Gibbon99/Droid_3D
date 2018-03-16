#include "s_globals.h"
#include "s_keyboard.h"
#include "s_camera.h"
#include "s_shadowMap.h"
#include "s_collideBSP.h"

float       acceleration;
float       maxSpeed;
float       currentSpeed;
float       friction;

float       currentBackwardSpeed = 0.0f;
float       currentForwardSpeed = 0.0f;
float       currentLeftSpeed = 0.0f;
float       currentRightSpeed = 0.0f;

bool		cameraMoved = false;

glm::vec3   previousCamPosition;		// The position from last movement
glm::vec3   previousViewPosition;		// The view from the last movement

//-----------------------------------------------------------------------------
//
// Process player movement speed
void gam_processPlayerMovement ( float interpolate )
//-----------------------------------------------------------------------------
{
	previousCamPosition = camPosition;
	previousViewPosition = camDirection;

//
// Move LEFT
	if ( true == keyLeftDown )
		{
			if ( currentRightSpeed > 0.0f )
				{
					currentRightSpeed -= acceleration * interpolate;

					if ( currentRightSpeed < 0.0f )
						{
							currentRightSpeed = 0.0f;
							currentLeftSpeed = 0.0f;
						}

				}

			else
				{
					currentLeftSpeed += acceleration * interpolate;

					if ( currentLeftSpeed > maxSpeed )
						currentLeftSpeed = maxSpeed;
				}

			cameraMoved = true;
			cam_strafeCamera ( -currentLeftSpeed );
		}

//
// Move RIGHT
	else if ( true == keyRightDown )
		{
			if ( currentLeftSpeed > 0.0f )
				{
					currentLeftSpeed -= acceleration * interpolate;

					if ( currentLeftSpeed < 0.0f )
						{
							currentLeftSpeed = 0.0f;
							currentRightSpeed = 0.0f;
						}

				}

			else
				{
					currentRightSpeed += acceleration * interpolate;

					if ( currentRightSpeed > maxSpeed )
						currentRightSpeed = maxSpeed;
				}

			cameraMoved = true;
			cam_strafeCamera ( currentRightSpeed );
		}

//
// Move FORWARD
	if ( true == keyForwardDown )
		{
			if ( currentBackwardSpeed > 0.0f )
				{
					currentBackwardSpeed -= acceleration * interpolate;

					if ( currentBackwardSpeed < 0.0f )
						{
							currentBackwardSpeed = 0.0f;
							currentForwardSpeed = 0.0f;
						}

				}

			else
				{
					currentForwardSpeed += acceleration * interpolate;

					if ( currentForwardSpeed > maxSpeed )
						currentForwardSpeed = maxSpeed;
				}

			cameraMoved = true;
			cam_moveCamera ( currentForwardSpeed, interpolate );
		}

//
// Move BACKWARD
	else if ( true == keyBackwardDown )
		{
			if ( currentForwardSpeed > 0.0f )
				{
					currentForwardSpeed -= acceleration * interpolate;

					if ( currentForwardSpeed < 0.0f )
						{
							currentForwardSpeed = 0.0f;
							currentBackwardSpeed = 0.0f;
						}

				}

			else
				{
					currentBackwardSpeed += acceleration * interpolate;

					if ( currentBackwardSpeed > maxSpeed )
						currentBackwardSpeed = maxSpeed;
				}

			cameraMoved = true;
			cam_moveCamera ( -currentBackwardSpeed, interpolate );
		}

//
// Do the slowdown movements - no key pressed
//
// Slowdown FORWARD movement
	if ( false == keyForwardDown )
		{
			if ( currentForwardSpeed > 0.0f )
				{
					currentForwardSpeed -= friction * interpolate;

					if ( currentForwardSpeed < 0.0f )
						currentForwardSpeed = 0.0f;

					cameraMoved = true;
					cam_moveCamera ( currentForwardSpeed, interpolate );
				}
		}

//
// Slowdown BACKWARD movement
	if ( false == keyBackwardDown )
		{
			if ( currentBackwardSpeed > 0.0f )
				{
					currentBackwardSpeed -= friction * interpolate;

					if ( currentBackwardSpeed < 0.0f )
						currentBackwardSpeed = 0.0f;

					cameraMoved = true;
					cam_moveCamera ( -currentBackwardSpeed, interpolate );
				}
		}

//
// Slowdown LEFT movement
	if ( false == keyLeftDown )
		{
			if ( currentLeftSpeed > 0.0f )
				{
					currentLeftSpeed -= friction * interpolate;

					if ( currentLeftSpeed < 0.0f )
						currentLeftSpeed = 0.0f;

					cameraMoved = true;
					cam_strafeCamera ( -currentLeftSpeed );
				}
		}

//
// Slowdown RIGHT movement
	if ( false == keyRightDown )
		{
			if ( currentRightSpeed > 0.0f )
				{
					currentRightSpeed -= friction * interpolate;

					if ( currentRightSpeed < 0.0f )
						currentRightSpeed = 0.0f;

					cameraMoved = true;
					cam_strafeCamera ( currentRightSpeed );
				}
		}

	if ( true == keyUpDown )
//        gl_moveLightPos(glm::vec3(0.0, -0.1, 0.0));
		cam_moveCameraYPos ( 1.0f, interpolate );

	if ( true == keyDownDown )
//        gl_moveLightPos(glm::vec3(0.0, 0.1, 0.0));
		cam_moveCameraYPos ( -1.0f, interpolate );

	//
	// Do collision with BSP mesh
	if ( true == bspCollisionDetectOn )
		{
			testCollisionWithBSP = false;
			cam_resolveCollisions();

		}

	else
		{
			cam_moveTo ( camPosition + wantedCamPosition );
		}

	wantedCamCopy = wantedCamPosition;
	wantedCamPosition = glm::vec3 ( 0.0 );
}
