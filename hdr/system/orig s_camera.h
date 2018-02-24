#pragma once

extern glm::vec3 	camPosition;
extern glm::vec3	camDirection;
extern glm::vec3	wantedCamPosition;

// Used for moving up/down
void camMoveCameraYPos(float speed, float interpolate);

//	This will move the camera forward or backward depending on the speed
void camMoveCamera(float speed, float interpolate);

//	This strafes the camera left and right depending on the speed (-/+)
void camStrafeCamera(float speed, float interpolate);

//	This function sets the camera's position and view and up vVector.
void camPositionCamera(      float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ);
