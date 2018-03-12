#pragma once

#include <vector>

#define LIGHT_SPOTLIGHT    0
#define LIGHT_POINT        1

#define EFFECT_NONE         0
#define EFFECT_FLICKER      1

typedef struct
{
	bool		active;
	int         type;
	int         effect;
	int         entitySetID;
	glm::vec3   position;
	glm::vec3   color;
	float       attenuation;
	float       ambientCoefficient;
	float       coneAngle;
	glm::vec3   coneDirection;
	int         typeID;
	int        	posID;
	int        	colorID;
	uint        attenuationID;
	uint        ambientCoefficientID;
	uint        coneAngleID;
	uint        coneDirectionID;

	bool        flickerStateOn;
	float       flickerChangeCount;
	glm::vec3   flickerColorOn;
} _myLights;



extern vector<_myLights> allLights;

extern int                  numOfLights;
extern float                globalAmbient;
extern float                globalGammaFactor;

extern bool                 g_debugLightPos;

extern float   materialShininess;

// Free memory used by lights - when changing level
void bsp_freeLightArray();

// Get number of light entities in the BSP
int bsp_findNumOfLights();

// Populate light array with values from BSP
void bsp_setLightArrayData();

// Process any effects that the light may have
int bsp_processLightEffect(float interpolate);

// Add a new light - usually dynamic
//
// Return pointer to new light index
int bsp_addNewLight(glm::vec3 color, int effect, int type);
