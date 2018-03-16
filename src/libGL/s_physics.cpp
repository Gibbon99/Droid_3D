#include "s_physics.h"
#include "s_physicsDebug.h"
#include "s_render.h"

#include "s_camera.h"       // Need to set camera position before calling this

#define NUM_CUBES       50

float        gravityX;      // Set from startup script
float        gravityY;
float        gravityZ;

bool        physicsEngineStarted = false;

btBroadphaseInterface*                  broadphase;
btDefaultCollisionConfiguration*        collisionConfiguration;
btCollisionDispatcher*                  dispatcher;
btSequentialImpulseConstraintSolver*    solver;
btDiscreteDynamicsWorld*                dynamicsWorld;

btCollisionShape*                       playerShape;
btDefaultMotionState*                   playerMotionState;
btRigidBody*                            playerRigidBody;
btScalar                                playerMass = 1.0f;

typedef struct
{
	btRigidBody*                        fallRigidBody;
	btDefaultMotionState*               fallMotionState;
} _voxelPhysics;

vector<_voxelPhysics>                   voxelPhysics;
_voxelPhysics                           tempVoxelPhysics;

GLDebugDrawer                           debugDrawer;

//------------------------------------------------------------
//
// Get matrix for falling box
glm::mat4 bul_getMatrix ( int index )
//------------------------------------------------------------
{
	glm::mat4 ATTRIBUTE_ALIGNED16 ( worldTrans );
	btTransform trans;

	trans.setIdentity();

	voxelPhysics[index].fallRigidBody->getMotionState()->getWorldTransform ( trans );

	trans.getOpenGLMatrix ( glm::value_ptr ( worldTrans ) );

	return worldTrans;
}

//------------------------------------------------------------
//
// Return position of the camera after physics world processed
glm::vec3   bul_returnCameraPosition()
//------------------------------------------------------------
{
	glm::vec3   tempPosition;

	btTransform trans;
	playerRigidBody->getMotionState()->getWorldTransform ( trans );

	tempPosition.x = trans.getOrigin().getX();
	tempPosition.y = trans.getOrigin().getY();
	tempPosition.z = trans.getOrigin().getZ();

	return tempPosition;
}

//------------------------------------------------------------
//
// Set camera physics object
void bul_setCameraVelocity ( glm::vec3 camVelocity )
//------------------------------------------------------------
{
	playerRigidBody->clearForces();

	playerRigidBody->setLinearVelocity ( btVector3 ( camVelocity.x, camVelocity.y, camVelocity.z ) );

	btVector3 tempPos = playerRigidBody->getCenterOfMassPosition();
	printf ( "center of mass position [ %3.3f %3.3f %3.3f\n", tempPos.x(), tempPos.y(), tempPos.z() );

	btTransform transform;
	transform.setIdentity ();
	transform = playerRigidBody->getCenterOfMassTransform();
	transform.setOrigin ( btVector3 ( camPosition.x, camPosition.y, camPosition.z ) );

	playerRigidBody->setCenterOfMassTransform ( transform );

}

//------------------------------------------------------------
//
// Step the physics world
void bul_processPhysics ( btScalar physicsTime )
//------------------------------------------------------------
{
	//
	// Step world
	dynamicsWorld->stepSimulation ( 1 / physicsTime, 10 );
}

//------------------------------------------------------------
//
// Create debug line information
void bul_enableDebug ( bool performDebug )
//------------------------------------------------------------
{
	//
	// Debug world
	if ( true == performDebug )
		{
			debugDrawer.setDebugMode ( 1 );
			dynamicsWorld->debugDrawWorld();
			bul_drawDebugLines ( debugDrawer.GetLines() );

		}

	else
		{
			debugDrawer.setDebugMode ( 0 );
		}
}

//------------------------------------------------------------
//
// Set gravity to current values in gravity variables
//
// Not sure this works without recalulating inertia of objects
bool bul_setGravity()
//------------------------------------------------------------
{
	dynamicsWorld->setGravity ( btVector3 ( gravityX, gravityY, gravityZ ) );

	return true;
}

