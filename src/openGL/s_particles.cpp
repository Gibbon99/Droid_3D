#include <hdr/openGL/s_shaders.h>
#include "s_openGLWrap.h"
#include "io_textures.h"
#include "s_globals.h"
#include "s_particles.h"
#include "s_shaders.h"
#include "s_camera3.h"

// Pass in position and type to start a new emitter
// Types - Smoke, spark, healing, explosion

#define NUM_PARTICLES_START 5

std::vector<_particleEmitter> particleEmitter;
unsigned int m_currVB;
unsigned int m_currTFB;
float cosTable[360];
float sinTable[360];

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
// Setup a number of particles ready to use
void par_initParticleSystem()
//----------------------------------------------------------------------------
{
	_particleEmitter    tempParticle;
	_particleMember     tempParticleMember;

	par_computeLookupTable();

	for (int i = 0; i != NUM_PARTICLES_START; i++)
	{
		tempParticle.inUse = false;
		tempParticle.position = glm::vec3{0,0,0};
		tempParticle.texID = 0;
		tempParticle.type = 0;

		for (int j = 0; j != MAX_NUM_PARTICLE_MEMBERS; j++)
		{
			tempParticleMember.position = glm::vec3{0,0,0};
			tempParticleMember.alphaValue = 0.0f;
			tempParticleMember.fadeOnDone = PARTICLE_FADE_ON;
			tempParticleMember.lifetimeLeft = 0.0f;
			tempParticleMember.velocity = glm::vec3{0,0,0};

			tempParticle.particleMember.push_back(tempParticleMember);
		}
		particleEmitter.push_back(tempParticle);
	}
}

//----------------------------------------------------------------------------
//
// Get a random position for a starting particle member
glm::vec3 par_getRandomPosition(glm::vec3 startPosition)
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

	for (uint i = 0; i != particleEmitter.size(); i++)
	{
		if (particleEmitter[i].inUse == true)
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
	for (uint i = 0; i != particleEmitter.size(); i++)
	{
		if (particleEmitter[i].inUse == true)
		{
			for ( int j = 0; j != MAX_NUM_PARTICLE_MEMBERS; j++ )
			{
				switch (particleEmitter[i].particleMember[j].fadeOnDone)
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
						if ( particleEmitter[i].particleMember[j].lifetimeLeft < 1.0f) {
							particleEmitter[i].particleMember[j].alphaValue = particleEmitter[i].particleMember[j].lifetimeLeft;
						}

						if ( particleEmitter[i].particleMember[j].lifetimeLeft < 0.0f )
						{
							particleEmitter[i].particleMember[j].lifetimeLeft = rand () % 10 + 5;
							particleEmitter[i].particleMember[j].position = particleEmitter[i].position + par_getRandomPosition (glm::vec3{0, 16, 0});
							particleEmitter[i].particleMember[j].fadeOnDone = PARTICLE_FADE_ON;
							particleEmitter[i].particleMember[j].alphaValue = 0.0f;
						}
						break;

					default:
						break;
				}
				particleEmitter[i].particleMember[j].position += particleEmitter[i].particleMember[j].velocity * (1.0f * timeDelta);
			}
		}
	}
}

//----------------------------------------------------------------------------
//
// Init the particle system
//
// Pass in what sort of particle, it's starting position, fixed or moving dependant
// on followIndex - more than 0 means attach to a moving object, otherwise fixed
void par_newParticle(uint type, const glm::vec3 &position, int followIndex)
//----------------------------------------------------------------------------
{
	switch (type)
	{
		case PARTICLE_TYPE_SMOKE:
			break;

		case PARTICLE_TYPE_HEAL:
			for (uint i = 0; i != particleEmitter.size(); i++)
			{
				if (false == particleEmitter[i].inUse)
				{
					particleEmitter[i].inUse = true;
					particleEmitter[i].position = position;
					particleEmitter[i].type = type;
					particleEmitter[i].texID = 0;
					for (int j = 0; j != MAX_NUM_PARTICLE_MEMBERS; j++)
					{
						particleEmitter[i].particleMember[j].position = particleEmitter[i].position + par_getRandomPosition(position);
						particleEmitter[i].particleMember[j].velocity = glm::vec3{0, 9.4, 0};
						particleEmitter[i].particleMember[j].lifetimeLeft = (rand() % 5) + 3;
						particleEmitter[i].particleMember[j].fadeOnDone = PARTICLE_FADE_ON;
						particleEmitter[i].particleMember[j].alphaValue = 0.0f;
					}
					return;
				}
			}
			break;

		case PARTICLE_TYPE_SPARK:
			break;

		case PARTICLE_TYPE_EXPLODE:
			break;

		default:
			break;
	}
}

//----------------------------------------------------------------------------
//
// Render a billboard texture at passed in position
void par_renderBillBoard(const uint whichEmitter)
//----------------------------------------------------------------------------
{
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
	wrapglBindTexture ( GL_TEXTURE0, texturesLoaded[TEX_FLARE].texID );
	GL_CHECK ( glUniform1i ( shaderProgram[SHADER_BILLBOARD].inTextureUnit, 0 ) );
	//
	// Bind the generated buffer
	GL_CHECK ( glBindBuffer(GL_ARRAY_BUFFER, billBoard_VB) );
	GL_CHECK ( glBufferData(GL_ARRAY_BUFFER, sizeof(_particleMember) * MAX_NUM_PARTICLE_MEMBERS, &particleEmitter[whichEmitter].particleMember[0].position, GL_DYNAMIC_DRAW) );

	GL_CHECK ( glVertexAttribPointer(shaderProgram[SHADER_BILLBOARD].inVertsID, 3, GL_FLOAT, GL_FALSE, sizeof(_particleMember), (GLvoid *)offsetof(_particleMember, position)) );   // position
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[SHADER_BILLBOARD].inVertsID ) );

	GL_CHECK ( glVertexAttribPointer(alphaValue_ID, 1, GL_FLOAT, GL_FALSE, sizeof(_particleMember), (GLvoid *)offsetof(_particleMember, alphaValue)));
	GL_CHECK ( glEnableVertexAttribArray(alphaValue_ID));

	glDrawArrays(GL_POINTS, 0, MAX_NUM_PARTICLE_MEMBERS);

	glDisableVertexAttribArray(alphaValue_ID);
	glDisableVertexAttribArray(shaderProgram[SHADER_BILLBOARD].inVertsID );
}