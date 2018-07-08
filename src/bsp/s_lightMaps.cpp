//#include <hdr/libGL/soil/SOIL.h>
//#include <FreeImage.h>
#include "s_lightMaps.h"
#include "s_varsBSP.h"
#include "s_openGLWrap.h"

int      g_lightmapShift = 5;
bool     g_debugVolLights = false;
int     lightVolIndex;
int     lightXPos;
int     lightZPos;

typedef struct
{
	glm::vec3           lightVolPosition;
	glm::vec3           lightVolAmbientColor;
} _lightVol;

vector<_lightVol>       lightVol;

int                   lightVolumes_X = 0;
int                   lightVolumes_Y = 0;
int                   lightVolumes_Z = 0;

//------------------------------------------------------------
//
// Brighten the light volume colors
glm::vec3 bsplightVolGamma(glm::vec3 inColor)
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
// Brighten the lightmap texture ( Gamme )
void bsp_lightmapGamma( byte *pImageBits, int width, int height )
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
		bsp_lightmapGamma ( pImageBits, width, height );

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
	int             counter = 0;

	lightVol.reserve((unsigned long)m_numOfLightVolumes);

	lightVolumes_X = (int)((abs(m_pModels[0].min.x) + abs(m_pModels[0].max.x)) / LIGHT_VOL_WIDTH) + 1;
	lightVolumes_Y = (int)((abs(m_pModels[0].min.y) + abs(m_pModels[0].max.y)) / LIGHT_VOL_HEIGHT) + 1;
	lightVolumes_Z = (int)((abs(m_pModels[0].min.z) + abs(m_pModels[0].max.z)) / LIGHT_VOL_DEPTH) + 1;

	con_print(CON_INFO, true, "Level dimensions Min : [ %4.2f %4.2f %4.2f ] Max : [ %4.2f %4.2f %4.2f ]",
	          m_pModels[0].min.x, m_pModels[0].min.y, m_pModels[0].min.z,
	          m_pModels[0].max.x, m_pModels[0].max.y, m_pModels[0].max.z );

	con_print(CON_INFO, true, "LightVol Across X [ %2.3f ] Height [ %2.2f ] Depth [ %2.2f ]", lightVolumes_X, lightVolumes_Y, lightVolumes_Z);

	xPosition = 0.0f;
	yPosition = 0.0f;
	zPosition = 0.0f;
	//
	// Go through the light volume array and work out the markers grid position
	for (counter = 0; counter != m_numOfLightVolumes; counter++)
	{
//		lightVol[counter].lightVolPosition.x = (m_pModels[0].min.x) - ((xPosition++ * LIGHT_VOL_WIDTH) - LIGHT_VOL_WIDTH / 2.0f);
//		lightVol[counter].lightVolPosition.y = ((yPosition * LIGHT_VOL_HEIGHT) + LIGHT_VOL_HEIGHT / 2.0f) + m_pModels[0].min.y;
//		lightVol[counter].lightVolPosition.z = (m_pModels[0].min.z) + ((zPosition * LIGHT_VOL_DEPTH) - LIGHT_VOL_DEPTH / 2.0f);

		lightVol[counter].lightVolPosition.x = -((xPosition++ * LIGHT_VOL_WIDTH) - (LIGHT_VOL_WIDTH / 2.0f));
		lightVol[counter].lightVolPosition.y = (yPosition * LIGHT_VOL_HEIGHT) + (LIGHT_VOL_HEIGHT / 2.0f);
		lightVol[counter].lightVolPosition.z = (zPosition * LIGHT_VOL_DEPTH) - (LIGHT_VOL_DEPTH / 2.0f);

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
		tempColor = bsplightVolGamma(glm::vec3{m_pLightVols[counter].ambient[0], m_pLightVols[counter].ambient[1], m_pLightVols[counter].ambient[2]});
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
// Get the ambient light color from the light volume array
// Pass in world location - return ambient color based on location in light volume grid
glm::vec3 bsp_getAmbientColor(glm::vec3 position)
//------------------------------------------------------------------------------------------
{
	float xAcross;
	float zAcross;
	float yAcross;
	float index;

	lightXPos = abs(position.x) / 64;
	lightZPos = position.z / 64;

	lightXPos++;
	lightZPos++;

	xAcross = ((abs (m_pModels[0].min.x) + position.x) / LIGHT_VOL_WIDTH);
	yAcross = ((abs (m_pModels[0].min.y) + position.y) / LIGHT_VOL_HEIGHT);
	zAcross = ((abs (m_pModels[0].min.z) + position.z) / LIGHT_VOL_DEPTH);

//	xAcross = abs (position.x) / LIGHT_VOL_WIDTH;
//	yAcross = abs (position.y) / LIGHT_VOL_HEIGHT;
//	zAcross = abs (position.z) / LIGHT_VOL_DEPTH;

	index = (lightZPos * lightVolumes_X) + lightXPos;

	lightVolIndex = (int)index;

	//index = (((int) zAcross * lightVolumes_X) + (int) xAcross) + ((int)yAcross * (lightVolumes_X * lightVolumes_Z));

//	return lightVol[(int)index + LIGHT_VOL_SLIP_COUNT].lightVolAmbientColor;
	return lightVol[(int)index].lightVolAmbientColor;
}
/*
gridx = (x - models[0].mins[0]) / 64;
gridy = (y - models[0].mins[1]) / 64;
gridz = (z - models[0].mins[2]) / 128 + 0.5f;

 I access the light volume at:

gridz * num_lightvols_y * num_lightvols_x + gridy * num_lightvols_x + gridx
 */