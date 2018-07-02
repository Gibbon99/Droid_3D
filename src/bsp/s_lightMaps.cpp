#include <hdr/libGL/soil/SOIL.h>
#include <FreeImage.h>
#include "s_varsBSP.h"
#include "s_openGLWrap.h"

int g_lightmapShift = 5;

// Brighten the lightmap with component = component * 255 / 63

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

	float lightVolumes_X, lightVolumes_Y, lightVolumes_Z;

	lightVolumes_X = floor(m_pModels[0].max[0] / 64) - ceil(m_pModels[0].min[0] / 64) + 1;
	lightVolumes_Y = floor(m_pModels[0].max[1] / 64) - ceil(m_pModels[0].min[1] / 64) + 1;
	lightVolumes_Z = floor(m_pModels[0].max[2] / 128) - ceil(m_pModels[0].min[2] / 128) + 1;

	con_print(CON_INFO, true, "Level dimensions Max : [ %4.2f %4.2f %4.2f ] Min : [ %4.2f %4.2f %4.2f ]",
	          m_pModels[0].max.x, m_pModels[0].max.y, m_pModels[0].max.z,
	          m_pModels[0].min.x, m_pModels[0].min.y, m_pModels[0].min.z);

/*
	for (int j = 0; j != m_numOfLightVolumes; j++)
	{
		con_print(CON_INFO, true, "Volume [ %i ] Ambient [ %x %x %x ] Directional [ %x %x %x ] Dir [ %x %x %x ]",
		          j, m_pLightVols[j].ambient[0], m_pLightVols[j].ambient[1], m_pLightVols[j].ambient[2],
		          m_pLightVols[j].directional[0], m_pLightVols[j].directional[1], m_pLightVols[j].directional[2],
		          m_pLightVols[j].dir[0], m_pLightVols[j].dir[1], m_pLightVols[j].dir[2]);

	}
*/

	con_print(CON_INFO, true, "Dimensions [ %4.2f %4.2f %4.2f ]",
	          lightVolumes_X, lightVolumes_Y, lightVolumes_Z);
}