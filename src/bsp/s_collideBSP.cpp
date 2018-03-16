#include "s_globals.h"
#include "s_collideBSP.h"
#include "s_varsBSP.h"

bool            bspCollisionDetectOn;
bool            collisionWithBSP = false;
bool            testCollisionWithBSP = false;

byte            MoveObjectType;     // This hold the Object Type (Ray / Sphere)
float           MoveOffset;			// This hold the Object Offset (Ray = 0, Sphere = Radius)
glm::vec3       traceMaxs;
glm::vec3       traceMins;
glm::vec3       MStart;
glm::vec3       MEnd;
glm::vec3       traceExtents;

//----------------------------------------------------------------------
//
// BSP collision
void bsp_checkTouchBrush ( tBSPBrush *brush, int leafNode, sMoveData &localDataMoveBrush )
//----------------------------------------------------------------------
{
	float startFraction = -1.0f;	// The S in the pictures
	float endFraction = 1.0f;		// The E in the pictures
	bool  startsOut = false;		// If not plane change this value then the ray start IN
	bool  endsOut = false;			// If not plane change this value then the ray end IN

	float startDistance;
	float endDistance;

	glm::vec3 CandidateToHitNormal;

	// Go for every brush side
	for ( int i = 0; i < brush->numOfBrushSides; i++ )
		{
			// take a pointer to the brush side and the plane it represent
			tBSPBrushSide *brushSide = &m_pBrushSides[brush->brushSide + i];
			tBSPPlane     *plane	 = &m_pPlanes[brushSide->plane];

			if ( CO_BOX == MoveObjectType )
				{
					glm::vec3 offset;

					if ( plane->vNormal.x < 0 )
						offset.x = traceMaxs.x;

					else
						offset.x = traceMins.x;

					if ( plane->vNormal.y < 0 )
						offset.y = traceMaxs.y;

					else
						offset.y = traceMins.y;

					if ( plane->vNormal.z < 0 )
						offset.z = traceMaxs.z;

					else
						offset.z = traceMins.z;

					startDistance = ( MStart.x + offset.x ) * plane->vNormal.x +
					                ( MStart.y + offset.y ) * plane->vNormal.y +
					                ( MStart.z + offset.z ) * plane->vNormal.z - plane->d;
					endDistance = ( MEnd.x + offset.x ) * plane->vNormal.x +
					              ( MEnd.y + offset.y ) * plane->vNormal.y +
					              ( MEnd.z + offset.z ) * plane->vNormal.z - plane->d;

				}

			else
				{
					// Compute distances from the plane to the MStart and MEnd (the values enter at the start of the trace
					startDistance =  	plane->vNormal.x * MStart.x +
					                    plane->vNormal.y * MStart.y +
					                    plane->vNormal.z * MStart.z -
					                    plane->d - MoveOffset;
					endDistance   =  	plane->vNormal.x * MEnd.x +
					                    plane->vNormal.y * MEnd.y +
					                    plane->vNormal.z * MEnd.z -
					                    plane->d - MoveOffset;
				}

			// if the start point is in front this plane, then it can be IN the brush so it's OUT
			if ( startDistance > 0 )	startsOut = true;

			// if the end point is in front this plane, then it can be IN the brush so it's OUT
			if ( endDistance > 0 )	endsOut = true;


			// make sure the trace isn't completely on one side of the brush
			if ( startDistance > 0 && endDistance > 0 )
				{
					// both are in front of the plane, its outside of this brush
					return;
				}

			if ( startDistance <= 0 && endDistance <= 0 )
				{
					// both are behind this plane, it will get clipped by another one
					continue;
				}


			// The line is Red so is entering the brush.
			// Compute the new S   (fraction)
			// if (the new S) > S    move S  and update CandidateToHitNormal

			if ( startDistance > endDistance )
				{
					float fraction = ( startDistance - EPSILON ) / ( startDistance - endDistance );

					if ( fraction > startFraction )
						{
							startFraction = fraction;
							CandidateToHitNormal = plane->vNormal;
						}

				}

			else
				// The line is Black so is leaving the brush.
				// Compute the new E   (fraction)
				// if (the new E) < E    move E
				{
					float fraction = ( startDistance + EPSILON ) / ( startDistance - endDistance );

					if ( fraction < endFraction )
						endFraction = fraction;
				}
		}

	// After checking all sides if startOut remain false, then the Start Point is complete in the brush
	if ( startsOut == false )
		{
			localDataMoveBrush.StartOut = false;

			// if the EndPoint is also in this brush, then the ray is complete in this brush
			if ( endsOut == false )
				localDataMoveBrush.AllSolid = true;

			return;
		}

	// if there was collision against the brush (S < E)
	if ( startFraction < endFraction )
		{
			// And of course if we move S (if we dont then we dont enter the brush
			// and we dont collide with some nearest brush
			if ( startFraction > -1 && startFraction < localDataMoveBrush.Fraction )
				{
					// UpDate OutPut values
					if ( startFraction < 0 )
						startFraction = 0;

					localDataMoveBrush.Fraction		        = startFraction;
					localDataMoveBrush.CollisionNormal      = CandidateToHitNormal;
					localDataMoveBrush.nodeIndex		    = leafNode;

					return;
				}
		}
}

