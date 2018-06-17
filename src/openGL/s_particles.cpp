
#include <hdr/game/s_bullet.h>
#include "s_entitiesBSP.h"
#include "s_bullet.h"
#include "s_openGLWrap.h"
#include "io_textures.h"
#include "s_globals.h"
#include "s_particles.h"
#include "s_shaders.h"
#include "s_camera3.h"

// Pass in position and type to start a new emitter
// Types - Smoke, spark, healing, explosion

#define NUM_PARTICLES_START 15

std::vector<_particleEmitter> particleEmitter;
std::vector<_particleTexture> particleTexture;
float cosTable[360];
float sinTable[360];
int numOfParticles;

//----------------------------------------------------------------------------
//
// Call this function from the script to populate the texture names
void par_addParticleTexture(uint particleType, string fileName)
//----------------------------------------------------------------------------
{
	_particleTexture    tempParticleTexture;

	if (particleType > PARTICLE_TYPE_NUMBER)
	{
		con_print(CON_ERROR, true, "Attempted to add invalid particle texture type. Must be less than [ %i ]", PARTICLE_TYPE_NUMBER);
		return;
	}
	tempParticleTexture.fileName = fileName;
	tempParticleTexture.texID = utilLoadTexture ( fileName.c_str(), -1);
	if (tempParticleTexture.texID == -1)
	{
		con_print(CON_ERROR, true, "Unable to load particle texture file [ %s ]", fileName.c_str());
		return;
	}
	particleTexture.push_back(tempParticleTexture);
}

//----------------------------------------------------------------------------
//
// Precompute SIN and COS into lookup table
void par_computeLookupTable()
//----------------------------------------------------------------------------
{
	float angle = 0.0f;

	for (int i = 0; i < 360; i++)
	{
		cosTable[i] = cos(angle);
		sinTable[i] = sin(angle);
		angle++;
	}
}

//----------------------------------------------------------------------------
//
// Populate the particle system with the ones defined in the level
void par_startStaticParticles()
//----------------------------------------------------------------------------
{
	for ( int i = 0; i != numOfParticles; i++ )
	{
		if ( bsp_findEntityInfo ("healing_particles", "origin", &particleEmitter[i].position, true, particleEmitter[i].entitySetID,
		                         VAR_TYPE_VEC3) < 0 )
		{
			con_print (CON_ERROR, true, "Error looking for particle position value - not found. [ %i ]", i);
		}
		par_newParticle(PARTICLE_TYPE_HEAL, particleEmitter[i].position, 0);
	}
}


//----------------------------------------------------------------------------
//
// Setup a number of particles ready to use
void par_initParticleSystem()
//----------------------------------------------------------------------------
{
	_particleEmitter    tempParticle;
	_particleMember     tempParticleMember;

	par_computeLookupTable();

	numOfParticles = bsp_getNumEntities ( "healing_particles" );
	//
	// check numofParticles against NUM_PARTICLES_START
	//

	for (int i = 0; i != NUM_PARTICLES_START; i++)
	{
		tempParticle.inUse = false;
		tempParticle.position = glm::vec3{0,0,0};
		tempParticle.type = 0;
		tempParticle.entitySetID =  bsp_getEntitySetID ( "healing_particles", true );

		for (int j = 0; j != MAX_NUM_PARTICLE_MEMBERS; j++)
		{
			tempParticleMember.position = glm::vec3{0,0,0};
			tempParticleMember.alphaValue = 0.0f;
			tempParticleMember.fadeOnDone = PARTICLE_FADE_ON;
			tempParticleMember.lifetimeLeft = 0.0f;
			tempParticleMember.velocity = glm::vec3{0,0,0};
			tempParticleMember.sizeValue = 3.0f;

			tempParticle.particleMember.push_back(tempParticleMember);
		}
		particleEmitter.push_back(tempParticle);
	}
	//
	// Load the textures for each particle type
	util_executeScriptFunction ( "scr_loadParticleTextures", "" );
	par_startStaticParticles();
}

//----------------------------------------------------------------------------
//
// Get random vector number
float par_getRandomVectorNumber(float compareDirection)
//----------------------------------------------------------------------------
{
	float newDirection;

	newDirection = rand() %1000;
	newDirection = newDirection / 1000.0f;

	if (compareDirection > 0.0)
		newDirection = -newDirection;

	return newDirection;
}

//----------------------------------------------------------------------------
//
// Get a random position for a new particle in bullet_1
glm::vec3 par_getRandomPositionBullet_1 (const glm::vec3 startPosition)
//----------------------------------------------------------------------------
{
	float posX, posY;

	float radius = 2.0f;    // TODO: Make configurable from script
	int angle;

	angle = rand() % 360;
	posY = sinTable[angle] * radius;
	posX = cosTable[angle] * radius;

	return glm::vec3{posX, posY, 0};
}

