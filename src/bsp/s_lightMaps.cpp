#include "s_lightMaps.h"
#include "s_varsBSP.h"
#include "s_openGLWrap.h"

int      g_lightmapShift = 5;
int     lightVolIndex;

typedef struct
{
	glm::vec3           lightVolPosition;
	glm::vec3           lightVolAmbientColor;
} _lightVol;

vector<_lightVol>       lightVol;

bool                  g_debugVolLights = false;

int                   lightXPos = 0;
int                   lightZPos = 0;
int                   lightYPos = 0;
//
// How many lightmaps positions for each dimension
int                   lightVolumes_X = 0;
int                   lightVolumes_Y = 0;
int                   lightVolumes_Z = 0;

//------------------------------------------------------------
//
// Brighten the light volume colors
glm::vec3 bsp_lightVolGamma ( glm::vec3 inColor )
//------------------------------------------------------------
{
	glm::vec3   outColor;

	float r, g, b;
	r = inColor.r;
	g = inColor.g;
	b = inColor.b;

	r *= (g_Gamma + g_lightmapShift) / 255.0f;
	g *= (g_Gamma + g_lightmapShift) / 255.0f;
	b *= (g_Gamma + g_lightmapShift) / 255.0f;

	//find the value to scale back up
	float scale=1.0f;
	float temp;
	if(r > 1.0f && (temp = (1.0f/r)) < scale) scale=temp;
	if(g > 1.0f && (temp = (1.0f/g)) < scale) scale=temp;
	if(b > 1.0f && (temp = (1.0f/b)) < scale) scale=temp;

	// scale up color values
	scale *=255.0f;
	r *= scale;
	g *= scale;
	b *= scale;

	outColor.r = r;
	outColor.g = g;
	outColor.b = b;

	return outColor;
}

//------------------------------------------------------------
//
// Brighten the lightmap texture ( Gamma )
void bsp_lightMapGamma ( byte *pImageBits, int width, int height )
//------------------------------------------------------------
{
	for(int j=0; j<width*height; ++j)
	{
		float r, g, b;
		r=pImageBits[j*3+0];
		g=pImageBits[j*3+1];
		b=pImageBits[j*3+2];

		r *= (g_Gamma + g_lightmapShift) / 255.0f;
		g *= (g_Gamma + g_lightmapShift) / 255.0f;
		b *= (g_Gamma + g_lightmapShift) / 255.0f;

		//find the value to scale back up
		float scale=1.0f;
		float temp;
		if(r > 1.0f && (temp = (1.0f/r)) < scale) scale=temp;
		if(g > 1.0f && (temp = (1.0f/g)) < scale) scale=temp;
		if(b > 1.0f && (temp = (1.0f/b)) < scale) scale=temp;

		// scale up color values
		scale*=255.0f;
		r*=scale;
		g*=scale;
		b*=scale;

		//fill data back in
		pImageBits[j*3+0]=(GLubyte)r;
		pImageBits[j*3+1]=(GLubyte)g;
		pImageBits[j*3+2]=(GLubyte)b;
	}
}

//------------------------------------------------------------
//
//	This creates a texture map from the light map image bits
void bsp_createLightmapTexture ( unsigned int &texture, byte *pImageBits, int width, int height )
//------------------------------------------------------------
{
	bsp_lightMapGamma (pImageBits, width, height);

	// Generate a texture with the associative texture ID stored in the array
	GL_CHECK (glGenTextures (1, &texture));

	// Bind the texture to the texture arrays index and init the texture
	wrapglBindTexture ( GL_TEXTURE0, texture );

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GL_ASSERT (glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageBits));

	glBindTexture (GL_TEXTURE_2D, 0);

	con_print (CON_INFO, true, "Lightmap ID [ %i ] created.", texture);
}

