#include "s_globals.h"
#include "s_movement.h"
#include "s_window.h"
#include "s_collideBSP.h"
#include "s_physics.h"
#include "s_antBar.h"

// The camera is surrounded by a sphere of this radius
#define PLAYER_BOUNDING_SPHERE_RADIUS 15

// This vector hold in which direction and how much the camera can clamp vertically
glm::vec3 STEP_UP ( 0,PLAYER_BOUNDING_SPHERE_RADIUS * 1.5,0 );

#define		BOX_SIZE			PLAYER_BOUNDING_SPHERE_RADIUS

glm::vec3 	camPosition =       glm::vec3 ( 0.0 );
glm::vec3	camDirection =      glm::vec3 ( 0.0 );
glm::vec3	wantedCamPosition = glm::vec3 ( 0.0 );
glm::vec3	wantedCamCopy =     glm::vec3 ( 0.0 );
glm::vec3   vecUp =             glm::vec3 ( 0.0 );
glm::vec3   camStrafe =         glm::vec3 ( 0.0 );

glm::vec3	boxMin;
glm::vec3	boxMax;

float   	camMoveDistance;

//-----------------------------------------------------------------------------
//
// Return the current camera position
glm::vec3 cam_getPosition()
//-----------------------------------------------------------------------------
{
	return camPosition;
}

//-----------------------------------------------------------------------------
//
//	This rotates the view around the position using an axis-angle rotation
void camRotateView ( float angle, float x, float y, float z )
//-----------------------------------------------------------------------------
{
	glm::vec3 vNewView;

	// Get the view vector (The direction we are facing)
	glm::vec3 vView = camDirection - camPosition;

	// Calculate the sine and cosine of the angle once
	float cosTheta = ( float ) cos ( angle );
	float sinTheta = ( float ) sin ( angle );

	// Find the new x position for the new rotated point
	vNewView.x  = ( cosTheta + ( 1 - cosTheta ) * x * x )		* vView.x;
	vNewView.x += ( ( 1 - cosTheta ) * x * y - z * sinTheta )	* vView.y;
	vNewView.x += ( ( 1 - cosTheta ) * x * z + y * sinTheta )	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ( ( 1 - cosTheta ) * x * y + z * sinTheta )	* vView.x;
	vNewView.y += ( cosTheta + ( 1 - cosTheta ) * y * y )		* vView.y;
	vNewView.y += ( ( 1 - cosTheta ) * y * z - x * sinTheta )	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ( ( 1 - cosTheta ) * x * z - y * sinTheta )	* vView.x;
	vNewView.z += ( ( 1 - cosTheta ) * y * z + x * sinTheta )	* vView.y;
	vNewView.z += ( cosTheta + ( 1 - cosTheta ) * z * z )		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	camDirection = camPosition + vNewView;
}


//-----------------------------------------------------------------------------
//
//	This allows us to look around using the mouse, like in most first person games.
void cam_setViewByMouse ( float x, float y )
//-----------------------------------------------------------------------------
{
	float angleY = 0.0f;							// This is the direction for looking up or down
	float angleZ = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)
	static float currentRotX = 0.0f;

	if ( false == g_lockMouse )
		return;

	//
	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleY = x * mouseSpeed; //(float)( (middleX - mousePos.x) ) / 500.0f;
	angleZ = y * mouseSpeed; //(float)( (middleY - mousePos.y) ) / 500.0f;

	// Here we keep track of the current rotation (for up and down) so that
	// we can restrict the camera from doing a full 360 loop.
	currentRotX -= angleZ;

	// If the current rotation (in radians) is greater than 1.0, we want to cap it.
	if ( currentRotX > 1.0f )
		currentRotX = 1.0f;

	// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
	else if ( currentRotX < -1.0f )
		currentRotX = -1.0f;

	// Otherwise, we can rotate the view around our position
	else
		{
			// To find the axis we need to rotate around for up and down
			// movements, we need to get a perpendicular vector from the
			// camera's view vector and up vector.  This will be the axis.
			glm::vec3 vAxis = glm::cross ( camDirection - camPosition, vecUp );
			vAxis = glm::normalize ( vAxis );

			// Rotate around our perpendicular axis and along the y-axis
			camRotateView ( angleZ, vAxis.x, vAxis.y, vAxis.z );
			camRotateView ( angleY, 0, 1, 0 );
		}

	if ( true == g_lockMouse )
		lib_setMousePos ( winWidth / 2, winHeight / 2 );
}

