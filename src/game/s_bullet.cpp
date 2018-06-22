#include <hdr/system/s_events.h>
#include <hdr/system/s_audio.h>
#include "s_particles.h"
#include "s_globals.h"
#include "s_assimp.h"
#include "s_shaders.h"
#include "s_collideBSP.h"
#include "s_bullet.h"
#include "s_shaderLights.h"
#include "s_physicsCollision.h"

_bullet             bullet[MAX_NUM_BULLETS];

float               bullet_1_speed;
int                 numActiveBullets = 0;

//----------------------------------------------------------------
//
// Draw the bullet model at the passed in position
void gam_drawBullet ( unsigned int whichBullet, glm::mat4 physicsMatrix, int whichShader )
//----------------------------------------------------------------
{
	ass_renderMeshMat4 ( MODEL_CRATE, whichShader, physicsMatrix, bullet[whichBullet].meshScaleFactor, glm::vec3 ( 1.0f, 0.2f, 0.4f ) );
}

//----------------------------------------------------------------
//
// Draw bullet model
void gam_drawBullets ( int whichShader )
//----------------------------------------------------------------
{
	for ( int i = 0; i != MAX_NUM_BULLETS; i++ )
		{
			if ( true == bullet[i].active )
				{
					gam_drawBullet ( i, phy_bulletToGlmMatrix(bullet[i].physicsIndex), whichShader );
				}
		}
}

//----------------------------------------------------------------
//
// Process a bullets movement
void gam_processBulletMovement ( GLfloat interpolate )
//----------------------------------------------------------------
{
	for ( int i = 0; i != MAX_NUM_BULLETS; i++ )
		{
			if ( true == bullet[i].active )
				{
					if ( bullet[i].lightIndex != -1 )
						allLights[bullet[i].lightIndex].position = phy_getObjectPosition(bullet[i].physicsIndex);
				}
		}
}

//----------------------------------------------------------------
//
// Create new bullet
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
					bullet[i].lightIndex = bsp_addNewLight ( glm::vec3 ( 55.0f, 10.0f, 5.0f ), LIGHT_POINT, LIGHT_POINT );
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
	con_print(CON_INFO, true, "Removing bullet [ %i ]", bulletIndex);

 	evt_sendEvent(USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, 66, 45, 0, "" );

	bullet[bulletIndex].active = false;
	if (bullet[bulletIndex].particleIndex != -1)
		par_removeEmitter(bullet[bulletIndex].particleIndex);

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