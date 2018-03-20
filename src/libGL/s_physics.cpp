#include "s_physics.h"
#include "s_physicsDebug.h"
#include "s_render.h"

#include "s_camera.h"       // Need to set camera position before calling this

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

GLDebugDrawer                           debugDrawer;

vector<_physicsObject>					physicsObjects;


//------------------------------------------------------------
//
// Get position for physics object by index
glm::vec3 phy_getObjectPosition ( int index )
//------------------------------------------------------------
{
	btTransform trans;

	physicsObjects[index].rigidBody->getMotionState()->getWorldTransform ( trans );

	return glm::vec3 ( trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ() );
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

	playerRigidBody->setLinearVelocity ( btVector3 ( 0.0, 0.0, 0.0 ) );

	btVector3 tempPos = playerRigidBody->getCenterOfMassPosition();

	btTransform transform = playerRigidBody->getCenterOfMassTransform();
	transform.setOrigin ( btVector3 ( camVelocity.x, camVelocity.y, camVelocity.z ) );

	playerRigidBody->applyCentralImpulse ( btVector3 ( camVelocity.x, camVelocity.y, camVelocity.z ) );
}

/*
//------------------------------------------------------------
//
// Draw the falling sphere with a cube
void bul_showFallingObject(int whichShader)
//------------------------------------------------------------
{
	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform ( trans );

	ass_renderMesh ( MODEL_CRATE, whichShader, glm::vec3 ( trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ() ), 0.1f, glm::vec3() );
}
*/

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
// Apply a force to a physics object
void phy_applyMovement ( int index, float applyAmount, glm::vec3 direction )
//------------------------------------------------------------
{
	btVector3		application;

	if ((index < 0) || (index > physicsObjects.size()))
		return;

	direction = normalize(direction);

	application.setX ( direction.x * applyAmount );
	application.setY ( direction.y * applyAmount );
	application.setZ ( direction.z * applyAmount );

	physicsObjects[index].rigidBody->applyCentralForce ( application );
}

//------------------------------------------------------------
//
// Add a physics object to the world
int bul_addPhysicsObject ( int index, int objectSize, int objectType, float objectMass, glm::vec3 objectPosition )
//------------------------------------------------------------
{
	btCollisionShape*   objectShape;
	_physicsObject		tempObject;

	if ( false == physicsEngineStarted )
		{
			con_print ( true, true, "Physics engine is not started. Attempting to add object failed." );
			return false;
		}

	switch ( objectType )
		{
		case PHYSICS_OBJECT_BOX:
			tempObject.shape = new btBoxShape ( btVector3 ( objectSize / 2, objectSize / 2, objectSize / 2 ) );
			break;

		}

	btVector3 fallInertia ( 0, 0, 0 );
	float scalePhysicsBy = 1.0;

	tempObject.shape->setLocalScaling ( btVector3 ( scalePhysicsBy, scalePhysicsBy, scalePhysicsBy ) );

	tempObject.shape->calculateLocalInertia ( objectMass, fallInertia );
	tempObject.motionState = new btDefaultMotionState ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( objectPosition.x, objectPosition.y, objectPosition.z ) ) );
	tempObject.rigidBody = new btRigidBody ( objectMass, tempObject.motionState, tempObject.shape, fallInertia );

	//
	// Need more work on this
	tempObject.rigidBody->setUserPointer ( ( void * ) index );

	dynamicsWorld->addRigidBody ( tempObject.rigidBody );

	physicsObjects.push_back ( tempObject );

	return physicsObjects.size() - 1;	// Return index into array
}

//------------------------------------------------------------
//
// Start Physics system
bool bul_startPhysics()
//------------------------------------------------------------
{

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


	playerShape = new btCapsuleShape ( btScalar ( 0.5 ), btScalar ( 1.0f ) );
	playerMotionState = new btDefaultMotionState ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( 0, 0, 0 ) ) );
	btVector3 fallInertiaPlayer ( 0, 0, 0 );
	playerShape->calculateLocalInertia ( playerMass, fallInertiaPlayer );


	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI ( playerMass, playerMotionState, playerShape, fallInertiaPlayer );
	playerRigidBody = new btRigidBody ( playerRigidBodyCI );
//	playerRigidBody->setActivationState ( DISABLE_DEACTIVATION );
	dynamicsWorld->addRigidBody ( playerRigidBody );
	//
	// Stop player collision shape from rotating
	playerRigidBody->setAngularFactor ( btVector3 ( 0.0, 0.0, 0.0 ) );


	//
	// Point to debug renderer
	dynamicsWorld->setDebugDrawer ( &debugDrawer );
	physicsEngineStarted = true;

	bul_enableDebug ( true );

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

//------------------------------------------------------------
//
// Add a physics BSP hull object to the world
void bul_addPhysicsBSP ( float scalePhysicsBy, btAlignedObjectArray<btVector3>& vertices )
//------------------------------------------------------------
{
	float		bspMass = 0.0f;

	btCollisionShape*   objectShape;

	btCollisionShape* 		shape;
	btDefaultMotionState* 	motionState;
	btRigidBody* 			rigidBody;


	if ( false == physicsEngineStarted )
		{
			con_print ( true, true, "Physics engine is not started. Attempting to add object failed." );
			return false;
		}

	objectShape= new btConvexHullShape ( & ( vertices[0].getX() ),vertices.size() );

	objectShape->setLocalScaling ( btVector3 ( scalePhysicsBy, scalePhysicsBy, scalePhysicsBy ) );

	btVector3 fallInertia ( 0, 0, 0 );
	objectShape->calculateLocalInertia ( bspMass, fallInertia );

	motionState = new btDefaultMotionState ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( 0.0f, 0.0f, 0.0f ) ) );
	btRigidBody::btRigidBodyConstructionInfo bspRigidBodyCI ( bspMass, motionState, objectShape, fallInertia );

	rigidBody = new btRigidBody ( bspRigidBodyCI );
	dynamicsWorld->addRigidBody ( rigidBody );
}