//------------------------------------------------------------
//
// Add a physics object to the world
bool bul_addPhysicsObject ( int index, int objectSize, int objectType, float objectMass, glm::vec3 objectPosition, 
		btAlignedObjectArray<btVector3>& vertices )
//------------------------------------------------------------
{
	btCollisionShape*   objectShape;

	if ( false == physicsEngineStarted )
		{
			con_print ( true, true, "Physics engine is not started. Attempting to add object failed." );
			return false;
		}

	switch ( objectType )
		{
		case PHYSICS_OBJECT_BOX:
			objectShape = new btBoxShape ( btVector3 ( objectSize / 2, objectSize / 2, objectSize / 2 ) );
			break;
			
		case PHYSICS_OBJECT_BSP:
			objectShape= new btConvexHullShape ( & ( vertices[0].getX() ),vertices.size() );
			break;
		}

	float scalePhysicsBy = 1.0;
	objectShape->setLocalScaling ( btVector3 ( scalePhysicsBy, scalePhysicsBy, scalePhysicsBy ) );

	btVector3 fallInertia ( 0, 0, 0 );
	objectShape->calculateLocalInertia ( objectMass, fallInertia );

	tempVoxelPhysics.fallMotionState = new btDefaultMotionState ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( objectPosition.x, objectPosition.y, objectPosition.z ) ) );
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI ( objectMass, tempVoxelPhysics.fallMotionState, objectShape, fallInertia );

	tempVoxelPhysics.fallRigidBody = new btRigidBody ( fallRigidBodyCI );
	tempVoxelPhysics.fallRigidBody->setUserPointer ( ( void * ) index );
	dynamicsWorld->addRigidBody ( tempVoxelPhysics.fallRigidBody );

	voxelPhysics.push_back ( tempVoxelPhysics );
}

//------------------------------------------------------------
//
// Start Physics system
bool bul_startPhysics()
//------------------------------------------------------------
{
//	m_collisionConfiguration = new btDefaultCollisionConfiguration();
//	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	//m_broadphase = new btDbvtBroadphase();
//	m_solver = new btSequentialImpulseConstraintSolver();
//	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);

//
// Not set?
//	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
//	m_dynamicsWorld->setGravity(grav);
	
	
	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher ( collisionConfiguration );

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld ( dispatcher, broadphase, solver, collisionConfiguration );
	dynamicsWorld->setGravity ( btVector3 ( gravityX, gravityY, gravityZ ) );
	//
	// Point to debug renderer
	dynamicsWorld->setDebugDrawer ( &debugDrawer );

	btVector3 fallInertia ( 0, 0, 0 );
	playerShape = new btCapsuleShape ( btScalar ( 0.5 ), btScalar ( 1.0f ) );
	playerShape->calculateLocalInertia ( playerMass, fallInertia );

	camPosition = glm::vec3 ( 144.00, 64.00, -16.00 );

	playerMotionState = new btDefaultMotionState ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( 0, 0, 0 ) ) );

	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI ( playerMass, playerMotionState, playerShape, fallInertia );
	playerRigidBody = new btRigidBody ( playerRigidBodyCI );
	playerRigidBody->setActivationState ( DISABLE_DEACTIVATION );
	dynamicsWorld->addRigidBody ( playerRigidBody );
	//
	// Stop player collision shape from rotating
	playerRigidBody->setAngularFactor ( btVector3 ( 0.0, 0.0, 0.0 ) );

	physicsEngineStarted = true;

	playerMotionState->setWorldTransform ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( camPosition.x, camPosition.y, camPosition.z ) ) );

	btTransform transform = playerRigidBody->getCenterOfMassTransform();
	transform.setOrigin ( btVector3 ( camPosition.x, camPosition.y, camPosition.z ) );
	playerRigidBody->setCenterOfMassTransform ( transform );


//BspLoader bspLoader;


	return true;
}

//------------------------------------------------------------
//
// Stop Physics system
bool bul_stopPhysics()
//------------------------------------------------------------
{
	// Free memory allocated
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	delete playerShape;
	delete playerMotionState;
	delete playerRigidBody;

	return true;
}
