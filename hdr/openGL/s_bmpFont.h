#pragma once

// Draw a text string
void gl_texFontPrint(int whichFont, float x, float y, const char *str, ...);

// Return length of string according to font size
int gl_getFontStringWidth(int whichFont, const char *textWord);

// Start the font system - setup VAO and associated data with the VAO
bool gl_texFontInit (int fontSize, int whichFont);