//----------------------------------------------------------------------
//
// Check if we collide with something of this node ( including children and leafs
void bsp_checkMoveByNode ( int nodeIndex, float startFraction, float endFraction, glm::vec3 Start, glm::vec3 End, sMoveData &localMoveDataNode )
//----------------------------------------------------------------------
{
	// If the path was blocked by a nearer obstacle, don't bother checking
	if ( localMoveDataNode.Fraction <= startFraction )
		return;

	if ( nodeIndex < 0 )
		{
			// this is a leaf
			tBSPLeaf *leaf = &m_pLeafs[~nodeIndex]; // Get a pointer from the m_pLeafs Array.

			// Remember that NodeIndex is negative so the corresponding leafs is
			// ~nodeIndex which is same of saying -(NodeIndex+1) but faster!
			// Loop by all the brushes
			for ( int i = 0; i < leaf->numOfLeafBrushes; i++ )
				{
					//Take a point to the i-th brush in the leaf
					tBSPBrush *brush = &m_pBrushes[m_pLeafBrushes[leaf->leafBrush + i]];

					// If the brush has sides (is a convex volume) and is SOLID then check it out!
					if ( brush->numOfBrushSides > 0 && ( m_pTextures[brush->textureID].contents & 1 ) )
						{
							bsp_checkTouchBrush ( brush, nodeIndex, localMoveDataNode );
						}
				}

			// don't have to do anything else for leaves
			return;
		}

	// this is a node.
	// Get pointer to the Node it self and to the splitter plane
	tBSPNode  *node  = &m_pNodes[nodeIndex];
	tBSPPlane *plane = &m_pPlanes[node->plane];

	// Get the distance from the start and end point to the plane. This is done with the Plane formula:
	float startDistance =  plane->vNormal.x * Start.x +
	                       plane->vNormal.y * Start.y +
	                       plane->vNormal.z * Start.z -
	                       plane->d;
	float endDistance   =  plane->vNormal.x * End.x +
	                       plane->vNormal.y * End.y +
	                       plane->vNormal.z * End.z -
	                       plane->d;

	if ( CO_BOX == MoveObjectType )
		{
			// If both points are in front of the plane (the positive side) pass the ray to the front
			// child as the ray comes.
			// this is just a dot product, but we want the absolute values
			MoveOffset = ( float ) ( fabs ( traceExtents.x * plane->vNormal.x ) +
			                         fabs ( traceExtents.y * plane->vNormal.y ) +
			                         fabs ( traceExtents.z * plane->vNormal.z ) );
		}

	if ( startDistance >= MoveOffset && endDistance >= MoveOffset )
		{
			bsp_checkMoveByNode ( node->front, startFraction, endFraction, Start, End, localMoveDataNode );
		}

	// If both points are in behind the plane (the negative side) pass the ray to the back
	// child as the ray comes.

	else if ( startDistance < -MoveOffset && endDistance < -MoveOffset )
		{
			bsp_checkMoveByNode ( node->back, startFraction, endFraction, Start, End, localMoveDataNode );
		}

	else
		{
			// the line spans the splitting plane
			int side1, side2;  // Variables to hold which side to traverse first and which second
			float fraction1, fraction2;	   // Used to know compute the Smiddle and Emiddle.
			glm::vec3 middle;			   // middle point (Smiddle and Emiddle)

			// split the segment into two

			// If we know that one is positive and the other is negative, and startDistance < endDistance, so
			// startDistance is negative and endDistance is positive (front and back correspondingly)
			if ( startDistance < endDistance )
				{
					side1 = node->back;    // the back side contains the start point so the back will be the first
					side2 = node->front;
					float inverseDistance = 1.0f / ( startDistance - endDistance ); // optimization
					fraction1 = ( startDistance - EPSILON - MoveOffset ) * inverseDistance;	 // compute fraction for the start-middle
					fraction2 = ( startDistance + EPSILON + MoveOffset ) * inverseDistance;	 // compute fraction for the middle-end
				}

			// On the other hand if endDistance is negative and startDistance is positive
			else if ( endDistance < startDistance )
				{
					side1 = node->front;   // the front side contains the start point so the front will be the first
					side2 = node->back;
					float inverseDistance = 1.0f / ( startDistance - endDistance );
					fraction1 = ( startDistance + EPSILON + MoveOffset ) * inverseDistance;
					fraction2 = ( startDistance - EPSILON - MoveOffset ) * inverseDistance;

				}

			else
				{
					side1 = node->front;
					side2 = node->back;
					fraction1 = 1.0f;
					fraction2 = 0.0f;
				}

			// make sure the numbers are valid
			if ( fraction1 < 0.0f ) fraction1 = 0.0f;

			else if ( fraction1 > 1.0f ) fraction1 = 1.0f;

			if ( fraction2 < 0.0f ) fraction2 = 0.0f;

			else if ( fraction2 > 1.0f ) fraction2 = 1.0f;

			// calculate the middle point for the first side
			middle		   = Start + ( End - Start ) *fraction1;

			// get the Smiddle fraction (not just the point)
			float middleFraction = startFraction + ( endFraction - startFraction ) * fraction1;

			// check the first side
			bsp_checkMoveByNode ( side1, startFraction, middleFraction, Start, middle, localMoveDataNode );

			// calculate the middle point for the second side
			middle = Start + ( End - Start ) *fraction2;

			// get the Emiddle fraction (not just the point)
			middleFraction = startFraction + ( endFraction - startFraction ) * fraction2;

			// check the second side
			bsp_checkMoveByNode ( side2, middleFraction, endFraction, middle, End, localMoveDataNode );
		}
}

