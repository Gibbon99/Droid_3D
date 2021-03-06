#include "s_varsBSP.h"
#include "s_entitiesBSP.h"
#include "s_shaderLights.h"
#include "s_shaders.h"
#include "s_bullet.h"

#include <vector>
#include <hdr/bsp/s_loadBsp.h>

vector<_myLights>   allLights;
int                 numOfLights = -1;

bool                g_debugLightPos = true;

//-------------------------------------------------------------------------------
//
// Free memory used by lights - when changing level
void bsp_freeLightArray()
//-------------------------------------------------------------------------------
{
	allLights.clear();
	numOfLights = -1;   // Reset one time flag for new level
}

//-------------------------------------------------------------------------------
//
// Get number of light entities in the BSP
int bsp_findNumOfLights()
//-------------------------------------------------------------------------------
{
	_myLights	tempLight;

	//
	// Save value for use later on
	numOfLights = bsp_getNumEntities ( "myLight" );

	//
	// Get memory to hold light information
	allLights.reserve ( numOfLights + MAX_NUM_BULLETS );

	//
	// Set values to known for testing later
	for ( int i = 0; i != numOfLights + MAX_NUM_BULLETS; i++ )
		{
			tempLight.active = false;
			tempLight.entitySetID = bsp_getEntitySetID ( "myLight", true );
			tempLight.posID = 0;
			tempLight.colorID = 0;
			tempLight.attenuationID = 0;
			tempLight.ambientCoefficientID = 0;

			allLights.push_back ( tempLight );
		}

	return numOfLights;
}

//-------------------------------------------------------------------------------
//
// Populate light array with values from BSP
void bsp_setLightArrayData()
//-------------------------------------------------------------------------------
{
	glm::vec3   tempVar;

	if ( numOfLights == -1 )
		bsp_findNumOfLights();

	if (numOfLights > 0)
	{
//		for ( int i = 0; i < numOfLights + MAX_NUM_BULLETS; i++ )    // Need to assign shader uniform for all possible lights
		for ( int i = 0; i < numOfLights; i++ )    // Need to assign shader uniform for all possible lights
		{
			//
			// Type of light
			if ( bsp_findEntityInfo ("myLight", "type", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0 )
			{
				con_print (CON_ERROR, true, "Error looking for light entity type value - not found. [ %i ]", i);
			}
			else
			{
				allLights[i].active = true;
				allLights[i].type = (int)tempVar.x;
			}
			//
			// Effct of light
			if ( bsp_findEntityInfo ("myLight", "effect", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0 )
			{
				con_print (CON_ERROR, true, "Error looking for light entity effect value - not found. [ %i ]", i);
			}

			allLights[i].effect = (int)tempVar.x;

			//
			// Get the lights position
			if ( bsp_findEntityInfo ("myLight", "origin", &allLights[i].position, true, allLights[i].entitySetID, VAR_TYPE_VEC3) < 0 )
			{
				con_print (CON_ERROR, true, "Error looking for light entity position value - not found. [ %i ]", i);
			}
			else
			{
//				allLights[i].position = bsp_swapValues (allLights[i].position);
				con_print (CON_INFO, true, "[ %i ] Light Pos [ %3.3f %3.3f %3.3f ]", i, allLights[i].position.x, allLights[i].position.y, allLights[i].position.z);
			}

			//
			// Lights color
			if ( bsp_findEntityInfo ("myLight", "color", &allLights[i].color, false, allLights[i].entitySetID, VAR_TYPE_VEC3) < 0 )
			{
				con_print (CON_ERROR, true, "Error looking for light entity color value - not found. [ %i ]", i);
			}
			else
				con_print(CON_INFO, true, "[ %i ] light color [ %3.3f %3.3f %3.3f ]", i, allLights[i].color.r, allLights[i].color.g, allLights[i].color.b);

			//
			// Lights attenuation - brightness and reach
			if ( bsp_findEntityInfo ("myLight", "light", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0 )
			{
				con_print (CON_ERROR, true, "Error looking for light entity attenuation value - not found. [ %i ]", i);
			}
			else
			{
				allLights[i].attenuation = tempVar.x;
				con_print (CON_INFO, true, "[ %i ] Light Attenuation [ %3.3f ]", allLights[i].attenuation);
			}

			//
			// Lights ambient coefficient
			if ( bsp_findEntityInfo ("myLight", "ambientCoefficient", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0 )
			{
				con_print (CON_ERROR, true, "Error looking for light entity ambientCoefficient value - not found. [ %i ]", i);
			}

			allLights[i].ambientCoefficient = tempVar.x;

			//
			// Angle of the end of the cone
			if ( bsp_findEntityInfo ("myLight", "coneAngle", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0 )
			{
				con_print (CON_ERROR, true, "Error looking for light entity coneAngle value - not found. [ %i ]", i);
			}

			allLights[i].coneAngle = tempVar.x;

			//
			// Lights direction vector for cone to point
			if ( bsp_findEntityInfo ("myLight", "coneDirection", &allLights[i].coneDirection, false, allLights[i].entitySetID, VAR_TYPE_VEC3) < 0 )
			{
				con_print (CON_ERROR, true, "Error looking for light entity coneDirection value - not found. [ %i ]", i);
			}

			switch ( allLights[i].effect )
			{
				case EFFECT_NONE:
					break;

				case EFFECT_FLICKER:
					allLights[i].flickerColorOn = allLights[i].color;
					break;

				default:
					break;
			}

		}
	}

	for ( int i = numOfLights; i < numOfLights + MAX_NUM_BULLETS; i++ )
		{
			allLights[i].active = false;
		}
}

//-------------------------------------------------------------------------------
//
// Process any effects that the light may have
int bsp_processLightEffect ( float interpolate )
//-------------------------------------------------------------------------------
{
	for ( int i = 0; i != numOfLights; i++ )
		{
			switch ( allLights[i].effect )
				{
				case EFFECT_NONE:
					break;

				case EFFECT_FLICKER:
					allLights[i].flickerChangeCount -= 1.0f * interpolate;

					if ( allLights[i].flickerChangeCount < 0.0f )
						{
							allLights[i].flickerChangeCount = rand() % 20 + 5;
							allLights[i].flickerStateOn = !allLights[i].flickerStateOn; // Swap state

							if ( true == allLights[i].flickerStateOn )
								{
									allLights[i].color = allLights[i].flickerColorOn;

								}

							else
								{
									allLights[i].color = glm::vec3 ( 0 );
								}
						}
					break;

					default:
						break;
				}
		}
	return 0;
}

//-------------------------------------------------------------------------------
//
// Add a new light - usually dynamic
//
// Return pointer to new light index
int bsp_addNewLight ( glm::vec3 color, int effect, int type )
//-------------------------------------------------------------------------------
{
	for ( int i = numOfLights; i != numOfLights + MAX_NUM_BULLETS; i++ )
		{
			if ( false == allLights[i].active )
				{
					allLights[i].active = true;
					allLights[i].color = color;
					allLights[i].effect = LIGHT_POINT;
					allLights[i].type = LIGHT_POINT;
					allLights[i].attenuation = 0.260f;
					return i;
				}
		}
	return -1;	// No slots available
}

//-------------------------------------------------------------------------------
//
// Remove a light from the world
void bsp_removeLight( int lightIndex )
//-------------------------------------------------------------------------------
{
	allLights[lightIndex].active = false;
}