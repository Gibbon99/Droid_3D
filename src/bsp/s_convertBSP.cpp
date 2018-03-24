#include "btBulletDynamicsCommon.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btGeometryUtil.h"

#include "s_varsBSP.h"
#include "s_physics.h"
#include "s_doorsBSP.h"

//--------------------------------------------------------------------------
void addConvexVerticesCollider ( btAlignedObjectArray<btVector3>& vertices, bool isEntity, int whichDoor, const btVector3& entityTargetLocation )
//--------------------------------------------------------------------------

{
	///perhaps we can do something special with entities (isEntity)
	///like adding a collision Triggering (as example)
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
// Physics for BSP models
void bspConvertDoors ( int whichDoor, float scaling )
//--------------------------------------------------------------------------
{
	const tBSPModel& model = m_pModels[whichDoor];

	for ( int n=0; n < model.numBrushes; n++ )
		{
			btAlignedObjectArray<btVector3> planeEquations;
			bool	isValidBrush = false;

			//convert brush
			const tBSPBrush& brush = m_pBrushes[model.firstBrush+n];
			{
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
//			printf ( "Reading bspLeaf %i from total %i (%f procent)\n",i, m_numOfLeafs, ( 100.f* ( float ) i/float ( m_numOfLeafs ) ) );

			bool isValidBrush = false;

//			tBSPLeaf&	leaf = bspLoader.m_dleafs[i];
			tBSPLeaf&	leaf = m_pLeafs[i];

			for ( int b=0; b < leaf.numOfLeafBrushes; b++ )
				{
					btAlignedObjectArray<btVector3> planeEquations;

//					int brushid = bspLoader.m_dleafbrushes[leaf.firstLeafBrush+b];
					int brushid = m_pLeafBrushes[leaf.leafBrush+b];

//					tBSPBrush& brush = bspLoader.m_dbrushes[brushid];
					tBSPBrush& brush = m_pBrushes[brushid];

					if ( brush.textureID != -1 )
						{
							if ( m_pTextures [ brush.textureID ].contents & BSPCONTENTS_SOLID )
//							if ( bspLoader.m_dshaders[ brush.shaderNum ].contentFlags & BSPCONTENTS_SOLID )
								{
//									brush.shaderNum = -1;
									brush.textureID = -1;

									for ( int p=0; p < brush.numOfBrushSides; p++ )
										{
											int sideid = brush.brushSide+p;

//											tBSPBrushSide& brushside = bspLoader.m_dbrushsides[sideid];
											tBSPBrushSide& brushside = m_pBrushSides[sideid];

											int planeid = brushside.plane;

//											tBSPPlane& plane = bspLoader.m_dplanes[planeid];
											tBSPPlane& plane = m_pPlanes[planeid];

											btVector3 planeEq;

											planeEq.setValue (
											    plane.vNormal[0],
											    plane.vNormal[1],
											    plane.vNormal[2] );

											planeEq[3] = scaling * -plane.d;

											planeEquations.push_back ( planeEq );
											isValidBrush=true;
										}

									if ( isValidBrush )
										{

											btAlignedObjectArray<btVector3>	vertices;
											btGeometryUtil::getVerticesFromPlaneEquations ( planeEquations,vertices );

											bool isEntity = false;
											btVector3 entityTarget ( 0.f,0.f,0.f );
											addConvexVerticesCollider ( vertices,isEntity, -1, entityTarget );
										}
								}
						}
				}
		}
	bul_enableDebug ( true );
}
