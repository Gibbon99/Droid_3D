#include "s_physics.h"
#include "s_physicsDebug.h"
#include "s_render.h"
#include "s_doorsBSP.h"
#include "s_camera.h"       // Need to set camera position before calling this
#include "s_assimp.h"

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
btScalar                                playerMass = 0.2f;
btScalar								playerMaxSpeed = 20.0f;

vector<_physicsObject>					physicsObjects;

//------------------------------------------------------------
//
// Translate bullet results into GLM matrix
// https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11462
glm::mat4 phy_bulletToGlmMatrix ( int index )
//------------------------------------------------------------
{
	glm::mat4 			m;
	btTransform 		trans;
	btMatrix3x3 		basis;
	
	physicsObjects[index].rigidBody->getMotionState()->getWorldTransform ( trans );
	basis = trans.getBasis();
		
	// rotation
	for ( int r = 0; r < 3; r++ )
		{
			for ( int c = 0; c < 3; c++ )
				{
					m[c][r] = basis[r][c];
				}
		}
		
	// traslation
	btVector3 origin = trans.getOrigin();
	m[3][0] = origin.getX();
	m[3][1] = origin.getY();
	m[3][2] = origin.getZ();
	
	// unit scale
	m[0][3] = 0.0f;
	m[1][3] = 0.0f;
	m[2][3] = 0.0f;
	m[3][3] = 1.0f;
	
	return m;
}

//------------------------------------------------------------
//
// Tick Callback to limit velocity of player
void limitPlayerVelocityCallback ( btDynamicsWorld *world, btScalar timeStep )
//------------------------------------------------------------
{
	btVector3 velocity = playerRigidBody->getLinearVelocity();
	btScalar speed = velocity.length();
	if ( speed > playerMaxSpeed )
		{
			velocity *= playerMaxSpeed / speed;
			playerRigidBody->setLinearVelocity ( velocity );
		}
}

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
glm::vec3 bul_returnCameraPosition()
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
//class 		bul_applyMovement;

//------------------------------------------------------------
//
// Set camera physics object
void bul_setCameraVelocity ( glm::vec3 camVelocity )
//------------------------------------------------------------
{
//	playerRigidBody->clearForces();

//	playerRigidBody->setLinearVelocity ( btVector3 ( 0.0, 0.0, 0.0 ) );

//	btTransform transform = playerRigidBody->getCenterOfMassTransform();
//	transform.setOrigin ( btVector3 ( camVelocity.x, camVelocity.y, camVelocity.z ) );

	playerRigidBody->applyCentralImpulse ( btVector3 ( camVelocity.x, camVelocity.y, camVelocity.z ) );
}

