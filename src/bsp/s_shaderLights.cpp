#include "s_varsBSP.h"
#include "s_entitiesBSP.h"
#include "s_shaderLights.h"
#include "s_shaders.h"
#include "s_bullet.h"

#include <vector>

vector<_myLights>   allLights;
int                 numOfLights = -1;
float               materialShininess = 5.0f;

float               globalAmbient;
float               globalGammaFactor;

uint                globalAmbientID;
uint                globalGammaFactorID;

bool                g_debugLightPos;

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
	numOfLights = bsp_getNumEntities("myLight");

	if (numOfLights == 0)
		return 0;

	//
	// Get memory to hold light information
	allLights.reserve(numOfLights + MAX_NUM_BULLETS);

	//
	// Set values to known for testing later
	for (int i = 0; i != numOfLights + MAX_NUM_BULLETS; i++)
		{
			tempLight.entitySetID = bsp_getEntitySetID("myLight", true);
			tempLight.posID = 0;
			tempLight.colorID = 0;
			tempLight.attenuationID = 0;
			tempLight.ambientCoefficientID = 0;

			allLights.push_back(tempLight);
		}

	return numOfLights;
}

//-------------------------------------------------------------------------------
//
// Populate light array with values from BSP
void bsp_setLightArrayData(int whichShader)
//-------------------------------------------------------------------------------
{
	char        tempString[MAX_STRING_SIZE];
	glm::vec3   tempVar;

	if (numOfLights == -1)
		bsp_findNumOfLights();

	if (numOfLights == 0)
		return;

	globalAmbientID =     glGetUniformLocation(shaderProgram[whichShader].programID, "globalAmbient");
	globalGammaFactorID = glGetUniformLocation(shaderProgram[whichShader].programID, "globalGammaFactor");

	for (int i = 0; i < numOfLights + MAX_NUM_BULLETS; i++)  	// Need to assign shader uniform for all possible lights
		{
			//
			// Type of light
			if (bsp_findEntityInfo("myLight", "type", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0)
				{
					con_print(CON_ERROR, true, "Error looking for light entity type value - not found. [ %i ]", i);
				}

			allLights[i].type = tempVar.x;

			//
			// Effct of light
			if (bsp_findEntityInfo("myLight", "effect", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0)
				{
					con_print(CON_ERROR, true, "Error looking for light entity effect value - not found. [ %i ]", i);
				}

			allLights[i].effect = tempVar.x;

			//
			// Get the lights position
			if (bsp_findEntityInfo("myLight", "origin", &allLights[i].position, true, allLights[i].entitySetID, VAR_TYPE_VEC3) < 0)
				{
					con_print(CON_ERROR, true, "Error looking for light entity position value - not found. [ %i ]", i);
				}

			//
			// Lights color
			if (bsp_findEntityInfo("myLight", "myColor", &allLights[i].color, false, allLights[i].entitySetID, VAR_TYPE_VEC3) < 0)
				{
					con_print(CON_ERROR, true, "Error looking for light entity color value - not found. [ %i ]", i);
				}

			//
			// Lights attenuation - brightness and reach
			if (bsp_findEntityInfo("myLight", "attenuation", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0)
				{
					con_print(CON_ERROR, true, "Error looking for light entity attenuation value - not found. [ %i ]", i);
				}

			allLights[i].attenuation = tempVar.x;

			//
			// Lights ambientcoefficient
			if (bsp_findEntityInfo("myLight", "ambientCoefficient", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0)
				{
					con_print(CON_ERROR, true, "Error looking for light entity ambientCoefficient value - not found. [ %i ]", i);
				}

			allLights[i].ambientCoefficient = tempVar.x;

			//
			// Angle of the end of the cone
			if (bsp_findEntityInfo("myLight", "coneAngle", &tempVar, false, allLights[i].entitySetID, VAR_TYPE_FLOAT) < 0)
				{
					con_print(CON_ERROR, true, "Error looking for light entity coneAngle value - not found. [ %i ]", i);
				}

			allLights[i].coneAngle = tempVar.x;

			//
			// Lights direction vector for cone to point
			if (bsp_findEntityInfo("myLight", "coneDirection", &allLights[i].coneDirection, false, allLights[i].entitySetID, VAR_TYPE_VEC3) < 0)
				{
					con_print(CON_ERROR, true, "Error looking for light entity coneDirection value - not found. [ %i ]", i);
				}

			sprintf(tempString, "allLights[%i].type", i);
			allLights[i].typeID = glGetUniformLocation(shaderProgram[whichShader].programID, tempString);

			sprintf(tempString, "allLights[%i].position", i);
			allLights[i].posID = glGetUniformLocation(shaderProgram[whichShader].programID, tempString);

			sprintf(tempString, "allLights[%i].intensities", i);
			allLights[i].colorID = glGetUniformLocation(shaderProgram[whichShader].programID, tempString);

			sprintf(tempString, "allLights[%i].attenuation", i);
			allLights[i].attenuationID = glGetUniformLocation(shaderProgram[whichShader].programID, tempString);

			sprintf(tempString, "allLights[%i].ambientCoefficient", i);
			allLights[i].ambientCoefficientID = glGetUniformLocation(shaderProgram[whichShader].programID, tempString);

			sprintf(tempString, "allLights[%i].coneAngle", i);
			allLights[i].coneAngleID = glGetUniformLocation(shaderProgram[whichShader].programID, tempString);

			sprintf(tempString, "allLights[%i].coneDirection", i);
			allLights[i].coneDirectionID = glGetUniformLocation(shaderProgram[whichShader].programID, tempString);

			switch(allLights[i].effect)
				{
					case EFFECT_NONE:
						break;

					case EFFECT_FLICKER:
						allLights[i].flickerColorOn = allLights[i].color;
						break;
				}

		}
}

//-------------------------------------------------------------------------------
//
// Process any effects that the light may have
int bsp_processLightEffect(float interpolate)
//-------------------------------------------------------------------------------
{
	for (int i = 0; i != numOfLights; i++)
		{
			switch (allLights[i].effect)
				{
					case EFFECT_NONE:
						break;

					case EFFECT_FLICKER:
						allLights[i].flickerChangeCount -= 1.0f * interpolate;

						if (allLights[i].flickerChangeCount < 0.0f)
							{
								allLights[i].flickerChangeCount = rand() % 20 + 5;
								allLights[i].flickerStateOn = !allLights[i].flickerStateOn; // Swap state

								if (true == allLights[i].flickerStateOn)
									{
										allLights[i].color = allLights[i].flickerColorOn;

									}

								else
									{
										allLights[i].color = glm::vec3(0);
									}
							}

						break;
				}
		}

}

//-------------------------------------------------------------------------------
//
// Send data from light array into the shader
void bsp_sendLightArrayToShader(int whichShader)
//-------------------------------------------------------------------------------
{
	int i = 0;

	GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID,  "numLights"), numOfLights + numActiveBullets));

	GL_CHECK(glUniform1f(globalAmbientID, globalAmbient));
	GL_CHECK(glUniform1f(globalGammaFactorID, globalGammaFactor));

	for (i = 0; i < numOfLights; i++)
		{
			GL_CHECK(glUniform1i(allLights[i].typeID, allLights[i].type));

			if (LIGHT_SPOTLIGHT == allLights[i].type)
				{
					GL_CHECK(glUniform1f(allLights[i].coneAngleID, allLights[i].coneAngle));
					GL_CHECK(glUniform3fv(allLights[i].coneDirectionID, 1, glm::value_ptr(glm::vec4(allLights[i].coneDirection, 1.0))));
				}

			GL_CHECK(glUniform4fv(allLights[i].posID,   1, glm::value_ptr(glm::vec4(allLights[i].position, 1.0))));
			GL_CHECK(glUniform3fv(allLights[i].colorID, 1, glm::value_ptr(glm::vec3(allLights[i].color))));
			GL_CHECK(glUniform1f(allLights[i].attenuationID, allLights[i].attenuation));
			GL_CHECK(glUniform1f(allLights[i].ambientCoefficientID, allLights[i].ambientCoefficient));
		}

	if (numActiveBullets > 0)
		{
			for (int j = 0; j < MAX_NUM_BULLETS; j++)
				{
					if (true == bullet[j].active)
						{
							GL_CHECK(glUniform1i(allLights[i].typeID, LIGHT_POINT));
							GL_CHECK(glUniform4fv(allLights[i].posID,   1, glm::value_ptr(glm::vec4(bullet[j].position, 1.0))));
							GL_CHECK(glUniform3fv(allLights[i].colorID, 1, glm::value_ptr(glm::vec3(200.0, 10.0, 10.0))));
							GL_CHECK(glUniform1f(allLights[i].attenuationID, 0.25f));
							GL_CHECK(glUniform1f(allLights[i].ambientCoefficientID, 10.0f));
							i++;
						}
				}
		}
}
