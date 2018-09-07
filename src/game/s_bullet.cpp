#include "s_globals.h"
#include "s_particles.h"
#include "s_assimp.h"
#include "s_bullet.h"
#include "s_shaderLights.h"
#include "s_physicsCollision.h"
#include "s_lightMaps.h"

_bullet             bullet[MAX_NUM_BULLETS];

float               bullet_1_speed;
int                 numActiveBullets = 0;

//----------------------------------------------------------------
//
// Draw the bullet model at the passed in position
void gam_drawBullet ( unsigned int whichBullet, const glm::mat4 &physicsMatrix, int whichShader )
//----------------------------------------------------------------
{
	ass_renderMeshMat4 ( MODEL_CRATE, whichShader, physicsMatrix, bullet[whichBullet].meshScaleFactor, bsp_getAmbientColor(gam_getBulletPosition(whichBullet) ));
}

//----------------------------------------------------------------
//
// Draw bullet model
void gam_drawBullets ( int whichShader )
//----------------------------------------------------------------
{
	for ( unsigned int i = 0; i != MAX_NUM_BULLETS; i++ )
		{
			if ( bullet[i].active )
				{
					gam_drawBullet ( i, phy_bulletToGlmMatrix(bullet[i].physicsIndex), whichShader );
				}
		}
}

//----------------------------------------------------------------
//
// Process a bullets movement
void gam_processBulletMovement ()
//----------------------------------------------------------------
{
	for ( int i = 0; i != MAX_NUM_BULLETS; i++ )
		{
			if ( bullet[i].active )
			{
				if ( bullet[i].lightIndex != -1 )
				{
					allLights[bullet[i].lightIndex].position = phy_getObjectPosition (bullet[i].physicsIndex);
				}
			}
		}
}

//----------------------------------------------------------------
//
// Create new bullet
//
// This gets called from mouse thread - so just create the instance into the array
// and process the array from the main thread
void gam_createBullet ( glm::vec3 direction, glm::vec3 position, GLfloat speed )
//----------------------------------------------------------------
{
	for ( uint i = 0; i != MAX_NUM_BULLETS; i++ )
		{
			if ( !bullet[i].active )
				{
					numActiveBullets++;
					bullet[i].active = true;
					bullet[i].speed = speed;
					bullet[i].direction = direction;

					bullet[i].position = position + (direction * 7.0f);		// Put in front of players rigid body - need to add forward velocity as well
					bullet[i].whichMesh = MODEL_CRATE;
					bullet[i].meshScaleFactor = 2.0f;

					bullet[i].particleIndex = par_newParticle(PARTICLE_TYPE_BULLET_1, bullet[i].position, i);
					bullet[i].lightIndex = bsp_addNewLight ( glm::vec3 ( 100.0f, 0.0f, 0.0f ), LIGHT_POINT, LIGHT_POINT );

					bullet[i].collisionIndex = phy_addCollisionObject(COL_OBJECT_BULLET, i);
					bullet[i].physicsIndex = bul_addPhysicsObject(bullet[i].collisionIndex, MODEL_CRATE, bullet[i].meshScaleFactor, PHYSICS_OBJECT_BOX, 1.0f, bullet[i].position );
					
					bul_applyMovement(bullet[i].physicsIndex, bullet[i].speed, bullet[i].direction);

					return;
				}
		}
}

//----------------------------------------------------------------
//
// Remove a bullet from the World
void gam_removeBullet (int bulletIndex )
//----------------------------------------------------------------
{
	bullet[bulletIndex].active = false;

	if (bullet[bulletIndex].particleIndex != -1)
		par_removeEmitter(bullet[bulletIndex].particleIndex);

	if (bullet[bulletIndex].lightIndex != -1)
		bsp_removeLight(bullet[bulletIndex].lightIndex);

	phy_removeCollisionObject( bullet[bulletIndex].collisionIndex );
	bul_removePhysicsObject( bullet[bulletIndex].physicsIndex );

	numActiveBullets--;
}

//----------------------------------------------------------------
//
// Get the bullets current position by index
glm::vec3 gam_getBulletPosition(uint bulletIndex)
//----------------------------------------------------------------
{
	return phy_getObjectPosition(bullet[bulletIndex].physicsIndex);
}