//----------------------------------------------------------------------------
//
// Get a random position for a starting particle member
glm::vec3 par_getRandomPositionHeal ( const glm::vec3 startPosition )
//----------------------------------------------------------------------------
{
	float posX, posY, posZ;

	float radius = 10.0f;
	int angle;

	angle = rand() % 360;   // TODO: Make a random lookup table filled with random numbers?

	posX = cosTable[angle] * radius;
	posZ = sinTable[angle] * radius;

	posY = rand() % 100;
	posY = posY / 100;
	posY = posY * 15.0f;

	return glm::vec3{posX, posY, posZ};
}

//----------------------------------------------------------------------------
//
// Render the particles
void par_renderParticles()
//----------------------------------------------------------------------------
{
	wrapglEnable ( GL_BLEND );
	GL_CHECK ( glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
//	GL_CHECK ( glBlendFunc ( GL_ONE, GL_ONE_MINUS_SRC_ALPHA ) );

	for (uint i = 0; i != particleEmitter.size(); i++)
	{
		if ( particleEmitter[i].inUse == true )
			par_renderBillBoard (i);
	}

	wrapglDisable( GL_BLEND );
}
//----------------------------------------------------------------------------
//
// Animate the particles
void par_processParticles(float timeDelta)
//----------------------------------------------------------------------------
{
	for ( uint i = 0; i != particleEmitter.size (); i++ )
	{
		if ( particleEmitter[i].inUse )
		{
			switch ( particleEmitter[i].type )
			{
				case PARTICLE_TYPE_HEAL:
					for ( int j = 0; j != MAX_NUM_PARTICLE_MEMBERS; j++ )
					{
						particleEmitter[i].particleMember[j].position +=
								particleEmitter[i].particleMember[j].velocity * (1.0f * timeDelta);

						switch ( particleEmitter[i].particleMember[j].fadeOnDone )
						{
							case PARTICLE_FADE_ON:
								particleEmitter[i].particleMember[j].alphaValue += (1.0f * timeDelta);
								if ( particleEmitter[i].particleMember[j].alphaValue > 1.0f )
								{
									particleEmitter[i].particleMember[j].alphaValue = 1.0f;
									particleEmitter[i].particleMember[j].fadeOnDone = PARTICLE_FADE_DONE;
								}
								break;

							case PARTICLE_FADE_DONE:
								particleEmitter[i].particleMember[j].lifetimeLeft -= (1.0f * timeDelta);
								if ( particleEmitter[i].particleMember[j].lifetimeLeft < 1.0f )
								{
									particleEmitter[i].particleMember[j].alphaValue = particleEmitter[i].particleMember[j].lifetimeLeft;
								}

								if ( particleEmitter[i].particleMember[j].lifetimeLeft < 0.0f )
								{
									particleEmitter[i].particleMember[j].lifetimeLeft = rand () % 10 + 5;
									particleEmitter[i].particleMember[j].position =
											particleEmitter[i].position +
											par_getRandomPositionHeal (glm::vec3{0, 16, 0});
									particleEmitter[i].particleMember[j].fadeOnDone = PARTICLE_FADE_ON;
									particleEmitter[i].particleMember[j].alphaValue = 0.0f;
									particleEmitter[i].particleMember[j].sizeValue = (rand () % 3) + 1;
								}
								break;

							default:
								break;
						}
					}
					break;

				case PARTICLE_TYPE_BULLET_1:
					particleEmitter[i].position = gam_getBulletPosition (particleEmitter[i].followIndex);

					for ( int j = 0; j != MAX_NUM_PARTICLE_MEMBERS; j++ )
					{
						if ( particleEmitter[i].particleMember[j].lifetimeLeft < 0.0f )
						{
							particleEmitter[i].spawnCounter++;
							if (particleEmitter[i].spawnCounter > 5)
							{
								particleEmitter[i].spawnCounter = 0;
								break;
							}
							particleEmitter[i].particleMember[j].lifetimeLeft = (rand () % 1) + 2;
							particleEmitter[i].particleMember[j].position = particleEmitter[i].position +
							                                                par_getRandomPositionBullet_1 (
									                                                particleEmitter[i].position);
							particleEmitter[i].particleMember[j].fadeOnDone = PARTICLE_FADE_ON;
							particleEmitter[i].particleMember[j].alphaValue = 1.0f;
							particleEmitter[i].particleMember[j].sizeValue = (rand () % 3) + 1;
						}

						particleEmitter[i].particleMember[j].lifetimeLeft -= (2.0f * timeDelta);
						particleEmitter[i].particleMember[j].position += particleEmitter[i].particleMember[j].velocity;

						if ( particleEmitter[i].particleMember[j].lifetimeLeft < 1.0f )
							particleEmitter[i].particleMember[j].alphaValue = particleEmitter[i].particleMember[j].lifetimeLeft;

					}
					break;

				case PARTICLE_TYPE_SPARK:
					for ( int j = 0; j != MAX_NUM_SPARK_MEMBERS; j++ )
					{
						if (particleEmitter[i].spawnCounter <= 0)
						{
							particleEmitter[i].inUse = false;
						}

						if (particleEmitter[i].particleMember[j].lifetimeLeft >= 0.0f)
						{
							particleEmitter[i].particleMember[j].position += particleEmitter[i].particleMember[j].velocity * (8.0f * timeDelta);

							particleEmitter[i].particleMember[j].lifetimeLeft -= (4.0f * timeDelta);
							if (particleEmitter[i].particleMember[j].lifetimeLeft  < 1.0f)
								particleEmitter[i].particleMember[j].alphaValue = particleEmitter[i].particleMember[j].lifetimeLeft;

							if (particleEmitter[i].particleMember[j].lifetimeLeft <= 0.0f)
							{
								particleEmitter[i].spawnCounter--;
							}
						}
					}
					break;

				default:
					break;
			}
		}
	}
}


//----------------------------------------------------------------------------
//
// Remove an emitter from use
void par_removeEmitter(int emitterIndex)
//----------------------------------------------------------------------------
{
	particleEmitter[emitterIndex].inUse = false;

	con_print(CON_INFO, true, "Removed particle emitter [ %i ]", emitterIndex);
}

//----------------------------------------------------------------------------
//
// Init the particle system
//
// Pass in what sort of particle, it's starting position, fixed or moving dependant
// on followIndex - more than 0 means attach to a moving object, otherwise fixed
int par_newParticle(uint type, const glm::vec3 &position, uint followIndex)
//----------------------------------------------------------------------------
{
	switch (type)
	{
		case PARTICLE_TYPE_SMOKE:
			break;

		case PARTICLE_TYPE_HEAL:
			for (uint i = 0; i != particleEmitter.size(); i++)
			{
				if ( !particleEmitter[i].inUse )
				{
					particleEmitter[i].inUse = true;
					particleEmitter[i].position = position;
					particleEmitter[i].type = type;
					for (int j = 0; j != MAX_NUM_PARTICLE_MEMBERS; j++)
					{
						particleEmitter[i].particleMember[j].position = particleEmitter[i].position +
								par_getRandomPositionHeal (position);
						particleEmitter[i].particleMember[j].velocity = glm::vec3{0, 9.4, 0};
						particleEmitter[i].particleMember[j].lifetimeLeft = (rand() % 5) + 3;
						particleEmitter[i].particleMember[j].fadeOnDone = PARTICLE_FADE_ON;
						particleEmitter[i].particleMember[j].alphaValue = 0.0f;
						particleEmitter[i].particleMember[j].sizeValue = 2.0f;
					}
					return i;
				}
			}
			break;

		case PARTICLE_TYPE_SPARK:

			for (uint i = 0; i != particleEmitter.size(); i++)
			{
				if (!particleEmitter[i].inUse)
				{
					con_print(CON_INFO, true, "Adding new spark particle [ %i ] followIndex [ %i ]", i, followIndex);
					particleEmitter[i].inUse = true;
					particleEmitter[i].position = position;
					particleEmitter[i].type = type;
					particleEmitter[i].spawnCounter = MAX_NUM_SPARK_MEMBERS;
					{
						for (int j = 0; j != MAX_NUM_SPARK_MEMBERS; j++)
						{
							particleEmitter[i].particleMember[j].velocity = bullet[followIndex].direction;
							particleEmitter[i].particleMember[j].velocity.x = par_getRandomVectorNumber(particleEmitter[i].particleMember[j].velocity.x);
							particleEmitter[i].particleMember[j].velocity.y = par_getRandomVectorNumber(particleEmitter[i].particleMember[j].velocity.y);
							particleEmitter[i].particleMember[j].velocity.z = par_getRandomVectorNumber(particleEmitter[i].particleMember[j].velocity.z);
							particleEmitter[i].particleMember[j].position = position;
							particleEmitter[i].particleMember[j].lifetimeLeft = (rand() % 2) + 1;
							particleEmitter[i].particleMember[j].fadeOnDone = PARTICLE_FADE_DONE;
							particleEmitter[i].particleMember[j].alphaValue = 1.0f;
							particleEmitter[i].particleMember[j].sizeValue = 2.0f;
						}
					}
					return i;
				}
			}
			break;

		case PARTICLE_TYPE_EXPLODE:
			break;

		case PARTICLE_TYPE_BULLET_1:
			for (uint i = 0; i != particleEmitter.size(); i++)
			{
				if ( !particleEmitter[i].inUse )
				{
					con_print(CON_INFO, true, "Adding new emitter [ %i ] - followindex [ %i ]", i, followIndex);
					particleEmitter[i].inUse = true;
					particleEmitter[i].position = position;
					particleEmitter[i].type = type;
					particleEmitter[i].followIndex = followIndex;
					particleEmitter[i].spawnCounter = 0;
					for (int j = 0; j != MAX_NUM_PARTICLE_MEMBERS; j++)
					{
						particleEmitter[i].particleMember[j].position = glm::vec3{0,0,0}; //particleEmitter[i].position + par_getRandomPositionBullet_1 (position);
						particleEmitter[i].particleMember[j].velocity = glm::vec3{0, -0.1, 0};
						particleEmitter[i].particleMember[j].lifetimeLeft = -1.0f;
						particleEmitter[i].particleMember[j].fadeOnDone = PARTICLE_FADE_ON;
						particleEmitter[i].particleMember[j].alphaValue = 1.0f;
						particleEmitter[i].particleMember[j].sizeValue = 3.0f;
					}
					return i;
				}
			}
			return -1;  // no free slots
			break;

		default:
			break;
	}
	return -1;  // Keep the compiler happy
}

//----------------------------------------------------------------------------
//
// Render a billboard texture at passed in position
void par_renderBillBoard(const uint whichEmitter)
//----------------------------------------------------------------------------
{
	static GLint sizeValue_ID = 0;
	static GLint alphaValue_ID = 0;
	static GLint viewProj_ID = 0;
	static GLint cameraPos_ID = 0;

	static bool initDone = false;
	static uint billBoard_VB = 0;
	static uint billBoard_VAO_ID = 0;
	glm::vec3   billBoardPos;

	if (false == initDone)
	{
		glUseProgram(shaderProgram[SHADER_BILLBOARD].programID);

		glGenVertexArrays (1, &billBoard_VAO_ID);

		glGenBuffers(1, &billBoard_VB);

		sizeValue_ID = glGetAttribLocation (shaderProgram[SHADER_BILLBOARD].programID, "sizeValue_in" );
		alphaValue_ID = glGetAttribLocation ( shaderProgram[SHADER_BILLBOARD].programID, "alphaValue_in" );
		viewProj_ID = glGetUniformLocation ( shaderProgram[SHADER_BILLBOARD].programID, "u_viewProjectionMat" );
		cameraPos_ID = glGetUniformLocation ( shaderProgram[SHADER_BILLBOARD].programID, "gCameraPos" );
		initDone = true;
	}
	//
	// Start using shader program
	GL_CHECK ( glUseProgram ( shaderProgram[SHADER_BILLBOARD].programID ) );

	GL_CHECK ( glBindVertexArray (billBoard_VAO_ID));

	GL_ASSERT ( glUniformMatrix4fv ( viewProj_ID, 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );
	GL_ASSERT ( glUniform3fv ( cameraPos_ID, 1, glm::value_ptr ( cam3_getCameraPosition () ) ) );
	//
	// Bind texture if it's not already bound as current texture
	wrapglBindTexture ( GL_TEXTURE0, particleTexture[particleEmitter[whichEmitter].type].texID );
	GL_CHECK ( glUniform1i ( shaderProgram[SHADER_BILLBOARD].inTextureUnit, 0 ) );
	//
	// Bind the generated buffer
	GL_CHECK ( glBindBuffer(GL_ARRAY_BUFFER, billBoard_VB) );
	GL_CHECK ( glBufferData(GL_ARRAY_BUFFER, sizeof(_particleMember) * MAX_NUM_PARTICLE_MEMBERS, &particleEmitter[whichEmitter].particleMember[0].position, GL_DYNAMIC_DRAW) );

	GL_CHECK ( glVertexAttribPointer(shaderProgram[SHADER_BILLBOARD].inVertsID, 3, GL_FLOAT, GL_FALSE, sizeof(_particleMember), (GLvoid *)offsetof(_particleMember, position)) );   // position
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_BILLBOARD].inVertsID ) );

	GL_CHECK ( glVertexAttribPointer(alphaValue_ID, 1, GL_FLOAT, GL_FALSE, sizeof(_particleMember), (GLvoid *)offsetof(_particleMember, alphaValue)));
	GL_CHECK ( glEnableVertexAttribArray(alphaValue_ID));

	GL_CHECK ( glVertexAttribPointer(sizeValue_ID, 1, GL_FLOAT, GL_FALSE, sizeof(_particleMember), (GLvoid *)offsetof(_particleMember, sizeValue)));
	GL_CHECK ( glEnableVertexAttribArray(sizeValue_ID));

	glDrawArrays(GL_POINTS, 0, MAX_NUM_PARTICLE_MEMBERS);

	glDisableVertexAttribArray(alphaValue_ID);
	glDisableVertexAttribArray(shaderProgram[SHADER_BILLBOARD].inVertsID );
}