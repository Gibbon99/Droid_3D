#include "s_globals.h"
#include "s_console.h"

//-----------------------------------------------------------------------------
//
// Execute external file
int conExecFile(char *param1)
//-----------------------------------------------------------------------------
{
	ALLEGRO_FILE		*fileHandle;
	int64_t			fileSize;
	char			*fileLocation;		// Pointer to memory to hold the configFile
	char			readLine[MAX_STRING_SIZE];	// Current line read from file
	int				j;

	if (0 == strlen(param1))
		{
			con_print (0, false, "Missing filename.");
			return -1;
		}

	// Read the file into memory
	fileHandle = al_fopen(param1, "r");

	if (NULL == fileHandle)
		sysErrorNormal(__FILE__, __LINE__, "Open error [ %s ]. [ %s ]", param1, "GET ERROR");

	fileSize = al_fsize(fileHandle);
	fileLocation = (char *)malloc(sizeof(char) * (int)fileSize);

	if (NULL == fileLocation)
		{
			sysErrorNormal(__FILE__, __LINE__, "Memory allocation failed for [ %s ]", param1);
			return -1;
		}

	strcpy(fileLocation, "");

	if (al_fread(fileHandle, (void *)fileLocation, (PHYSFS_sint32)fileSize * 1) <= 0)
		sysErrorNormal(__FILE__, __LINE__, "Read error [ %s ]. [ %s ]", param1, "GET ERROR");

	al_fclose(fileHandle);

	for (int i = 0; i != fileSize; i++)
		{
			strcpy(readLine, "");
			j = 0;

			while (fileLocation[i] != '\n')
				{
					readLine[j] = fileLocation[i];
					j++;
					i++;
				}

			readLine[--j] = '\0';
//		conPushCommand(readLine);
		}

	if (fileLocation)
		free(fileLocation);

	return 1;
}