//---------------------------------------------------------------------------
//
// Work out the position for each light volume on the grid
void bsp_setupLightVolumeData()
//---------------------------------------------------------------------------
{
	// Light volumes are arranged from left to right, back to front and bottom to top
	float           xPosition, yPosition, zPosition;
	glm::vec3       tempColor;

	lightVol.reserve((unsigned long)m_numOfLightVolumes);

	lightVolumes_X = (int)((abs(m_pModels[0].min.x) + abs(m_pModels[0].max.x)) / LIGHT_VOL_WIDTH) + 1;
	lightVolumes_Y = (int)((abs(m_pModels[0].min.y) + abs(m_pModels[0].max.y)) / LIGHT_VOL_HEIGHT) + 1;
	lightVolumes_Z = (int)((abs(m_pModels[0].min.z) + abs(m_pModels[0].max.z)) / LIGHT_VOL_DEPTH) + 1;

	con_print(CON_INFO, true, "Level dimensions Min : [ %4.2f %4.2f %4.2f ] Max : [ %4.2f %4.2f %4.2f ]",
	          m_pModels[0].min.x, m_pModels[0].min.y, m_pModels[0].min.z,
	          m_pModels[0].max.x, m_pModels[0].max.y, m_pModels[0].max.z );

	con_print(CON_INFO, true, "LightVol Across X [ %i ] Height [ %i ] Depth [ %i ]", lightVolumes_X, lightVolumes_Y, lightVolumes_Z);

	xPosition = 0.0f;
	yPosition = 0.0f;
	zPosition = 0.0f;
	//
	// Go through the light volume array and work out the markers grid position
	for (int counter = 0; counter != m_numOfLightVolumes; counter++)
	{
		lightVol[counter].lightVolPosition.x = -((xPosition++ * LIGHT_VOL_WIDTH) - (LIGHT_VOL_WIDTH / 2.0f));
		lightVol[counter].lightVolPosition.y =   (yPosition * LIGHT_VOL_HEIGHT) + (LIGHT_VOL_HEIGHT / 2.0f);
		lightVol[counter].lightVolPosition.z =   (zPosition * LIGHT_VOL_DEPTH) - (LIGHT_VOL_DEPTH / 2.0f);

		//
		// Next row
		if (xPosition == lightVolumes_X)
		{
			xPosition = 0;
			zPosition++;
		}
		//
		// Next layer - bottom to top
		if (zPosition == lightVolumes_Z)
		{
			xPosition = 0;
			zPosition = 0;
			yPosition++;
		}
		//
		// Make the color brighter
		tempColor = bsp_lightVolGamma (glm::vec3{m_pLightVols[counter].ambient[0], m_pLightVols[counter].ambient[1], m_pLightVols[counter].ambient[2] } );
		//
		// Scale to openGL range
		lightVol[counter].lightVolAmbientColor.r = (byte)tempColor.r / 255.0f;
		lightVol[counter].lightVolAmbientColor.g = (byte)tempColor.g / 255.0f;
		lightVol[counter].lightVolAmbientColor.b = (byte)tempColor.b / 255.0f;

/*
		con_print(CON_INFO, true, "[ %i ] - lightPos [ %3.2f %3.2f %3.2f ] Ambient [ %2.3f %2.3f %2.3f ]", counter, lightVol[counter].lightVolPosition.x, lightVol[counter].lightVolPosition.y, lightVol[counter].lightVolPosition.z,
        lightVol[counter].lightVolAmbientColor.r, lightVol[counter].lightVolAmbientColor.g, lightVol[counter].lightVolAmbientColor.b);
*/
	}
}

//------------------------------------------------------------------------------------------
//
// Show the locations of the light volumes
void bsp_showLightVolPositions(int whichShader)
//------------------------------------------------------------------------------------------
{
	for (int i = 0; i != m_numOfLightVolumes; i++)
	{
		ass_renderMeshVec3Position ( MODEL_CRATE,  whichShader, lightVol[i].lightVolPosition, 5.0f, lightVol[i].lightVolAmbientColor);
	}
}

//------------------------------------------------------------------------------------------
//
// Get the index of the closest other grid position - not the current one
int bsp_getNextVolLightIndex(int nextIndex, const glm::vec3 &currentPosition)
//------------------------------------------------------------------------------------------
{
	int     lowestIndex = nextIndex;
	float   lowestDistance = 99999.0f;
	float   leftDistance, rightDistance, upDistance, downDistance;

	upDistance = glm::distance(lightVol[nextIndex + lightVolumes_X].lightVolPosition, currentPosition);
	if (upDistance < lowestDistance)
	{
		lowestDistance = upDistance;
		lowestIndex = nextIndex + lightVolumes_X;
	}

	rightDistance = glm::distance(lightVol[nextIndex + 1].lightVolPosition, currentPosition);
	if (rightDistance < lowestDistance)
	{
		lowestDistance = rightDistance;
		lowestIndex = nextIndex + 1;
	}

	leftDistance = glm::distance(lightVol[nextIndex - 1].lightVolPosition, currentPosition);
	if (leftDistance < lowestDistance)
	{
		lowestDistance = leftDistance;
		lowestIndex = nextIndex - 1;
	}

	downDistance = glm::distance(lightVol[nextIndex - lightVolumes_X].lightVolPosition, currentPosition);
	if (downDistance < lowestDistance)
	{
		lowestDistance = downDistance;
		lowestIndex = nextIndex - lightVolumes_X;
	}

	return lowestIndex;
}

//------------------------------------------------------------------------------------------
//
// Get the ambient light color from the light volume array
// Pass in world location - return ambient color based on location in light volume grid
glm::vec3 bsp_getAmbientColor(glm::vec3 position)
//------------------------------------------------------------------------------------------
{
	int  index;
	int lowestIndex;

	float closestDistance, nextDistance;
	float closestPercent, nextPercent;

	glm::vec3       newColor;

	lightXPos = abs((int)position.x) / lightVolumes_X;
	lightYPos = (int)position.y / lightVolumes_Y;
	lightZPos = (int)position.z / lightVolumes_Z;

	lightXPos++;
	lightZPos++;

	index = (lightZPos * lightVolumes_X) + lightXPos;   // TODO: Add height as well ( + (z * x) * ( y  * lightYPos ) )

	lowestIndex = bsp_getNextVolLightIndex(index, position);

	closestDistance = glm::distance(lightVol[index].lightVolPosition, position);
	nextDistance = glm::distance(lightVol[lowestIndex].lightVolPosition, position);

	closestDistance = 64 - closestDistance;
	nextDistance = 64 - nextDistance;

	if (closestDistance < 1.0f)
		closestDistance = 1.0f;
	if (nextDistance < 1.0f)
		nextDistance = 1.0f;

	closestPercent = closestDistance / LIGHT_VOL_WIDTH;
	nextPercent = nextDistance / LIGHT_VOL_WIDTH;

	newColor = (lightVol[index].lightVolAmbientColor * closestPercent) + (lightVol[lowestIndex].lightVolAmbientColor * nextPercent);

	lightVolIndex = (int)index;

	return newColor;
}