//-----------------------------------------------------------------------------
//
// Called to point the camera vectors in the right direction
void cam_setupCamera()
//-----------------------------------------------------------------------------
{
	glm::vec3 vZero = glm::vec3 ( 0.0, 0.0, 0.0 );		// Init a vVector to 0 0 0 for our position
	glm::vec3 vView = glm::vec3 ( 0.0, 1.0, 0.5 );		// Init a starting view vVector (looking up and out the screen)
	glm::vec3 vUp   = glm::vec3 ( 0.0, 1.0, 0.0 );		// Init a standard up vVector (Rarely ever changes)

	camPosition		= vZero;					// Init the position to zero
	camDirection	= vView;					// Init the view to a std starting view
	vecUp			= vUp;						// Init the UpVector
}



//-----------------------------------------------------------------------------
//
//	Try to make a step up and if possible, move on or slide
bool cam_tryMove ( glm::vec3 vFrom, glm::vec3 vMoveToVector )
//-----------------------------------------------------------------------------
{
#define BOX_SIZE        15

	sMoveData   md;

	glm::vec3   boxMin, boxMax;
	glm::vec3   slide;

	boxMin.x = -BOX_SIZE;
	boxMin.y = -BOX_SIZE;
	boxMin.z = -BOX_SIZE;

	boxMax.x = BOX_SIZE;
	boxMax.y = BOX_SIZE;
	boxMax.z = BOX_SIZE;

	while ( fabs ( wantedCamPosition.x ) > 0.0001f || fabs ( wantedCamPosition.y ) > 0.0001f || fabs ( wantedCamPosition.z ) > 0.0001f )
		{
			bsp_checkSphereMove ( vFrom,vFrom + vMoveToVector, BOX_SIZE, md );

//		bsp_checkBoxMove ( vFrom, vFrom + vMoveToVector, boxMin, boxMax, md);
			//
			// No collision
			if ( true == md.AllSolid )
				{
					testCollisionWithBSP = false;
					return false;
				}

			//
			// Will collide, so reduce amount of movement
			if ( md.Fraction > 0 )
				{
					wantedCamPosition = md.EndPoint;    // This is where we hit something
					testCollisionWithBSP = false;
					return true;
				}

			//
			// Can't move so slide along
			wantedCamPosition = md.EndPoint;        // this is where the collision occured

			vMoveToVector =         glm::normalize ( vMoveToVector );
//        md.CollisionNormal =    glm::normalize(md.CollisionNormal);

			slide  = vMoveToVector - md.CollisionNormal * ( glm::dot ( md.CollisionNormal, vMoveToVector ) );

			printf ( "Slide [ %3.3f %3.3f %3.3f ] Node index [ %i ]\n", slide.x, slide.y, slide.z, md.nodeIndex );

			wantedCamPosition += slide;

			testCollisionWithBSP = true;

			return true;
		}

	testCollisionWithBSP = false;
	return false;
}



//-----------------------------------------------------------------------------
//
//	This will move the camera to the new location
//
// NewPosition is a vector indicating the desired movement direction
//
// Global wantedCamPosition - camPosition is used to derive it
void cam_moveTo ( glm::vec3 newPosition )
//-----------------------------------------------------------------------------
{
	camDirection = camDirection + ( newPosition - camPosition ); // - camPosition;
	camPosition = newPosition;
}

//-----------------------------------------------------------------------------
//
//	This will move the camera forward or backward depending on the speed
void cam_moveCamera ( float speed, float interpolate )
//-----------------------------------------------------------------------------
{
	glm::vec3 deltha;

	// Get the current view vector (the direction we are looking)
	deltha = camDirection - camPosition;

	deltha = glm::normalize ( deltha ) * ( speed * interpolate );

	wantedCamPosition.x = wantedCamPosition.x + deltha.x;
	wantedCamPosition.z = wantedCamPosition.z + deltha.z;
}

//-----------------------------------------------------------------------------
//
//	This strafes the camera left and right depending on the speed (-/+)
void cam_strafeCamera ( float speed )
//-----------------------------------------------------------------------------
{
	// Try to move in that direction
	wantedCamPosition = wantedCamPosition + ( camStrafe * speed );
}

//-----------------------------------------------------------------------------
//
//	This updates the camera's view and strafe vector
void cam_update ( float interpolate )
//-----------------------------------------------------------------------------
{
	// Initialize a variable for the cross product result
	glm::vec3 vCross = glm::cross ( camDirection - camPosition, vecUp );

	// Normalize the strafe vector
	camStrafe = glm::normalize ( vCross );

//	camDoGravity();

	// This checks to see if the keyboard was pressed
	gam_processPlayerMovement ( interpolate );
}

//-----------------------------------------------------------------------------
//
//	This updates the camera according to the view and position
void cam_look ( glm::vec3 matPosition, glm::vec3 matDirection )
//-----------------------------------------------------------------------------
{
	//
	// Camera matrix
	viewMatrix = glm::lookAt (
	                 matPosition,           // Camera is here
	                 matDirection, 			// and looks here : at the same position, plus "direction"
	                 vecUp                  // Head is up
	             );
}


