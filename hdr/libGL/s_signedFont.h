#pragma once

#include "s_globals.h"

#include "freetype-gl.h"
#include "vertex-buffer.h"

// Init the font system
bool sdf_initFontSystem();

// Add a string of text to the VBO
void sdf_addText(uint whichFont, glm::vec2 position, glm::vec4 lineColor, const char *text, ... );

// Draw the VBO's for each of the fonts
// Check the size of the vector to see if it has been used this frame
void sdf_displayText();


// Init the signed distance font
void sdf_printText(glm::vec2 position, glm::vec4 lineColor, const char *text, ... );

// Add information for font - called from script file
// Also checks if the fileName exists - should be called
// after the filesystem is active.
void sdf_addFontInfo(uint whichFont, string fileName, uint fontSize);

// Get the width of a passed in text string using passed in font
float sdf_getTextWidth(int whichFont, const char *text, ...);

// Get the width of a passed in text string using passed in font
float sdf_getTextHeight(int whichFont);