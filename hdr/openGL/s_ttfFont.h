#pragma once


#define NUM_CHARS           128
#define START_CHAR          0
#define NUM_TTF_FONTS       3

#define NUM_VERTS           8

extern _glColor       fontColor;

// Switch to change text render method
extern int             renderText;

extern float          fontSizeSmall;
extern float          fontSizeMedium;
extern float          fontSizeLarge;

// Print a text string using TTF
void ttf_printString ( int whichFont, float startX, float startY, const char *text, ... );

// Start the TTF library
bool ttf_initLibrary ( int fontSize, int whichFont );

// Start TTF Library
void ttf_initLibrary2 ( const char *fileName, int fontSize, int whichFont );

// Display all the text in memory buffers
void ttf_displayText ( int whichFont );

// Free memory used to hold vertex information for text
void ttf_freeTextMemory();

// Setup memory to hold text information
void ttf_startText();

// Calculate the verts and textures and positions for each char in the string
void ttf_addText ( int whichFont, float startX, float startY, const char *text, ... );

// Set the font filename from a script
bool ttf_setFontName ( std::string fontFileName );

// Set the font color
void ttf_setFontColor ( float red, float green, float blue, float alpha );

// ---- testing
int ttf_returnTexID ( int whichFont );
