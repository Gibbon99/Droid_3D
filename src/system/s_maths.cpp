#include "s_globals.h"
#include "s_openGL.h"
#include "io_keyboard.h"
#include "s_window.h"
#include "s_movement.h"
#include "s_camera.h"
#include "s_maths.h"
#include "s_varsBSP.h"

// We create an enum of the sides so we don't have to call each side 0 or 1.
// This way it makes it more understandable and readable when dealing with frustum sides.

enum FrustumSide
{
	RIGHT	= 0,		// The RIGHT side of the frustum
	LEFT	= 1,		// The LEFT	 side of the frustum
	BOTTOM	= 2,		// The BOTTOM side of the frustum
	TOP		= 3,		// The TOP side of the frustum
	BACK	= 4,		// The BACK	side of the frustum
	FRONT	= 5			// The FRONT side of the frustum
};


// Like above, instead of saying a number for the ABC and D of the plane, we
// want to be more descriptive.
enum PlaneData
{
	PLANE_A = 0,				// The X value of the plane's normal
	PLANE_B = 1,				// The Y value of the plane's normal
	PLANE_C = 2,				// The Z value of the plane's normal
	PLANE_D = 3				// The distance the plane is from the origin
};

//-----------------------------------------------------------------------------
//
// This normalizes a plane (A side) from a given frustum
void sys_normalizePlane ( float frustum[6][4], int side )
//-----------------------------------------------------------------------------
{
	// Here we calculate the magnitude of the normal to the plane (point A B C)
	// Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
	// To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
	float magnitude = ( float ) sqrt ( frustum[side][PLANE_A] * frustum[side][PLANE_A] +
	                                   frustum[side][PLANE_B] * frustum[side][PLANE_B] +
	                                   frustum[side][PLANE_C] * frustum[side][PLANE_C] );

	// Then we divide the plane's values by it's magnitude.
	// This makes it easier to work with.
	frustum[side][PLANE_A] /= magnitude;
	frustum[side][PLANE_B] /= magnitude;
	frustum[side][PLANE_C] /= magnitude;
	frustum[side][PLANE_D] /= magnitude;
}

//-----------------------------------------------------------------------------
//
// This extracts our frustum from the projection and modelview matrix
void sys_calculateFrustum()
//-----------------------------------------------------------------------------
{
	float   clip[16];								// This will hold the clipping planes

	/*
	float *pSource;

	pSource = (const float*)glm::value_ptr(projMatrix);
	for (int i = 0; i < 16; ++i)
	    proj[i] = pSource[i];

	pSource = (const float*)glm::value_ptr(viewMatrix);
	for (int i = 0; i < 16; ++i)
	    modl[i] = pSource[i];
	*/

	float *proj;
	float *modl;


	proj = glm::value_ptr ( projMatrix );
	modl = glm::value_ptr ( viewMatrix );

	// Now that we have our modelview and projection matrix, if we combine these 2 matrices,
	// it will give us our clipping planes.  To combine 2 matrices, we multiply them.

	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];


	// Now we actually want to get the sides of the frustum.  To do this we take
	// the clipping planes we received above and extract the sides from them.

	// This will extract the RIGHT side of the frustum
	m_Frustum[RIGHT][PLANE_A] = clip[ 3] - clip[ 0];
	m_Frustum[RIGHT][PLANE_B] = clip[ 7] - clip[ 4];
	m_Frustum[RIGHT][PLANE_C] = clip[11] - clip[ 8];
	m_Frustum[RIGHT][PLANE_D] = clip[15] - clip[12];

	// Now that we have a normal (A,B,C) and a distance (D) to the plane,
	// we want to normalize that normal and distance.

	// Normalize the RIGHT side
	sys_normalizePlane ( m_Frustum, RIGHT );

	// This will extract the LEFT side of the frustum
	m_Frustum[LEFT][PLANE_A] = clip[ 3] + clip[ 0];
	m_Frustum[LEFT][PLANE_B] = clip[ 7] + clip[ 4];
	m_Frustum[LEFT][PLANE_C] = clip[11] + clip[ 8];
	m_Frustum[LEFT][PLANE_D] = clip[15] + clip[12];

	// Normalize the LEFT side
	sys_normalizePlane ( m_Frustum, LEFT );

	// This will extract the BOTTOM side of the frustum
	m_Frustum[BOTTOM][PLANE_A] = clip[ 3] + clip[ 1];
	m_Frustum[BOTTOM][PLANE_B] = clip[ 7] + clip[ 5];
	m_Frustum[BOTTOM][PLANE_C] = clip[11] + clip[ 9];
	m_Frustum[BOTTOM][PLANE_D] = clip[15] + clip[13];

	// Normalize the BOTTOM side
	sys_normalizePlane ( m_Frustum, BOTTOM );

	// This will extract the TOP side of the frustum
	m_Frustum[TOP][PLANE_A] = clip[ 3] - clip[ 1];
	m_Frustum[TOP][PLANE_B] = clip[ 7] - clip[ 5];
	m_Frustum[TOP][PLANE_C] = clip[11] - clip[ 9];
	m_Frustum[TOP][PLANE_D] = clip[15] - clip[13];

	// Normalize the TOP side
	sys_normalizePlane ( m_Frustum, TOP );

	// This will extract the BACK side of the frustum
	m_Frustum[BACK][PLANE_A] = clip[ 3] - clip[ 2];
	m_Frustum[BACK][PLANE_B] = clip[ 7] - clip[ 6];
	m_Frustum[BACK][PLANE_C] = clip[11] - clip[10];
	m_Frustum[BACK][PLANE_D] = clip[15] - clip[14];

	// Normalize the BACK side
	sys_normalizePlane ( m_Frustum, BACK );

	// This will extract the FRONT side of the frustum
	m_Frustum[FRONT][PLANE_A] = clip[ 3] + clip[ 2];
	m_Frustum[FRONT][PLANE_B] = clip[ 7] + clip[ 6];
	m_Frustum[FRONT][PLANE_C] = clip[11] + clip[10];
	m_Frustum[FRONT][PLANE_D] = clip[15] + clip[14];

	// Normalize the FRONT side
	sys_normalizePlane ( m_Frustum, FRONT );
}