//------------------------------------------------------------
//
// Step the physics world
void bul_processPhysics ( btScalar physicsTime )
//------------------------------------------------------------
{
	dynamicsWorld->stepSimulation ( 1 / physicsTime, 10 );
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
void bul_applyMovement ( int index, float applyAmount, glm::vec3 direction )
//------------------------------------------------------------
{
	btVector3		application;

	if ( ( index < 0 ) || ( ( uint ) index > physicsObjects.size() ) )
		return;

	direction = normalize ( direction );

	application.setX ( direction.x * applyAmount );
	application.setY ( direction.y * applyAmount );
	application.setZ ( direction.z * applyAmount );

	physicsObjects[index].rigidBody->applyCentralImpulse ( application );
}

//------------------------------------------------------------
//
// Add a physics object to the world
int bul_addPhysicsObject ( int index, int whichMesh, float scaleBy, int objectType, float objectMass, glm::vec3 objectPosition )
//------------------------------------------------------------
{
	_physicsObject		tempObject;

	if ( false == physicsEngineStarted )
		{
			con_print ( CON_ERROR, true, "Physics engine is not started. Attempting to add object failed." );
			return false;
		}

	switch ( objectType )
		{
			case MODEL_TANK:
			case MODEL_FEMADROID:
			tempObject.shape = meshModels[objectType].mesh[whichMesh].physicsShapeConvexHull;
			tempObject.shape->setLocalScaling ( btVector3 ( scaleBy, scaleBy, scaleBy ) );
			break;
			
			case MODEL_CRATE:
			tempObject.shape = meshModels[objectType].mesh[whichMesh].physicsShapeConvexHull;
//			tempObject.shape = new btBoxShape ( btVector3 ( scaleBy / 2, scaleBy / 2, scaleBy / 2) );
//			tempObject.shape->setLocalScaling ( btVector3 ( scaleBy / 2, scaleBy / 2, scaleBy / 2 ) );
			tempObject.shape->setLocalScaling ( btVector3 ( scaleBy, scaleBy, scaleBy ) );
			break;

		}

	btVector3 fallInertia ( 1, 1, 1 );

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
// Set player rigid body to the same location as the camera
void bul_setPlayerPosition ( glm::vec3 position, glm::vec3 orientation )
//------------------------------------------------------------
{
	btTransform		newTransform;

	if ( false == physicsEngineStarted )
		{
			con_print ( CON_ERROR, true, "Error: Physics engine not started. Could not set player position." );
			return;
		}

	newTransform.setOrigin ( btVector3 ( position.x, position.y, position.z ) );
//	newTransform.setRotation(orientation);

	playerRigidBody->setWorldTransform ( newTransform );
	playerMotionState->setWorldTransform ( newTransform );
	/*
		void LimbBt::reposition(btVector3 position,btVector3 orientation) {
	    btTransform initialTransform;

	    initialTransform.setOrigin(position);
	    initialTransform.setRotation(orientation);

	    mBody->setWorldTransform(initialTransform);
	    mMotionState->setWorldTransform(initialTransform);
	*/

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

	//
	// Player collision object
	playerShape = new btCapsuleShape ( btScalar ( 1.5 ), btScalar ( 2.0f ) );
	playerMotionState = new btDefaultMotionState ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( 0, 0, 0 ) ) );
	btVector3 fallInertiaPlayer ( 0, 0, 0 );
	playerShape->calculateLocalInertia ( playerMass, fallInertiaPlayer );

	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI ( playerMass, playerMotionState, playerShape, fallInertiaPlayer );
	playerRigidBody = new btRigidBody ( playerRigidBodyCI );
	playerRigidBody->setActivationState ( DISABLE_DEACTIVATION );
	playerRigidBody->setDamping ( 0.69f, 0.9f );

	//
	// Prevent movement in Y direction ( gravity doesn't work )
	//
	//playerRigidBody->setLinearFactor(btVector3(1,0,1));
	//playerRigidBody->setAngularFactor(btVector3(0,1,0));

	dynamicsWorld->addRigidBody ( playerRigidBody );

	dynamicsWorld->setInternalTickCallback ( limitPlayerVelocityCallback );

	//
	// Stop player collision shape from rotating
	playerRigidBody->setAngularFactor ( btVector3 ( 0.0f, 0.0f, 0.0f ) );

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
void bul_addPhysicsBSP ( float scalePhysicsBy, bool isEntity, int whichDoor, btAlignedObjectArray<btVector3>& vertices )
//------------------------------------------------------------
{
	float		bspMass = 0.0f;

	btCollisionShape*		objectShape;
	btDefaultMotionState* 	motionState;
	btRigidBody* 			rigidBody;
	btTransform 			doorTransform;

	if ( false == physicsEngineStarted )
		{
			con_print ( CON_ERROR, true, "Physics engine is not started. Attempting to add object failed." );
			return;
		}

	if ( false == isEntity )
		{
			objectShape = new btConvexHullShape ( & ( vertices[0].getX() ),vertices.size() );

			objectShape->setLocalScaling ( btVector3 ( scalePhysicsBy, scalePhysicsBy, scalePhysicsBy ) );

			btVector3 fallInertia ( 0, 0, 0 );
			objectShape->calculateLocalInertia ( bspMass, fallInertia );

			motionState = new btDefaultMotionState ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( 0.0f, 0.0f, 0.0f ) ) );
			btRigidBody::btRigidBodyConstructionInfo bspRigidBodyCI ( bspMass, motionState, objectShape, fallInertia );

			rigidBody = new btRigidBody ( bspRigidBodyCI );
			dynamicsWorld->addRigidBody ( rigidBody );
		}
	else
		{
			btScalar		doorMass = 0.6f;

			// Manage door
			doorModels[whichDoor].shape = new btConvexHullShape ( & ( vertices[0].getX() ),vertices.size() );

			doorModels[whichDoor].shape->setLocalScaling ( btVector3 ( scalePhysicsBy, scalePhysicsBy, scalePhysicsBy ) );

			btVector3 fallInertia ( 0, 0, 0 );
			doorModels[whichDoor].shape->calculateLocalInertia ( doorMass, fallInertia );

			doorModels[whichDoor].motionShape = new btDefaultMotionState ( btTransform ( btQuaternion ( 0, 0, 0, 1 ), btVector3 ( 0.0f, 0.0f, 0.0f ) ) );
			btRigidBody::btRigidBodyConstructionInfo bspRigidBodyDoor ( doorMass, doorModels[whichDoor].motionShape, doorModels[whichDoor].shape, fallInertia );

			doorModels[whichDoor].rigidBody = new btRigidBody ( bspRigidBodyDoor );

			doorModels[whichDoor].rigidBody->setGravity ( btVector3 ( 0.0f, 0.0f, 0.0f ) );

			doorModels[whichDoor].rigidBody->setAngularFactor ( btVector3 ( 1.0f, 0.0f, 0.0f ) );

			dynamicsWorld->addRigidBody ( doorModels[whichDoor].rigidBody );

			doorModels[whichDoor].motionShape->getWorldTransform ( doorTransform );
			doorModels[whichDoor].physicsStartLocation = doorTransform.getOrigin();
		}
}
