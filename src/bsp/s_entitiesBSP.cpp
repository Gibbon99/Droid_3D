#include "s_camera.h"
#include "s_varsBSP.h"

int     g_numEntities = 0;
int     g_numEntityKeys = 0;

//-------------------------------------------------------------------------------
//
// Setup all the entities into a string array
bool bsp_setupEntities()
//-------------------------------------------------------------------------------
{

	int i, j;
	char tempEntityStr[256];

	//
	// Setup a string for each token line
	//
	for (i = 0; i != strlen(m_pEntities); i++)
		{
			if (m_pEntities[i] == '{')
				g_numEntities++;
		}

	con_print (CON_INFO, true, "Found [ %i ] entities.", g_numEntities);
	m_pEntitiesStruct = (tBSPEntity *)malloc(sizeof(tBSPEntity) * (g_numEntities + 1));

	if (NULL == m_pEntitiesStruct)
		sysErrorFatal(__FILE__, __LINE__, "Memory error: q3bspFindNumEntities: Malloc failed");

	int numOfEntities = 0;
	int charCount = 0;

	for (i = 0; i < (int)strlen(m_pEntities); i++)
		{
			m_pEntitiesStruct[numOfEntities].value[charCount] = m_pEntities[i];
			charCount++;

			if (charCount == MAX_ENTITY_SIZE)
				sysErrorFatal(__FILE__, __LINE__, "Entity string too large for value buffer.");

			if (m_pEntities[i] == '}')
				{
					m_pEntitiesStruct[numOfEntities].value[charCount] = '\0';
					numOfEntities++;
					charCount = 0;
					m_pEntitiesStruct[numOfEntities].checked = false;
				}
		}

	if (numOfEntities != g_numEntities)
		{
			con_print(CON_ERROR, true, "Error; Possible corruption in BSP file. Entity numbers differ.");
		}

	//
	// Get the number of keys in each entity
	//
	int numEntityKeys = 0;

	for (j = 0; j != numOfEntities; j++)
		{
			for (i = 0; i != strlen(m_pEntitiesStruct[j].value); i++)
				{
					if (m_pEntitiesStruct[j].value[i] == '{')
						numEntityKeys++;

					if ((m_pEntitiesStruct[j].value[i] == '{') ||
					        (m_pEntitiesStruct[j].value[i] == '}') ||
					        (m_pEntitiesStruct[j].value[i] == '\n'))
						m_pEntitiesStruct[j].value[i] = ' ';
				}
		}

	con_print(CON_INFO, true, "Number of keys in Entities [ %i ]", numEntityKeys);
	//
	// Now find out how much memory is need to store keys
	//
	char delimiter[] = "\"";
	char *result = NULL;
	bool useTokenValue = false;
	g_numEntityKeys = 0;
	char tempStr[128];

	for (j = 0; j != numOfEntities; j++)
		{
			//
			// strtok seems to destroy the original string
			strcpy(tempEntityStr, m_pEntitiesStruct[j].value);
			result = strtok( tempEntityStr, delimiter );

			while ( result != NULL )
				{
					if (result[0] != ' ')
						{
							if (false == useTokenValue)
								{
									useTokenValue = true;
									g_numEntityKeys++;

								}

							else
								{
									useTokenValue = false;
									g_numEntityKeys++;
								}
						}

					result = strtok( NULL, delimiter );

					//
					// Just in case we get a bad file
					if (g_numEntityKeys > 400)
						{
							con_print(CON_INFO, true, "Error: Endless loop getting entities.");
							break;
						}
				}
		}

	con_print (CON_INFO, true, "numEntityKeys = [ %i ]", g_numEntityKeys);

	useTokenValue = false;
	int tokenCounter = 0;
	int tokenGroup = 0;
	//
	// Now malloc memory and assign values to array
	//
	entityList = (_entity *)malloc(sizeof(_entity) * g_numEntityKeys);

	if (NULL == entityList)
		{
			sysErrorFatal(__FILE__, __LINE__, "Memory error for entityList. Exiting");
		}

	if (true == verbose)
		con_print (CON_INFO, true, "Got memory for token list.");

	tokenCounter = 0;

	for (j = 0; j < numOfEntities; j++)
		{
			strcpy(tempEntityStr, m_pEntitiesStruct[j].value);
			result = strtok( tempEntityStr, delimiter );

			while ( result != NULL )
				{
					if (result[0] != ' ')
						{
							entityList[tokenCounter].setID = tokenGroup;

							if (false == useTokenValue)
								{
									strcpy(entityList[tokenCounter].tokenName, result);

//									if (true == verbose)
//										con_print (CON_INFO, true, "[ %i ] Found [ %i ] tokenName [ %s ]", tokenCounter, entityList[tokenCounter].setID, entityList[tokenCounter].tokenName);

									useTokenValue = true;

								}

							else
								{
									strcpy(entityList[tokenCounter].tokenValue, result);

//									if (true == verbose)
//										con_print (CON_INFO, true, "[ %i ] Found [ %i ] tokenValue [ %s ]", tokenCounter, entityList[tokenCounter].setID, entityList[tokenCounter].tokenValue);

									useTokenValue = false;
									tokenCounter++;
								}
						}

					result = strtok( NULL, delimiter );
				}

			tokenGroup++;
		}

	if (true == verbose)
		con_print (CON_INFO, true, "Finished setting up keys into tokens");

	if (true == verbose)
		{
			for (i = 0; i != g_numEntityKeys / 2; i++)
				{
//					con_print (CON_INFO, true, "Token [ %i ] Name [ %s ] Value [ %s ]", entityList[i].setID, entityList[i].tokenName, entityList[i].tokenValue);
				}
		}

	return true;
}

