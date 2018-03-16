#pragma once

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined (WIN32)
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <time.h>

#include <stdarg.h>
#include <string.h>

// Log output to file on disk
void io_logToFile ( const char *format, ... );

// start the log file - if possible
// set fileLoggingOn to true
bool io_startLogFile ( const char *logFileName );

// if the log file is open - close it
void io_closeLogFile();
