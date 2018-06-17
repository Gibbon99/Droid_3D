#pragma once

#include "s_globals.h"

extern bool     keyForwardDown;
extern bool     keyBackwardDown;
extern bool     keyLeftDown;
extern bool     keyRightDown;
extern bool		keyUpDown;
extern bool		keyDownDown;

extern bool		keyDoorLeftDown;
extern bool		keyDoorRightDown;

// Handle a keyboard event
void io_handleKeyboardEvent ( ALLEGRO_EVENT event );

// Read a unicode character
void io_readChar ( int character );

