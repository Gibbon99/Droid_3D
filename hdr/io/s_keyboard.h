#pragma once

extern bool     keyForwardDown;
extern bool     keyBackwardDown;
extern bool     keyLeftDown;
extern bool     keyRightDown;
extern bool		keyUpDown;
extern bool		keyDownDown;


// Read and process the console keyboard
void io_readConsoleSpecialKeys ( int key, int action );

// Read a unicode character
void io_readCharCallback ( GLFWwindow* window, unsigned int character );

// Read and process keys for main game
void io_readGameSpecialKeys ( int key, int action );
