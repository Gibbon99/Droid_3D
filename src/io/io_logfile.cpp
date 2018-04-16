#include "s_globals.h"

//--------------------------------------------------------
//
// Logfile routines
//
// Create a text logfile to track application messages
//
//--------------------------------------------------------

static bool		fileLoggingOn;
static int		logFile;			// file handle to the logfile

//--------------------------------------------------------
// open the log file for writing
// returns TRUE/FALSE for operation
bool openLogFile ( const char *logFileName )
//--------------------------------------------------------
{
//#define APPEND_LOGFILE 1

#if defined (WIN32)

#ifdef APPEND_LOGFILE	// append logfile entrys
	logFile = _open ( logFileName, _O_WRONLY | _O_APPEND, _S_IREAD | _S_IWRITE );
#else					// or else create a new one each time
	logFile = _creat ( logFileName, _S_IWRITE );
#endif

#else

#ifdef APPEND_LOGFILE	// append logfile entrys
	logFile = open ( logFileName, O_WRONLY | O_APPEND, S_IREAD | S_IWRITE );
#else					// or else create a new one each time
	logFile = creat ( logFileName, S_IWRITE );
#endif

#endif

	if ( logFile == -1 )
		{
			// file not found - doesn't exist ??
			// try to create it
#if defined (WIN32)
			logFile = _open ( logFileName, _O_WRONLY | _O_CREAT, _S_IREAD | _S_IWRITE );
#else
			logFile = open ( logFileName, O_WRONLY | O_CREAT, S_IREAD | S_IWRITE );
#endif

			if ( logFile == -1 )	// can't even create it
				return false;	// return failure

			else
				return true;

		}

	else
		return true;
}


//--------------------------------------------------------
// Log output to file on disk
void io_logToFile ( const char *format, ... )
//--------------------------------------------------------
{
	va_list		args;
	char		logText[MAX_STRING_SIZE];
	int			byteswritten;
#if defined (WIN32)
	char		tmpTime[32];
#endif
	//
	// check if filelogging is actually enabled
	//
	if ( fileLoggingOn == false )
		return;

	//
	// check and make sure we don't overflow our string buffer
	//
	if ( strlen ( format ) >= MAX_STRING_SIZE - 1 )
		sysErrorNormal ( __FILE__, __LINE__, "String passed to logfile too long", ( MAX_STRING_SIZE - 1 ), strlen ( format ) - ( MAX_STRING_SIZE - 1 ) );

	//
	// get out the passed in parameters
	//
	va_start ( args, format );
	vsprintf ( logText, format, args );
	va_end ( args );
	//
	// get the current time and log to file
	//
#if defined (WIN32)
	_strtime ( tmpTime );
	strcat ( tmpTime, " > " );

	if ( ( byteswritten = _write ( logFile, tmpTime, strlen ( tmpTime ) ) ) == -1 )
		ErrorFatal ( __FILE__, __LINE__, "Write to logfile failed." );

#endif
	//
	// put a linefeed onto the end of the text line
	// and write it to the current line and file
	strcat ( logText, "\n" );
#if defined (WIN32)

	if ( ( byteswritten = _write ( logFile, logText, strlen ( logText ) ) ) == -1 )
#else
	if ( ( byteswritten = write ( logFile, logText, strlen ( logText ) ) ) == -1 )
#endif
		sysErrorFatal ( __FILE__, __LINE__, "Write to logfile failed." );
}


//--------------------------------------------------------
// log the current time and date to the log file
void logTimeToFile()
//--------------------------------------------------------
{
	char tmptime[128];
	char tmpdate[128];

#if defined (WIN32)
	_strtime ( tmptime );
	_strdate ( tmpdate );
#else
	strcpy ( tmptime, "Unknown" );
	strcpy ( tmpdate, "Unknown" );
#endif

	io_logToFile ( "%s\t%s", tmpdate, tmptime );
}

//--------------------------------------------------------
// start the log file - if possible
// set fileLoggingOn to true
bool io_startLogFile ( const char *logFileName )
//--------------------------------------------------------
{

	fileLoggingOn = false;

	if ( !openLogFile ( logFileName ) )
		{
			sysErrorNormal ( __FILE__, __LINE__, "Couldn't create the logfile - check file permissions or disk space." );
			fileLoggingOn = false;
			return false;

		}

	else
		{
			fileLoggingOn = true;
			io_logToFile ( "-----------------------------------------------------------------------------" );
			io_logToFile ( "Log file opened:" );
			logTimeToFile();
			io_logToFile ( "Console logfile started..." );
			return true;
		}
}



//--------------------------------------------------------
// if the log file is open - close it
void io_closeLogFile()
//--------------------------------------------------------
{
	if ( fileLoggingOn == true )
		{
			io_logToFile ( "Log file closed:" );
			logTimeToFile();
			io_logToFile ( "-----------------------------------------------------------------------------\n\n" );
#if defined (WIN32)
			_close ( logFile );
#else
			close ( logFile );
#endif
			fileLoggingOn = false;
		}
}