//-------------------------------------------------------------------------------
//
// Get number of entities matching the string
int bsp_getNumEntities(char *whichEntity)
//-------------------------------------------------------------------------------
{
	int i, entCount;

	entCount = 0;

	for (i = 0; i != g_numEntityKeys / 2; i++)
		{
			if (strcmp(entityList[i].tokenValue, whichEntity) == 0)
				entCount++;
		}

	return entCount;
}
/*
Pass in entity value and key to get value for

Return value for key

Return setID

Get number of entities with string
*/

//-------------------------------------------------------------------------------
//
// Reset the searched state for each entity
void bsp_resetEntitySearchFlag()
//-------------------------------------------------------------------------------
{
	int i;

	for (i = 0; i != g_numEntityKeys / 2; i++)
		{
			entityList[i].checked = false;
		}
}

//-------------------------------------------------------------------------------
//
// Return the setID for the entity
int bsp_getEntitySetID(char *entityStr, bool checkAll)
//-------------------------------------------------------------------------------
{
	int i;

	for (i = 0; i != g_numEntityKeys / 2; i++)
		{
			if (true == checkAll)
				{
					if (false == entityList[i].checked)
						{
							if (strcmp(entityList[i].tokenValue, entityStr) == 0)
								{
									entityList[i].checked = true;
									return entityList[i].setID;
								}
						}

				}

			else     // Just get the first occurance
				{
					if (strcmp(entityList[i].tokenValue, entityStr) == 0)
						return entityList[i].setID;
				}
		}

	return -1;
}


//-------------------------------------------------------------------------------
//
// Find the location and values for a entity class - return value in entityValue
int bsp_findEntityInfo(char *entityStr, char *entityKey, glm::vec3 *entityValue, bool swapValues, int whichSetID, int valueType)
//-------------------------------------------------------------------------------
{
	int i;
	int entitySetID = 0;
	float tempSwap;

	//
	// Pass in required set to look for - only do this if required set is -1
	//
	if (-1 == whichSetID)
		{
			entitySetID = bsp_getEntitySetID(entityStr, false);

			if (-1 == entitySetID)
				return -1;

		}

	else
		entitySetID = whichSetID;

	for (i = 0; i != g_numEntityKeys / 2; i++)
		{
			if (strcmp(entityList[i].tokenName, entityKey) == 0)
				{
					if (entitySetID == entityList[i].setID)
						{
							switch (valueType)
								{
									case VAR_TYPE_FLOAT:
										sscanf(entityList[i].tokenValue,"%f",&entityValue->x);
										break;

									case VAR_TYPE_VEC3:
										sscanf(entityList[i].tokenValue,"%f %f %f",&entityValue->x, &entityValue->y, &entityValue->z);

										if (true == swapValues)
											{
												tempSwap = entityValue->y;
												entityValue->y = entityValue->z;
												entityValue->z = -tempSwap;
											}

										return 1;
										break;

									case VAR_TYPE_INT:
										sscanf(entityList[i].tokenValue,"%f",&entityValue->x);
										return 1;   // Need to return value greater than 0
										break;

//
// Need to pass string back
									/*
									                case TYPE_STRING:
									                    sscanf(entityList[i].tokenValue, "%s",modelIndex);
									                    modelIndex[0] = ' ';
									                    tempIndex = atof(modelIndex);
									                    entityValue->x = (float)tempIndex;
									                    break;
									                    */
									case VAR_TYPE_TEXT:
										sscanf(entityList[i].tokenValue, "%s", entityKey);
//                    strcpy(entityKey, modelIndex);
										return 1;
										break;
								}
						}
				}
		}
}


