#include "s_globals.h"
#include "s_assimp.h"
#include "s_shaders.h"
#include "s_collideBSP.h"
#include "s_bullet.h"
#include "s_shaderLights.h"

_bullet             bullet[MAX_NUM_BULLETS];

float               bullet_1_speed;
int                 numActiveBullets = 0;

//----------------------------------------------------------------
//
// Check if the bullet collides with the world
bool bul_checkWorldCollide ( int whichBullet )
//-----------------------------------------------------------------------------
{
#define BOX_SIZE        15

	sMoveData   md;

	glm::vec3   boxMin, boxMax;

	boxMin.x = -BOX_SIZE;
	boxMin.y = -BOX_SIZE;
	boxMin.z = -BOX_SIZE;

	boxMax.x = BOX_SIZE;
	boxMax.y = BOX_SIZE;
	boxMax.z = BOX_SIZE;

//    bsp_checkSphereMove(bullet[whichBullet].position, bullet[whichBullet].position + bullet[whichBullet].direction, BOX_SIZE, md);

	bsp_checkBoxMove ( bullet[whichBullet].position, bullet[whichBullet].position + bullet[whichBullet].direction, boxMin, boxMax, md );

	//
	// No collision
	if ( true == md.AllSolid )
		{
			return false;
		}

	//
	// Will collide, so reduce amount of movement
	if ( md.Fraction > 0 )
		{
			bullet[whichBullet].position = md.EndPoint;    // This is where we hit something
			return true;
		}

	return true;
}

//----------------------------------------------------------------
//
// Draw the bullet model at the passed in position
void gam_drawBullet ( glm::vec3 position, int whichShader )
//----------------------------------------------------------------
{
	ass_renderMesh ( MODEL_CRATE, whichShader, position, 0.05f, glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
//	ass_renderMesh(MODEL_SPHERE, whichShader, position, 40.0f, glm::vec3(1.0f, 0.0f, 0.0f));

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
					gam_drawBullet ( phy_getObjectPosition(i), whichShader );
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
					phy_applyMovement(i, 50.0f, bullet[i].direction);
					
//					bullet[i].position += ( bullet[i].direction * ( bullet[i].speed * interpolate ) );
					if ( bullet[i].lightIndex != -1 )
						allLights[bullet[i].lightIndex].position = phy_getObjectPosition(i);

					if ( false == bul_checkWorldCollide ( i ) )
						{
							allLights[bullet[i].lightIndex].active = false;
							bullet[i].lightIndex = -1;	// Just in case
							bullet[i].active = false;
							numActiveBullets--;
						}
				}
		}
}

//----------------------------------------------------------------
//
// Create new bullet
void gam_createBullet ( glm::vec3 direction, glm::vec3 position, GLfloat speed )
//----------------------------------------------------------------
{
	for ( int i = 0; i != MAX_NUM_BULLETS; i++ )
		{
			if ( false == bullet[i].active )
				{
					numActiveBullets++;
					bullet[i].active = true;
					bullet[i].speed = speed;
					bullet[i].direction = direction - position;
					bullet[i].position = position;

					bullet[i].lightIndex = bsp_addNewLight ( glm::vec3 ( 255.0f, 0.0f, 0.0f ), LIGHT_POINT, LIGHT_POINT );
					bullet[i].physicsIndex = bul_addPhysicsObject(i, 2.0f, PHYSICS_OBJECT_BOX, 0.5f, position);

					return;
				}
		}
}
