#include "s_globals.h"
#include "s_camera2.h"
#include "s_window.h"
#include "s_keyboard.h"

glm::vec3	cam2Location;
glm::vec3	cam2Rotation;
glm::vec3	cam2Direction;

float 		moveSpeedFactor = 20.0f;

float		pitchSensitivity;
float		yawSensitivity;

//------------------------------------------------------------------
//
// Set the initial values for the camera
void cam_initCamera ( glm::vec3 location, glm::vec3 rotation )
//------------------------------------------------------------------
{
	cam2Location = location;
	cam2Rotation = rotation;
	cam2Direction = location + glm::vec3(0.0f, 0.0f, -5.0f);

	pitchSensitivity = 0.15f; // Mouse sensitivity when looking up and down
	yawSensitivity   = 0.15f; // Mouse sensitivity when looking left and right
}

//------------------------------------------------------------------
//
// Handle mouse movement changes
void cam_handleMouseMovement ( float mouseX, float mouseY )
//------------------------------------------------------------------
{
	// Calculate our horizontal and vertical mouse movement from middle of the window
	float horizMouseMovement = mouseX * yawSensitivity;
	float vertMouseMovement  = mouseY * pitchSensitivity;

	// Apply the mouse movement to our rotation vector. The vertical (look up and down
	// i.e. pitch) rotation is applied on the X axis, and the horizontal (look left and
	// i.e. yaw) rotation is applied on the Y Axis.
	cam2Rotation.x += vertMouseMovement;
	cam2Rotation.y += horizMouseMovement;

	// Limit loking up to vertically up
	if ( cam2Rotation.x < -90.0f )
		{
			cam2Rotation.x = -90.0f;
		}

	// Limit looking down to vertically down
	if ( cam2Rotation.x >  90.0f )
		{
			cam2Rotation.x = 90.0f;
		}

	// Looking left and right - keep angles in the range 0.0 to 360.0
	// 0 degrees is looking directly down the negative Z axis "North", 90 degrees is "East", 180 degrees is "South", 270 degrees is "West"
	// We can also do this so that our 360 degrees goes -180 through +180 and it works the same, but it's probably best to keep our
	// range to 0 through 360 instead of -180 through +180.
	if ( cam2Rotation.y < 0.0f )
		{
			cam2Rotation.y += 360.0f;
		}
	if ( cam2Rotation.y > 360.0f )
		{
			cam2Rotation.y -= 360.0f;
		}
	//
	// Reset back to the middle of the screen
	lib_setMousePos ( ( winWidth / 2 ), ( winHeight / 2 ) );
}

//------------------------------------------------------------------
//
// Create the view matrix
void cam_createViewMatrix(glm::vec3 camLocation)
//------------------------------------------------------------------
{
	// Reset our View matrix
	viewMatrix = mat4 ( 1.0f );

	// Perform camera rotation
	viewMatrix = glm::rotate ( viewMatrix, cam2Rotation.x, X_AXIS );
	viewMatrix = glm::rotate ( viewMatrix, cam2Rotation.y, Y_AXIS );

	// Translate to our camera position
	viewMatrix = glm::translate ( viewMatrix, -cam2Location );
}

//------------------------------------------------------------------
//
// Calculate which direction we need to move the camera and by what amount
glm::vec3 cam_getVelocity ( float interpolate )
//------------------------------------------------------------------
{
	// Vector to break up our movement into components along the X, Y and Z axis
	vec3 movement(0.0f, 0.0f, 0.0f);

	// Get the sine and cosine of our x and y axis rotation (specified in radians)
	float sinXRot = sin(cam2Rotation.x);
	float cosXRot = cos(cam2Rotation.x);

	float sinYRot = sin(cam2Rotation.y);
	float cosYRot = cos(cam2Rotation.y);

    // This cancels out moving on the Z axis when we're looking up or down
	float pitchLimitFactor = cosXRot;

    // Apply relevant movement to the camera location
	if (keyForwardDown)
	{
		movement += vec3(sinYRot * pitchLimitFactor, -sinXRot, -cosYRot * pitchLimitFactor);
	}

	if (keyBackwardDown)
	{
		movement += vec3(-sinYRot * pitchLimitFactor, sinXRot, cosYRot * pitchLimitFactor);
	}
	
	if (keyLeftDown)
	{
		movement -= vec3(cosYRot, 0.0f, sinYRot);
	}

	if (keyRightDown)
	{
		movement += vec3(cosYRot, 0.0f, sinYRot);
	}
		
	// Normalise our movement vector, but ONLY if it's non-zero! Normalising a vector of zero length
	// leads to the new vector having a length of NaN (Not a Number) because of the divide by zero.
	vec3 normalizedMovement(0.0f, 0.0f, 0.0f);
	if (glm::length(movement) != 0.0f)
	{
		normalizedMovement = glm::normalize(movement);
	}

	// Calculate our value to keep the movement the same speed regardless of the framerate...
	double framerateIndependentFactor = moveSpeedFactor * interpolate;

	// .. and then apply it to our movement vector.
	normalizedMovement *= framerateIndependentFactor;

	normalizedMovement.y = 0.0f;

	cam2Direction = normalizedMovement;

	// Finally, apply the movement to our camera location
	cam2Location += normalizedMovement;
	
	return cam2Direction;
}

glm::vec3 cam_getLocation()
{
	return cam2Location;
}