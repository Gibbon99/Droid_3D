#pragma once

#include "s_openGL.h"

typedef struct
{
	float advance;
} _glyph;
// TODO (dberry#1#): Make 256 char count a define

typedef struct
{
	_glyph        glyph[256];
	int           texID;
	int		    texWidth;
	int		    texHeight;
	float         size;
	float         percentOffset;
	_dataObject   glFont[256];
} _font;

extern _font          font[];
extern float          fontSizeSmall;
extern float          fontSizeMedium;
extern float          fontSizeLarge;
extern _glColor       fontColor;



// Update the vertex size for the font size
void gl_texFontUpdateVerts(int fontSize);
