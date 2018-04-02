#include "btBulletDynamicsCommon.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btGeometryUtil.h"

#include "s_varsBSP.h"
#include "s_physicsDebug.h"
#include "s_doorsBSP.h"

//--------------------------------------------------------------------------
void addConvexVerticesCollider ( btAlignedObjectArray<btVector3>& vertices, bool isEntity, int whichDoor, const btVector3& entityTargetLocation )
//--------------------------------------------------------------------------
{
	if ( vertices.size() > 0 )
		{
			btTransform startTransform;
			//can use a shift
			startTransform.setIdentity();
			startTransform.setOrigin ( btVector3 ( 0,0,0 ) );
			//this create an internal copy of the vertices

			bul_addPhysicsBSP ( 1.0f, isEntity, whichDoor, vertices );
		}
}

//--------------------------------------------------------------------------
//
// See if passed in Texture ID is used on door models.  If so, don't create
// physics convex hull
bool bspIsDoorTexture(int whichTextureID)
//--------------------------------------------------------------------------
{
	for (int i = 0; i != numOfDoors; i++)
		{
			if (whichTextureID == doorModels[i].textureID)
				return true;
		}

	return false;
}

//--------------------------------------------------------------------------
//
// Physics for BSP models
void bspConvertDoors ( int whichDoor, float scaling )
//--------------------------------------------------------------------------
{
	const tBSPModel& model = m_pModels[doorModels[whichDoor].ptrModel];

	for ( int n = 0; n < model.numBrushes; n++ )
		{
			btAlignedObjectArray<btVector3> planeEquations;
			bool	isValidBrush = false;

			//convert brush
			const tBSPBrush& brush = m_pBrushes[model.firstBrush+n];
			{

				doorModels[whichDoor].textureID = brush.textureID;

				for ( int p = 0; p < brush.numOfBrushSides; p++ )
					{
						int sideid = brush.brushSide+p;
						tBSPBrushSide& brushside = m_pBrushSides[sideid];

						int planeid = brushside.plane;
						tBSPPlane& plane = m_pPlanes[planeid];

						btVector3 planeEq;
						planeEq.setValue (
						    plane.vNormal[0],
						    plane.vNormal[1],
						    plane.vNormal[2] );

						planeEq[3] = scaling * -plane.d;
						planeEquations.push_back ( planeEq );
						isValidBrush = true;
					}

				if ( isValidBrush )
					{
						btAlignedObjectArray<btVector3>	vertices;
						btGeometryUtil::getVerticesFromPlaneEquations ( planeEquations,vertices );

						bool isEntity = true;
						btVector3 targetLocation ( 0.f,0.f,0.f );
						addConvexVerticesCollider ( vertices, isEntity, whichDoor, targetLocation );
					}
			}
		}
}


//--------------------------------------------------------------------------
//
// Convert BSP mesh into Bullet physics
void bspConvertMesh ( float scaling )
//--------------------------------------------------------------------------
{
	for ( int i=0; i < m_numOfLeafs; i++ )
		{
			bool isValidBrush = false;

			tBSPLeaf&	leaf = m_pLeafs[i];

			for ( int b=0; b < leaf.numOfLeafBrushes; b++ )
				{
					btAlignedObjectArray<btVector3> planeEquations;

					int brushid = m_pLeafBrushes[leaf.leafBrush+b];

					tBSPBrush& brush = m_pBrushes[brushid];

					//
					// Don't create if this texture is used on a door model
					if (true == bspIsDoorTexture(brush.textureID))
						return;

					con_print(CON_INFO, true, "MLeaf [ %i ] Brush [ %i ] Brush textureID [ %i ]", i, b, brush.textureID);

					if ( brush.textureID != -1 )
						{
							if ( m_pTextures [ brush.textureID ].contents & BSPCONTENTS_SOLID )
								{
									brush.textureID = -1;

									for ( int p=0; p < brush.numOfBrushSides; p++ )
										{
											int sideid = brush.brushSide+p;

											tBSPBrushSide& brushside = m_pBrushSides[sideid];

											int planeid = brushside.plane;

											tBSPPlane& plane = m_pPlanes[planeid];

											btVector3 planeEq;

											planeEq.setValue (
											    plane.vNormal[0],
											    plane.vNormal[1],
											    plane.vNormal[2] );

											planeEq[3] = scaling * -plane.d;

											planeEquations.push_back ( planeEq );
											isValidBrush = true;
										}

									if ( isValidBrush )
										{

											btAlignedObjectArray<btVector3>	vertices;
											btGeometryUtil::getVerticesFromPlaneEquations ( planeEquations,vertices );
											btVector3 entityTarget ( 0.f,0.f,0.f );
											addConvexVerticesCollider ( vertices, false, -1, entityTarget );
										}
								}
						}
				}
		}

	bul_enableDebug ( true );
}
