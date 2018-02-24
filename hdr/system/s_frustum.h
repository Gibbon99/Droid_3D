#pragma once

// BoxInFrustum will return one of this defines according to the position of the box in the frustum
#define COMPLETE_OUT 0
#define INTERSECT    1
#define COMPLETE_IN  2

// Call this every time the camera moves to update the frustum
void CalculateFrustum();

int BoxInFrustum( float x, float y, float z, float x2, float y2, float z2);
