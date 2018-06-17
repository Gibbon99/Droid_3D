#include "s_bullet.h"
#include "s_particles.h"
#include "s_globals.h"
#include "s_physics.h"
#include "s_physicsCollision.h"
#include "s_bullet.h"

vector<_collisionObject>		collisionObjects;

//---------------------------------------------------------------------------
//
// Show collision object data
void phy_debugShowCollisionData()
//---------------------------------------------------------------------------
{
	con_print(CON_INFO, true, "---------------------------");

	for (unsigned int i = 0; i != collisionObjects.size(); i++)
		{
			con_print(CON_INFO, true, "%i - source [ %i ] index [ %i ]", i, collisionObjects[i].objectSource, collisionObjects[i].objectIndex);
		}

	con_print(CON_INFO, true, "---------------------------");
}

//---------------------------------------------------------------------------
//
// Add an object to the collision object list for checking on collision
//
// Return the index into the collisionObjects list
int phy_addCollisionObject(int objectType, int objectIndex)
//---------------------------------------------------------------------------
{
	//
	// TODO: Need to limit size of array ??
	//
	_collisionObject		tempCollisionObject;

	for (unsigned int i = 0; i != collisionObjects.size(); i++)
	{
		if (false == collisionObjects[i].active)
		{
			collisionObjects[i].active = true;
			collisionObjects[i].objectSource = objectType;
			collisionObjects[i].objectIndex = objectIndex;
			return i;
		}
	}
	//
	// No empty slots so add a new one
	//
	tempCollisionObject.objectSource = objectType;
	tempCollisionObject.objectIndex = objectIndex;
	tempCollisionObject.active = true;

	collisionObjects.push_back(tempCollisionObject);

	return collisionObjects.size() - 1;
}

//---------------------------------------------------------------------------
//
// Remove a collision object from the list
void phy_removeCollisionObject( int collisionIndex )
//---------------------------------------------------------------------------
{
	collisionObjects[collisionIndex].active = false;
}

//---------------------------------------------------------------------------
//
// Do collision detection
void phy_doCollisionDetection()
//---------------------------------------------------------------------------
{
	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i=0; i<numManifolds; i++)
		{
			btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

			int numContacts = contactManifold->getNumContacts();

			if (numContacts > 0)
				{
					const btCollisionObject* obA = contactManifold->getBody0();
					const btCollisionObject* obB = contactManifold->getBody1();

					auto pnA = (int)obA->getUserIndex();
					auto pnB = (int)obB->getUserIndex();

//					if ((pnA > 0) || ( pnB > 0))
//						con_print(CON_INFO, true, "numContacts [ %i ] A [ %i ] B [ %i ]", numContacts, pnA, pnB );

//					if (COL_OBJECT_BULLET == collisionObjects[pnA].objectSource)
//						con_print(CON_INFO, true, "Source A Bullet [ %i ] Source B [ %i ] size [ %i ]", collisionObjects[pnA].objectSource, collisionObjects[pnB].objectSource, collisionObjects.size());

//					if (COL_OBJECT_BULLET == collisionObjects[pnB].objectSource)
//						con_print(CON_INFO, true, "Source B Bullet [ %i ] Source A [ %i ] size [ %i ]", collisionObjects[pnB].objectSource, collisionObjects[pnA].objectSource, collisionObjects.size());


					switch (collisionObjects[pnA].objectSource)
						{
							case COL_OBJECT_BSP:

								if (collisionObjects[pnB].objectSource == COL_OBJECT_BULLET)
									{
//							con_print(CON_INFO, true, "Bullet hit BSP [ %i ]", collisionObjects[pnB].objectIndex);
										par_newParticle(PARTICLE_TYPE_SPARK, gam_getBulletPosition(collisionObjects[pnB].objectIndex), collisionObjects[pnB].objectIndex);
										gam_removeBullet ( collisionObjects[pnB].objectIndex );
										return;
									}
								break;
						}
				}
		}
}


/*

enum collisiontypes {
    NOTHING         = 0,    // things that don't collide
    BALL_BODY       = 1<<2, // is ball
    TARGET_BODY     = 1<<3  // is target
};

int ballBodyCollidesWith    = TARGET_BODY | BALL_BODY;  // balls collide with targets and other balls
int targetBodyCollidesWith  = BALL_BODY;    // targets collide with balls

// ...
// bodies creation

dynamicsWorld->addRigidBody(ballBody, BALL_BODY, ballBodyCollidesWith);

dynamicsWorld->addRigidBody(targetBody, TARGET_BODY, targetBodyCollidesWith);


*/
