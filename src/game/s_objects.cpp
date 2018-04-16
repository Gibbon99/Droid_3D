#include "s_globals.h"
#include "s_physics.h"
#include "s_objects.h"

 vector<_object>		gameObject;
 
//----------------------------------------------------------
//
// Add a new object to the game world
void obj_addObject(glm::vec3 position, int meshType, bool usesPhysics, float scaleBy, glm::vec3 lightColor)
//----------------------------------------------------------
{
	_object tempGameObject;
	
	tempGameObject.position = position;
	tempGameObject.meshType = meshType;
	tempGameObject.lightColor = lightColor;
	tempGameObject.scaleBy = scaleBy;
	tempGameObject.usesPhysics = usesPhysics;
	
	if (true == usesPhysics)
	{
		switch (meshType)
		{
			case PHYSICS_OBJECT_BOX:
			tempGameObject.physicsPtr = bul_addPhysicsObject ( -1, 1.0f, PHYSICS_OBJECT_BOX, 0.5f, position );
			break;
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
			ass_renderMesh (gameObject[i].meshType, whichShader, phy_getObjectPosition ( gameObject[i].physicsPtr ), gameObject[i].scaleBy, gameObject[i].lightColor );
		else
			ass_renderMesh (gameObject[i].meshType, whichShader, gameObject[i].position, gameObject[i].scaleBy, gameObject[i].lightColor );
	}
}


//----------------------------------------------------------
//
// Add some objects to the world
void obj_addSomeObjects()
//----------------------------------------------------------
{
	obj_addObject( glm::vec3 ( 10.0, 15.0, 50.0 ), MODEL_TANK, false, 0.3f, glm::vec3() );
	obj_addObject( glm::vec3 ( 50.0, 15.0, 100.0 ), MODEL_TANK, false, 0.3f, glm::vec3() );
	
	obj_addObject( glm::vec3 ( 100.0, 5.0, 10.0 ), MODEL_CRATE, true, 0.1f, glm::vec3() );
	obj_addObject( glm::vec3 ( 120.0, 5.0, 10.0 ), MODEL_CRATE, true, 0.2f, glm::vec3() );
	obj_addObject( glm::vec3 ( 140.0, 5.0, 10.0 ), MODEL_CRATE, true, 0.3f, glm::vec3() );
}