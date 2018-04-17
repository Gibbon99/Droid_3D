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
	for ( int i = 0; i != MAX_NUM_BULLETS; i++ )
		{
			if ( false == bullet[i].active )
				{
					numActiveBullets++;
					bullet[i].active = true;
					bullet[i].speed = speed;
					bullet[i].direction = direction; // - position;
					bullet[i].position = position;
					bullet[i].whichMesh = MODEL_CRATE;
					bullet[i].meshScaleFactor = 0.05f;
					
					bullet[i].physicsSize = ass_getModelFinalsize(bullet[i].whichMesh, bullet[i].meshScaleFactor);

con_print(CON_INFO, true, "Bullet physics size [ %3.3f %3.3f %3.3f ]", bullet[i].physicsSize.x, bullet[i].physicsSize.y, bullet[i].physicsSize.z);

					bullet[i].lightIndex = bsp_addNewLight ( glm::vec3 ( 055.0f, 0.0f, 0.0f ), LIGHT_POINT, LIGHT_POINT );
					bullet[i].physicsIndex = bul_addPhysicsObject(i, bullet[i].physicsSize, PHYSICS_OBJECT_BOX, 1.0f, position);
					
					bul_applyMovement(bullet[i].physicsIndex, bullet[i].speed, bullet[i].direction);

					return;
				}
		}
}
