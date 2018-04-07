#include "s_globals.h"
#include "s_keyboard.h"
#include "s_window.h"

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;

// Camera Attributes
glm::vec3 cam3_Position;
glm::vec3 cam3_Front;
glm::vec3 cam3_Up;
glm::vec3 cam3_Right;
glm::vec3 cam3_WorldUp;

glm::vec3	cam3_WantedPosition;

// Euler Angles
float cam3_Yaw;
float cam3_Pitch;

// Camera options
float cam3_MovementSpeed;
float cam3_MouseSensitivity;

//-------------------------------------------------------------------------------
//
// Return the direction vector and turn this into velocity for physics
glm::vec3 cam3_getVelocity()
//-------------------------------------------------------------------------------
{
	glm::vec3 velocityVector;
	
	//
	// If we have moved position from last time, then
	// calculate the direction vector and return it
	//
	// Otherwise return 0.0 to indicate no force is applied
	// to the player body
	//
	if (cam3_WantedPosition != cam3_Position)
	{
		velocityVector = cam3_Position - cam3_WantedPosition;
		velocityVector = glm::normalize(velocityVector);
		velocityVector.y = 0.0f;	// Lock movement to the X/Z plane
		return velocityVector;
	}
	
//	cam3_Position.y = 40.0f;
	
	return glm::vec3(0.0f);		// Apply no force / impulse this time
}

//-------------------------------------------------------------------------------
//
void cam3_updateCameraVectors()
//-------------------------------------------------------------------------------
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos ( glm::radians ( cam3_Yaw ) ) * cos ( glm::radians ( cam3_Pitch ) );
	front.y = sin ( glm::radians ( cam3_Pitch ) );
	front.z = sin ( glm::radians ( cam3_Yaw ) ) * cos ( glm::radians ( cam3_Pitch ) );
	cam3_Front = glm::normalize ( front );
	
	// Also re-calculate the Right and Up vector
	cam3_Right = glm::normalize ( glm::cross ( cam3_Front, cam3_WorldUp ) ); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	cam3_Up    = glm::normalize ( glm::cross ( cam3_Right, cam3_Front ) );
}

//-------------------------------------------------------------------------------
//
// Init the camera with known values
void cam3_initCamera ( glm::vec3 location )
//-------------------------------------------------------------------------------
{
	cam3_Position = location;
	camPosition = location;
	cam3_Front = glm::vec3 ( 0.0f, 0.0f, -0.2f );
	cam3_WorldUp = glm::vec3 ( 0.0f, 1.0f, 0.0f );
	cam3_Up = glm::vec3 (0.0f, 1.0f, 0.0f );
	cam3_Yaw = 0.0f;
	cam3_Pitch = 0.0f;
	
	cam3_MouseSensitivity = 0.15f;
	cam3_MovementSpeed = 0.002f;

	cam3_updateCameraVectors();
}

//-------------------------------------------------------------------------------
//
// Create the view matrix
void cam3_CreateViewMatrix(glm::vec3 newPosition)
//-------------------------------------------------------------------------------
{
	viewMatrix = glm::lookAt ( newPosition, newPosition + cam3_Front, cam3_Up );
}

//-------------------------------------------------------------------------------
//
// Process movement keys
void cam3_processMovementKeys ( float interpolate )
//-------------------------------------------------------------------------------
{
	float velocity = cam3_MovementSpeed * interpolate;

	cam3_WantedPosition = cam3_Position;

	if ( keyForwardDown == true )
		cam3_Position += cam3_Front * velocity;

	if ( keyBackwardDown == true )
		cam3_Position -= cam3_Front * velocity;

	if ( keyLeftDown == true )
		cam3_Position -= cam3_Right * velocity;

	if ( keyRightDown == true )
		cam3_Position += cam3_Right * velocity;
}

//-------------------------------------------------------------------------------
//
// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void cam3_processMouseMovement ( float xoffset, float yoffset, GLboolean constrainPitch = true )
//-------------------------------------------------------------------------------
{
	xoffset *= cam3_MouseSensitivity;
	yoffset *= cam3_MouseSensitivity;

	cam3_Yaw   += -xoffset;
	cam3_Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if ( constrainPitch )
		{
			if ( cam3_Pitch > 89.0f )
				cam3_Pitch = 89.0f;
			if ( cam3_Pitch < -89.0f )
				cam3_Pitch = -89.0f;
		}

	// Update Front, Right and Up Vectors using the updated Euler angles
	cam3_updateCameraVectors();
	
	//
	// Reset back to the middle of the screen
	lib_setMousePos ( ( winWidth / 2 ), ( winHeight / 2 ) );
}
