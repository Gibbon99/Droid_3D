#include "s_objects.h"
#include "s_lightMaps.h"
#include "s_globals.h"
#include "s_physics.h"
#include "s_objects.h"
#include "s_assimp.h"
#include "s_physicsCollision.h"

vector<_object>		gameObject;

//----------------------------------------------------------
//
// Add a new object to the game world
//
// Need an object index for collisions in physics
//
void obj_addObject(glm::vec3 position, int meshType, bool usesPhysics, float scaleBy, glm::vec3 lightColor)
//----------------------------------------------------------
{
	_object 	tempGameObject;

	for (int whichMesh = 0; whichMesh != meshModels[meshType].numMeshes; whichMesh++)
		{
			tempGameObject.position = position;
			tempGameObject.meshType = meshType;
			tempGameObject.lightColor = lightColor;
			tempGameObject.scaleBy = scaleBy;
			tempGameObject.usesPhysics = usesPhysics;
			tempGameObject.objectID = gameObject.size();	// Returns current size - this index will be that size after insertion

			if (true == usesPhysics)
				{
					switch (meshType)
						{
							case MODEL_CRATE:
							case MODEL_TANK:
							case MODEL_FEMADROID:
								tempGameObject.collisionID = phy_addCollisionObject ( COL_OBJECT_MESH, tempGameObject.objectID );
								tempGameObject.physicsPtr = bul_addPhysicsObject ( tempGameObject.collisionID, whichMesh, scaleBy, meshType, 0.5f, position );
								break;
						}
				}
		}

	gameObject.push_back(tempGameObject);
}

//----------------------------------------------------------
//
// Render all the objects
// TODO: Do a bounding box test? Currently done in renderMesh
void obj_renderAllObjects(int whichShader)
//----------------------------------------------------------
{
	if (gameObject.size() == 0)
		return;

	for (unsigned int i = 0; i != gameObject.size(); i++)
		{
			if (true == gameObject[i].usesPhysics)
				ass_renderMeshMat4 (gameObject[i].meshType, whichShader, phy_bulletToGlmMatrix ( gameObject[i].physicsPtr ), gameObject[i].scaleBy, bsp_getAmbientColor(phy_getObjectPosition ( gameObject[i].physicsPtr )) );
			else
				ass_renderMeshVec3Position (gameObject[i].meshType, whichShader, gameObject[i].position, gameObject[i].scaleBy, bsp_getAmbientColor(gameObject[i].position) );
		}

}

//----------------------------------------------------------
//
// Add some objects to the world
void obj_addSomeObjects()
//----------------------------------------------------------
{
	obj_addObject( glm::vec3 ( -223.0, 20.0, 175.0 ), 	MODEL_FEMADROID, 	true, 8.7f, glm::vec3() );
	obj_addObject( glm::vec3 ( -192.0, 20.0, 340.0 ),   MODEL_TANK, 		true, 0.3f, glm::vec3() );

	obj_addObject( glm::vec3 ( -109.0, 32.0, 348.0 ), 	MODEL_CRATE, 		true, 5.0f, glm::vec3() );
	obj_addObject( glm::vec3 ( -100.0, 40.0, 252.0 ), 	MODEL_CRATE, 		true, 7.0f, glm::vec3() );
	obj_addObject( glm::vec3 ( -100.0, 50.0, 210.0 ), 	MODEL_CRATE, 		true, 10.0f, glm::vec3() );
}