//-------------------------------------------------------------------------------
//
// Find the location and values for a entity class - return value in entityValue
char *bsp_findEntityInfoString(char *entityStr, char *entityKey, int whichSetID)
//-------------------------------------------------------------------------------
{
	int i;
	int entitySetID = 0;

	//
	// Pass in required set to look for - only do this if required set is -1
	//
	if (-1 == whichSetID)
		{
			entitySetID = bsp_getEntitySetID(entityStr, false);

			if (-1 == entitySetID)
				return "";

		}

	else
		entitySetID = whichSetID;

	for (i = 0; i != g_numEntityKeys / 2; i++)
		{
			if (strcmp(entityList[i].tokenName, entityKey) == 0)
				{
					if (entitySetID == entityList[i].setID)
						{
							return entityList[i].tokenValue;
						}
				}
		}

	return "Error";
}

/*

// ---------------------------------------------------------------------------
//
// Get the number of models in the level
int bsp_entityGetNumDoors()
// ---------------------------------------------------------------------------
{
	glm::vec3	originCoords;
	int			doorCount = 0;
	int i, j;

	// See if we need to free the memory or not on reload
	if (doorModels)
	{
		free(doorModels);
		doorModels = NULL;
	}

    doorCount = q3bspGetNumEntities("func_door");
    if (0 == doorCount)
        return 0;

	doorModels = (_doorModel *)malloc(sizeof(_doorModel) * (doorCount + 1));
	if (NULL == doorModels)
		sysErrorFatal(__FILE__, __LINE__, "Memory allocation failure for [ bspEntityGetNumDoors ]");

   //
    // Get all the ID's for each entity
    q3bspResetEntitySearchFlag();
    for (i = 0; i != doorCount; i++)
    {
        doorModels[i].setID = q3bspGetEntitySetID("func_door", true);
        if (true == verbose)
            logToFile("Door [ %i ] - setID [ %i ]", i, doorModels[i].setID);
    }

    char tempKey[16];

    strcpy(tempKey, "model");

    q3bspResetEntitySearchFlag();
	int index = 0;
	for (i = 0; i != doorCount; i++)
	{
		bsp_findEntityInfo("func_door", tempKey, &originCoords, doorModels[i].setID, VAR_TYPE_TEXT);
		{
		    if (true == verbose)
                con_print (CON_INFO, true, "Door [ %i ] is model index [ %s ]", index, tempKey);
			tempKey[0] = ' ';
			doorModels[index].ptrModel = atoi(tempKey);
			if (true == verbose)
                con_print (CON_INFO, true, "Door [ %i ] ptrModel [ %i ]", index, doorModels[index].ptrModel);
			index++;
			strcpy(tempKey, "model");
		}
	}

	return doorCount;
}

*/

// ---------------------------------------------------------------------------
//
// Find the passed in entity and move the camera to it's origin
int bsp_placeCameraAtEntity(char *param1)
// ---------------------------------------------------------------------------
// TODO (dberry#1#): Handle missing entity
{
	glm::vec3	originCoords;
	float    temp;

	if (strlen(param1) < 1)
		{
			con_print (CON_INFO, true, "Usage: findent <str>");
			return -1;
		}

	if (true == verbose)
		con_print (CON_INFO, true, "Looking for [ %s ]", param1);

	bsp_resetEntitySearchFlag();

	if (bsp_findEntityInfo(param1, "origin", &originCoords, true, -1, VAR_TYPE_VEC3) < 0)
		{
			con_print (CON_INFO, true, "String [ %s ] not found in entity list.", param1);
			return -1;

		}

	else
		{
// TODO (dberry#1#): Check if this needs to be swapped - do we need to swap all entities - or just models
			/*
				temp = originCoords.y;
				originCoords.y = originCoords.z;
				originCoords.z = -temp;
			*/
			if (true == verbose)
				con_print (CON_INFO, true, "origin [ %4.2f ] [ %4.2f ] [ %4.2f ]", originCoords.x, originCoords.y, originCoords.z);

			cam_positionCamera(originCoords.x, originCoords.y, originCoords.z,
			                   originCoords.x, originCoords.y, originCoords.z - 1,
			                   0.0f, 1.0f, 0.0f);

			return 1;
		}

	//
	// Should never get here
	return 1;
}
