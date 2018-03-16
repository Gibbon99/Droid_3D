#pragma once


//-----------------------------------------------------------------------------
//
// Structure to hold image/textures to be loaded
//
//-----------------------------------------------------------------------------

typedef struct
{
	unsigned char	*imageData;			// Image Data (Up To 32 Bits)
	int	            bpp;				// Image Color Depth In Bits Per Pixel
	int	            width;				// Image Width
	int	            height;				// Image Height
	unsigned int	texID;				// Texture ID Used To Select A Texture
	unsigned int	type;				// Image Type (GL_RGB, GL_RGBA)
	bool            loaded;
} _Texture;

typedef struct
{
	char            fileName[TEXTURE_FILENAME_LENGTH];       // Filename for the image
} _textureNames;

//extern _Texture     knownTextures[];    // Textures needed for game that we know about
extern _Texture         *texturesLoaded;       // Unknown number of textures to be loaded from BSP info
extern _Texture         *modelTextures;       // Array holding all the textures from all sources
extern _textureNames    textureNames[];

enum
{
	TEX_WALL,
	TEX_HOOVER,
	TEX_ALLTILES,
	NUM_TEXTURES
};


extern int m_numOfTextures;		// The number of texture maps

// Load all the textures
bool io_loadAllTextures();

// Free memory used by texture array
void io_freeTextureArray();

// Setp the memory to hold info for textures for the models
bool io_setupModelTextureMemory ( int numberTextures );

// Load a texture and set it's TextureID
GLint utilLoadTexture ( GLuint whichTexture, const char *fileName );

// Save current screen to file
void io_saveScreenToFile();
