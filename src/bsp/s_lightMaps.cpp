#include "s_varsBSP.h"
#include "s_openGLWrap.h"
//------------------------------------------------------------
//
// This manually changes the gamma of an image
void bsp_changeGamma(byte *pImage, int size, float factor)
//------------------------------------------------------------
{
	// Go through every pixel in the lightmap
	for(int i = 0; i < size / 3; i++, pImage += 3)
		{
			float scale = 1.0f, temp = 0.0f;
			float r = 0, g = 0, b = 0;

			// extract the current RGB values
			r = (float)pImage[0];
			g = (float)pImage[1];
			b = (float)pImage[2];

			// Multiply the factor by the RGB values, while keeping it to a 255 ratio
			r = r * factor / 255.0f;
			g = g * factor / 255.0f;
			b = b * factor / 255.0f;

			// Check if the the values went past the highest value
			if(r > 1.0f && (temp = (1.0f/r)) < scale) scale=temp;

			if(g > 1.0f && (temp = (1.0f/g)) < scale) scale=temp;

			if(b > 1.0f && (temp = (1.0f/b)) < scale) scale=temp;

			// Get the scale for this pixel and multiply it by our pixel values
			scale*=255.0f;
			r*=scale;
			g*=scale;
			b*=scale;

			// Assign the new gamma'nized RGB values to our image
			pImage[0] = (byte)r;
			pImage[1] = (byte)g;
			pImage[2] = (byte)b;
		}
}

//------------------------------------------------------------
//
//	This creates a texture map from the light map image bits
void bsp_createLightmapTexture(unsigned int &texture, byte *pImageBits, int width, int height)
//------------------------------------------------------------
{
	// Generate a texture with the associative texture ID stored in the array
	GL_CHECK(glGenTextures(1, &texture));

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture arrays index and init the texture
	wrapglBindTexture(GL_TEXTURE0, texture);

	// Change the lightmap gamma values by our desired gamma
	bsp_changeGamma(pImageBits, width*height*3, g_Gamma);

	//Assign the mip map levels
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageBits));

	glBindTexture(GL_TEXTURE_2D, 0);

	con_print(CON_INFO, true, "Lightmap ID [ %i ] created.", texture);


//
// GL_SRGB is here instead of GL_RGB to change gamma corrected textures back to linear color space
// TODO (dberry#1#): Put in paramter for textures/shaders not doing gamma correction
//
//    glTexImage2D ( GL_TEXTURE_2D, 0, GL_SRGB, texturesLoaded[whichTexture].width, texturesLoaded[whichTexture].height, 0, GL_RGB, GL_UNSIGNED_BYTE, texturesLoaded[whichTexture].imageData );

}