//-----------------------------------------------------------------------------
//
// Used for moving up/down
void cam_moveCameraYPos ( float speed, float interpolate )
//-----------------------------------------------------------------------------
{
	wantedCamPosition.y -= speed * interpolate;
}

//-----------------------------------------------------------------------------
//
//	This function sets the camera's position and view and up vVector.
void cam_positionCamera (     float positionX, float positionY, float positionZ,
                              float viewX,     float viewY,     float viewZ,
                              float upVectorX, float upVectorY, float upVectorZ )
//-----------------------------------------------------------------------------
{
	glm::vec3 vPosition	= glm::vec3 ( positionX,  positionY,  positionZ );
	glm::vec3 vView		= glm::vec3 ( viewX,      viewY,      viewZ );
	glm::vec3 vUpVector	= glm::vec3 ( upVectorX,  upVectorY,  upVectorZ );

	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	camPosition     = vPosition;					// Assign the position
	camDirection    = vView;						// Assign the view
	vecUp           = vUpVector;					// Assign the up vector
//	wantedCamPosition = vPosition;
	wantedCamPosition = glm::vec3 ( 0.0 );
}

//-----------------------------------------------------------------------------
//
// Update camera information when collision occurs
void cam_handleCollision()
//-----------------------------------------------------------------------------
{
	currentForwardSpeed = 0.0f;
	currentBackwardSpeed = 0.0f;
	currentLeftSpeed = 0.0f;
	currentRightSpeed = 0.0f;

	camPosition = previousCamPosition;
	camDirection = previousViewPosition;
}


//-----------------------------------------------------------------------------
//
//	Try to move camera, if not slide along collision plane
bool cam_trySlide ( glm::vec3 vFrom )
//-----------------------------------------------------------------------------
{
	glm::vec3 slide;

	while ( fabs ( wantedCamPosition.x ) > 0.0001f || fabs ( wantedCamPosition.y ) > 0.0001f || fabs ( wantedCamPosition.z ) > 0.0001f )
		{
//		sMoveData md = bspCheckSphereMove(vFrom,vFrom + wantedCamPosition, PLAYER_BOUNDING_SPHERE_RADIUS);
			sMoveData md;
			bsp_checkBoxMove ( vFrom, vFrom + wantedCamPosition, boxMin, boxMax, md );

			if ( true == md.AllSolid )
				{
					testCollisionWithBSP = true;
					return false;
				}

			if ( md.Fraction > 0 )
				{
					cam_moveTo ( md.EndPoint );
					return true;
				}

			// can't move so slide
			md.CollisionNormal = glm::normalize ( md.CollisionNormal );
			slide  = wantedCamPosition - md.CollisionNormal* ( glm::dot ( md.CollisionNormal,wantedCamPosition ) );
			wantedCamPosition = slide;
		}

	return false;
}

//-----------------------------------------------------------------------------
//
// Walk up stairs/slopes/small bumps
bool cam_tryStepUpAndSlide()
//-----------------------------------------------------------------------------
{
//	sMoveData md = bspCheckSphereMove(m_vPosition,m_vPosition+STEP_UP, PLAYER_BOUNDING_SPHERE_RADIUS);
	sMoveData md;
	bsp_checkBoxMove ( camPosition, camPosition + STEP_UP, boxMin, boxMax, md );

	if ( md.Fraction == 0 )
		{
			cam_handleCollision();
			return false;
		}

	if ( !cam_trySlide ( md.EndPoint ) )
		{
			cam_handleCollision();
			return false;
		}

//	md = bspCheckSphereMove(m_vPosition,m_vPosition-STEP_UP, PLAYER_BOUNDING_SPHERE_RADIUS);
	bsp_checkBoxMove ( camPosition, camPosition - STEP_UP, boxMin, boxMax, md );

	if ( md.Fraction == 0 )
		{
			cam_handleCollision();
			return false;
		}

	cam_moveTo ( md.EndPoint );
	camMoveDistance = md.Fraction;
	return true;
}

//-----------------------------------------------------------------------------
//
// Called each frame to work out collisions with mesh
void cam_resolveCollisions()
//-----------------------------------------------------------------------------
{
	boxMin.x = -BOX_SIZE;
	boxMin.y = -BOX_SIZE;
	boxMin.z = -BOX_SIZE;

	boxMax.x = BOX_SIZE;
	boxMax.y = BOX_SIZE;
	boxMax.z = BOX_SIZE;

	// try to step up
	//     if not TrySlide
//	if (!cam_tryStepUpAndSlide())
	cam_trySlide ( camPosition );
}