//----------------------------------------------------------------------
//
// Check a ray against the BSP tree
void bsp_checkRayMove ( glm::vec3 Start, glm::vec3 End, sMoveData &localMoveDataRay )
//----------------------------------------------------------------------
{
	// Initializations, This will change if the traversal detect it was wrong
	localMoveDataRay.StartOut = true;
	localMoveDataRay.AllSolid = false;
	localMoveDataRay.Fraction = 1.0f;

	// Necessary info to collide with brushes
	MStart = Start;
	MEnd = End;
	MoveObjectType = CO_RAY;
	MoveOffset = 0;

	// walk through the BSP tree
	bsp_checkMoveByNode ( 0, 0.0f, 1.0f, Start, End, localMoveDataRay );

	if ( localMoveDataRay.Fraction == 1.0f )
		{
			// nothing blocked the trace
			localMoveDataRay.EndPoint = End;

		}

	else
		{
			// collided with something
			localMoveDataRay.EndPoint = Start + ( End-Start ) * localMoveDataRay.Fraction;
		}

	return;
}

//----------------------------------------------------------------------
//
// Check Box shape against BSP tree
void bsp_checkBoxMove ( glm::vec3 Start, glm::vec3 End, glm::vec3 inputMins, glm::vec3 inputMaxs, sMoveData &localDataMoveBox )
//----------------------------------------------------------------------
{
	// Initializations, This will change if the traversal detect it was wrong
	localDataMoveBox.StartOut = true;
	localDataMoveBox.AllSolid = false;
	localDataMoveBox.Fraction = 1.0f;

	// Necessary info to collide with brushes
	MStart = Start;
	MEnd = End;

	MoveObjectType	= CO_BOX;

	if ( inputMins.x == 0 && inputMins.y == 0 && inputMins.z == 0 &&
	        inputMaxs.x == 0 && inputMaxs.y == 0 && inputMaxs.z == 0 )
		{
			// the user TraceBox, but this is actually a ray
			bsp_checkRayMove ( Start, End, localDataMoveBox );

		}

	else
		{
			// setup for a box
			traceMins = inputMins;
			traceMaxs = inputMaxs;
			traceExtents.x = -traceMins.x > traceMaxs.x ? -traceMins.x : traceMaxs.x;
			traceExtents.y = -traceMins.y > traceMaxs.y ? -traceMins.y : traceMaxs.y;
			traceExtents.z = -traceMins.z > traceMaxs.z ? -traceMins.z : traceMaxs.z;
			// walk through the BSP tree
			bsp_checkMoveByNode ( 0, 0.0f, 1.0f, Start, End, localDataMoveBox );
		}

	if ( localDataMoveBox.Fraction == 1.0f )
		{
			// nothing blocked the trace
			localDataMoveBox.EndPoint = End;

		}

	else
		{
			// collided with something
			localDataMoveBox.EndPoint = Start + ( End-Start ) * localDataMoveBox.Fraction;
			testCollisionWithBSP = true;
//		currentFallingSpeed = 0.0f;
		}

	return;
}

//----------------------------------------------------------------------
//
// Check sphere ( with radius ) against BSP tree
void bsp_checkSphereMove ( glm::vec3 Start, glm::vec3 End, float Radius, sMoveData &localMoveDataSphere )
//----------------------------------------------------------------------
{
	// Initializations, This will change if the traversal detect it was wrong
	localMoveDataSphere.StartOut = true;
	localMoveDataSphere.AllSolid = false;
	localMoveDataSphere.Fraction = 1.0f;

	// Necessary info to collide with brushes
	MStart = Start;
	MEnd = End;

	MoveObjectType	= CO_SPHERE;
	MoveOffset		= Radius;

	// walk through the BSP tree
	bsp_checkMoveByNode ( 0, 0.0f, 1.0f, Start, End, localMoveDataSphere );

	if ( localMoveDataSphere.Fraction == 1.0f )
		{
			// nothing blocked the trace
			localMoveDataSphere.EndPoint = End;

		}

	else
		{
			// collided with something
			localMoveDataSphere.EndPoint = Start + ( End-Start ) * localMoveDataSphere.Fraction;
			testCollisionWithBSP = true;
		}

	return;
}