//-----------------------------------------------------------------------------
//
// This determines if a BOX is in or around our frustum by it's min and max points
int sys_boxInFrustum ( float x, float y, float z, float x2, float y2, float z2 )
//-----------------------------------------------------------------------------
{
	// Detailed explanation on the BSP tutorial 4
	byte mode=0;					// set IN and OUT bit to 0

	for ( int i = 0; i < 6; i++ )
		{
			mode &= V_OUT;				// clear the IN bit to 0

			if ( m_Frustum[i][PLANE_A] * x  + m_Frustum[i][PLANE_B] * y  + m_Frustum[i][PLANE_C] * z  + m_Frustum[i][PLANE_D] >= 0 )
				mode |= V_IN;  // set IN bit to 1

			else
				mode |= V_OUT; // set OUT bit to 1

			if ( mode == V_INTERSECT ) continue; // if we found a vertex IN for THIS plane and

			// a vertex OUT of ANY plane continue ( we have enough information to say:
			// INTERSECT! IF there is not vertex missing from the FRONT of the remaining planes)

			if ( m_Frustum[i][PLANE_A] * x2 + m_Frustum[i][PLANE_B] * y  + m_Frustum[i][PLANE_C] * z  + m_Frustum[i][PLANE_D] >= 0 )  mode |= V_IN;

			else mode |= V_OUT;

			if ( mode == V_INTERSECT ) continue;

			if ( m_Frustum[i][PLANE_A] * x  + m_Frustum[i][PLANE_B] * y2 + m_Frustum[i][PLANE_C] * z  + m_Frustum[i][PLANE_D] >= 0 )  mode |= V_IN;

			else mode |= V_OUT;

			if ( mode == V_INTERSECT ) continue;

			if ( m_Frustum[i][PLANE_A] * x2 + m_Frustum[i][PLANE_B] * y2 + m_Frustum[i][PLANE_C] * z  + m_Frustum[i][PLANE_D] >= 0 )  mode |= V_IN;

			else mode |= V_OUT;

			if ( mode == V_INTERSECT ) continue;

			if ( m_Frustum[i][PLANE_A] * x  + m_Frustum[i][PLANE_B] * y  + m_Frustum[i][PLANE_C] * z2 + m_Frustum[i][PLANE_D] >= 0 )  mode |= V_IN;

			else mode |= V_OUT;

			if ( mode == V_INTERSECT ) continue;

			if ( m_Frustum[i][PLANE_A] * x2 + m_Frustum[i][PLANE_B] * y  + m_Frustum[i][PLANE_C] * z2 + m_Frustum[i][PLANE_D] >= 0 )  mode |= V_IN;

			else mode |= V_OUT;

			if ( mode == V_INTERSECT ) continue;

			if ( m_Frustum[i][PLANE_A] * x  + m_Frustum[i][PLANE_B] * y2 + m_Frustum[i][PLANE_C] * z2 + m_Frustum[i][PLANE_D] >= 0 )  mode |= V_IN;

			else mode |= V_OUT;

			if ( mode == V_INTERSECT ) continue;

			if ( m_Frustum[i][PLANE_A] * x2 + m_Frustum[i][PLANE_B] * y2 + m_Frustum[i][PLANE_C] * z2 + m_Frustum[i][PLANE_D] >= 0 )  mode |= V_IN;

			else mode |= V_OUT;

			if ( mode == V_INTERSECT ) continue;

			// if we arrive to this point, then there are two possibilities:
			// there is not vertices in or there is not intersection till now, if
			// there is a vertice in, continue (we are not over!)
			if ( mode == V_IN ) continue;

			// there is not vertex IN front of this plane, so the box is COMPLETE_OUT
			return COMPLETE_OUT;
		}

	// All planes has a vertex IN FRONT so or the box is intersecting or complete IN
	if ( mode == V_INTERSECT )
		return INTERSECT;

	else
		return COMPLETE_IN;
}
