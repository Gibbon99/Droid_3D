#include "btBulletDynamicsCommon.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btGeometryUtil.h"

#include "s_varsBSP.h"
#include "s_physics.h"

//--------------------------------------------------------------------------
void addConvexVerticesCollider ( btAlignedObjectArray<btVector3>& vertices, bool isEntity, const btVector3& entityTargetLocation )
//--------------------------------------------------------------------------

{
	///perhaps we can do something special with entities (isEntity)
	///like adding a collision Triggering (as example)

	if ( vertices.size() > 0 )
		{
			float mass = 0.f;
			btTransform startTransform;
			//can use a shift
			startTransform.setIdentity();
			startTransform.setOrigin ( btVector3 ( 0,0,-10.f ) );
			//this create an internal copy of the vertices

			btCollisionShape* shape = new btConvexHullShape ( & ( vertices[0].getX() ),vertices.size() );
			
	
			bul_addPhysicsObject (0, 0, PHYSICS_OBJECT_BSP, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), vertices);
			
			//m_demoApp->m_collisionShapes.push_back ( shape );

			//m_demoApp->createRigidBody ( mass, startTransform,shape );
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
			printf ( "Reading bspLeaf %i from total %i (%f procent)\n",i, m_numOfLeafs, ( 100.f* ( float ) i/float ( m_numOfLeafs ) ) );

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

//					if ( brush.shaderNum != -1 )
					{
//							if ( bspLoader.m_dshaders[ brush.shaderNum ].contentFlags & BSPCONTENTS_SOLID )
						{
//									brush.shaderNum = -1;

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
									//addConvexVerticesCollider ( vertices,isEntity,entityTarget );
								}
						}
					}
				}
		}
}
