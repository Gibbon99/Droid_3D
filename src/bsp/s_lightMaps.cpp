#include <hdr/libGL/soil/SOIL.h>
#include <FreeImage.h>
#include "s_varsBSP.h"
#include "s_openGLWrap.h"

int g_lightmapShift = 5;

typedef struct
{
	glm::vec3           lightVolPosition;
	glm::vec3           lightVolAmbientColor;
} _lightVol;

vector<_lightVol>       lightVol;

float                   lightVolumes_X = 0.0f;
float                   lightVolumes_Y = 0.0f;
float                   lightVolumes_Z = 0.0f;

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
//
// GL_SRGB is here instead of GL_RGB to change gamma corrected textures back to linear color space
// TODO (dberry#1#): Put in paramter for textures/shaders not doing gamma correction
//
//    glTexImage2D ( GL_TEXTURE_2D, 0, GL_SRGB, texturesLoaded[whichTexture].width, texturesLoaded[whichTexture].height, 0, GL_RGB, GL_UNSIGNED_BYTE, texturesLoaded[whichTexture].imageData );
}

//---------------------------------------------------------------------------
//
// Work out the position for each light volume on the grid
void bsp_setupLightVolumeData()
//---------------------------------------------------------------------------
{
	// Light volumes are arranged from left to right, back to front and bottom to top
	float xPosition, yPosition, zPosition;

	lightVol.reserve(m_numOfLightVolumes);

	lightVolumes_X = ((abs(m_pModels[0].min.x) + abs(m_pModels[0].max.x)) / 64) + 1;
	lightVolumes_Y = ((abs(m_pModels[0].min.y) + abs(m_pModels[0].max.y)) / 128) + 1;
	lightVolumes_Z = ((abs(m_pModels[0].min.z) + abs(m_pModels[0].max.z)) / 64) + 1;

	con_print(CON_INFO, true, "Level dimensions Max : [ %4.2f %4.2f %4.2f ] Min : [ %4.2f %4.2f %4.2f ]",
	          m_pModels[0].max.x, m_pModels[0].max.y, m_pModels[0].max.z,
	          m_pModels[0].min.x, m_pModels[0].min.y, m_pModels[0].min.z);

	con_print(CON_INFO, true, "across X [ %2.3f ] height [ %2.2f ] depth [ %2.2f ]", lightVolumes_X, lightVolumes_Y, lightVolumes_Z);

	int counter = 0;

	xPosition = 0.0f;
	yPosition = 0.0f;
	zPosition = 0.0f;

	for (counter = 0; counter != m_numOfLightVolumes; counter++)
	{
		lightVol[counter].lightVolPosition.x = ((xPosition++ * 64.0f)  + (64.0f / 2.0f))   + (m_pModels[0].min.x - 64);
		lightVol[counter].lightVolPosition.y = ((yPosition * 128.0f) + (128.0f / 2.0f))  + m_pModels[0].min.y;
		lightVol[counter].lightVolPosition.z = ((zPosition * 64.0f)  + (64.0f / 2.0f))   + m_pModels[0].min.z;

		if (xPosition == lightVolumes_X)
		{
			xPosition = 0;
			zPosition++;
		}

		if (zPosition == lightVolumes_Z)
		{
			xPosition = 0;
			zPosition = 0;
			yPosition++;
		}

		glm::vec3 tempColor;

		tempColor = bsplightVolGamma(glm::vec3{m_pLightVols[counter].ambient[0], m_pLightVols[counter].ambient[1], m_pLightVols[counter].ambient[2]});

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
// Pass in world location - return ambient color
glm::vec3 bsp_getAmbientColor(glm::vec3 position)
//------------------------------------------------------------------------------------------
{
	float xAcross;
	float zAcross;
	float yAcross;
	float index;

	xAcross = (abs (m_pModels[0].min.x) + position.x) / 64;
	yAcross = (abs (m_pModels[0].min.y) + position.y) / 128;
	zAcross = (abs (m_pModels[0].min.z) + position.z) / 64;

	index = (((int) zAcross * lightVolumes_X) + (int) xAcross) + ((int)yAcross * (lightVolumes_X * lightVolumes_Z));

	return lightVol[index].lightVolAmbientColor;
}