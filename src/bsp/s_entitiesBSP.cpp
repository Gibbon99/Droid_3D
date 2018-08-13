#include <hdr/bsp/s_varsBSP.h>
#include "s_loadBsp.h"
#include "s_camera.h"
#include "s_varsBSP.h"
#include "s_physics.h"
#include "s_camera3.h"

uint     g_numEntities = 0;
uint     g_numEntityKeys = 0;

//-------------------------------------------------------------------------------
//
// Setup all the entities into a string array
bool bsp_setupEntities()
//-------------------------------------------------------------------------------
{
	uint i, j;
	char tempEntityStr[256];

	//
	// Setup a string for each token line
	//
	for ( i = 0; i != strlen ( m_pEntities ); i++ )
		{
			if ( m_pEntities[i] == '{' )
				g_numEntities++;
		}

	con_print ( CON_INFO, true, "Found [ %i ] entities.", g_numEntities );
	m_pEntitiesStruct = ( tBSPEntity * ) malloc ( sizeof ( tBSPEntity ) * ( g_numEntities + 1 ) );

	if ( NULL == m_pEntitiesStruct )
		sysErrorFatal ( __FILE__, __LINE__, "Memory error: bsp_setupEntities: Malloc failed" );

	uint numOfEntities = 0;
	int charCount = 0;

	for ( i = 0; i < strlen ( m_pEntities ); i++ )
		{
			m_pEntitiesStruct[numOfEntities].value[charCount] = m_pEntities[i];
			charCount++;

			if ( charCount == MAX_ENTITY_SIZE )
				sysErrorFatal ( __FILE__, __LINE__, "Entity string too large for value buffer." );

			if ( m_pEntities[i] == '}' )
				{
					m_pEntitiesStruct[numOfEntities].value[charCount] = '\0';
					numOfEntities++;
					charCount = 0;
					m_pEntitiesStruct[numOfEntities].checked = false;
				}
		}

	if ( numOfEntities != g_numEntities )
		{
			con_print ( CON_ERROR, true, "Error; Possible corruption in BSP file. Entity numbers differ." );
		}

	//
	// Get the number of keys in each entity
	//
	int numEntityKeys = 0;

	for ( j = 0; j != numOfEntities; j++ )
		{
			for ( i = 0; i != strlen ( m_pEntitiesStruct[j].value ); i++ )
				{
					if ( m_pEntitiesStruct[j].value[i] == '{' )
						numEntityKeys++;

					if ( ( m_pEntitiesStruct[j].value[i] == '{' ) ||
					        ( m_pEntitiesStruct[j].value[i] == '}' ) ||
					        ( m_pEntitiesStruct[j].value[i] == '\n' ) )
						m_pEntitiesStruct[j].value[i] = ' ';
				}
		}

	con_print ( CON_INFO, true, "Number of keys in Entities [ %i ]", numEntityKeys );
	//
	// Now find out how much memory is need to store keys
	//
	char delimiter[] = "\"";
	char *result = NULL;
	bool useTokenValue = false;
	g_numEntityKeys = 0;

	for ( j = 0; j != numOfEntities; j++ )
		{
			//
			// strtok destroys the original string
			strcpy ( tempEntityStr, m_pEntitiesStruct[j].value );
			result = strtok ( tempEntityStr, delimiter );

			while ( result != NULL )
				{
					if ( result[0] != ' ' )
						{
							if ( false == useTokenValue )
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

					result = strtok ( NULL, delimiter );

					//
					// Just in case we get a bad file
					if ( g_numEntityKeys > 400 )
						{
							con_print ( CON_INFO, true, "Error: Endless loop getting entities." );
							break;
						}
				}
		}

	con_print ( CON_INFO, true, "numEntityKeys = [ %i ]", g_numEntityKeys );

	useTokenValue = false;
	int tokenCounter = 0;
	int tokenGroup = 0;
	//
	// Now get  memory and assign values to array
	entityList.reserve (g_numEntityKeys);

	tokenCounter = 0;

	for ( j = 0; j < numOfEntities; j++ )
		{
			strcpy ( tempEntityStr, m_pEntitiesStruct[j].value );
			result = strtok ( tempEntityStr, delimiter );

			while ( result != NULL )
				{
					if ( result[0] != ' ' )
						{
							entityList[tokenCounter].setID = tokenGroup;

							if ( false == useTokenValue )
								{
									strcpy ( entityList[tokenCounter].tokenName, result );

									if (true == verbose)
										con_print (CON_INFO, true, "[ %i ] Found [ %i ] tokenName [ %s ]", tokenCounter, entityList[tokenCounter].setID, entityList[tokenCounter].tokenName);

									useTokenValue = true;
								}
							else
								{
									strcpy ( entityList[tokenCounter].tokenValue, result );

									if (true == verbose)
										con_print (CON_INFO, true, "[ %i ] Found [ %i ] tokenValue [ %s ]", tokenCounter, entityList[tokenCounter].setID, entityList[tokenCounter].tokenValue);

									useTokenValue = false;
									tokenCounter++;
								}
						}

					result = strtok ( NULL, delimiter );
				}

			tokenGroup++;
		}

	if ( true == verbose )
		con_print ( CON_INFO, true, "Finished setting up keys into tokens" );

	if ( true == verbose )
		{
			for ( i = 0; i != g_numEntityKeys / 2; i++ )
				{
					con_print (CON_INFO, true, "Token [ %i ] Name [ %s ] Value [ %s ]", entityList[i].setID, entityList[i].tokenName, entityList[i].tokenValue);
				}
		}
	return true;
}

//-------------------------------------------------------------------------------
//
// Get number of entities matching the string
int bsp_getNumEntities ( string whichEntity )
//-------------------------------------------------------------------------------
{
	unsigned int i, entCount;

	entCount = 0;

	for ( i = 0; i != g_numEntityKeys / 2; i++ )
		{
			if ( strcmp ( entityList[i].tokenValue, whichEntity.c_str() ) == 0 )
				entCount++;
		}

	return entCount;
}

//-------------------------------------------------------------------------------
//
// Reset the searched state for each entity
void bsp_resetEntitySearchFlag()
//-------------------------------------------------------------------------------
{
	unsigned int i;

	for ( i = 0; i != g_numEntityKeys / 2; i++ )
		{
			entityList[i].checked = false;
		}
}

//-------------------------------------------------------------------------------
//
// Return the setID for the entity
int bsp_getEntitySetID ( string entityStr, bool checkAll )
//-------------------------------------------------------------------------------
{
	unsigned int i;

	for ( i = 0; i != g_numEntityKeys / 2; i++ )
		{
			if ( true == checkAll )
				{
					if ( false == entityList[i].checked )
						{
							if ( strcmp ( entityList[i].tokenValue, entityStr.c_str() ) == 0 )
								{
									entityList[i].checked = true;
									return entityList[i].setID;
								}
						}

				}

			else     // Just get the first occurance
				{
					if ( strcmp ( entityList[i].tokenValue, entityStr.c_str() ) == 0 )
						return entityList[i].setID;
				}
		}

	return -1;
}


//-------------------------------------------------------------------------------
//
// Find the location and values for a entity class - return value in entityValue
//int bsp_findEntityInfo ( string entityStr, char *entityKey, glm::vec3 *entityValue, bool swapValues, int whichSetID, int valueType )
int bsp_findEntityInfo ( string entityStr, string entityKey, glm::vec3 *entityValue, bool swapValues, int whichSetID, int valueType )
//-------------------------------------------------------------------------------
{
	unsigned int i;
	int entitySetID = 0;
	float tempSwap;

	//
	// Pass in required set to look for - only do this if required set is -1
	//
	if ( -1 == whichSetID )
		{
			entitySetID = bsp_getEntitySetID ( entityStr, false );

			if ( -1 == entitySetID )
				return -1;

		}

	else
		entitySetID = whichSetID;

	for ( i = 0; i != g_numEntityKeys / 2; i++ )
		{
			if ( strcmp ( entityList[i].tokenName, entityKey.c_str() ) == 0 )
				{
					if ( entitySetID == entityList[i].setID )
						{
							switch ( valueType )
								{
								case VAR_TYPE_FLOAT:
									sscanf ( entityList[i].tokenValue,"%f",&entityValue->x );
									return 1;
									break;

								case VAR_TYPE_VEC3:
									sscanf ( entityList[i].tokenValue,"%f %f %f",&entityValue->x, &entityValue->y, &entityValue->z );

									if ( true == swapValues )
										{
											// Swap the Z and Y values so Y is up
//											entityValue = bsp_swapValues (entityValue);
											tempSwap = entityValue->y;
											entityValue->y = entityValue->z;
											entityValue->z = tempSwap;
											entityValue->x = -entityValue->x;
										}

									return 1;
									break;

								case VAR_TYPE_INT:
									if (entityList[i].tokenValue[0] == '*')		// See if it's a model pointer or not
										entityList[i].tokenValue[0] = ' ';
									sscanf ( entityList[i].tokenValue,"%f",&entityValue->x );
									return 1;   // Need to return value greater than 0
									break;

								case VAR_TYPE_TEXT:	// NOT working
//									sscanf ( entityList[i].tokenValue, "%s", entityKey.c_str() );
//									strcpy(entityList[i].tokenValue, entityKey.c_str());
									return 1;
									break;
								}
						}
				}
		}
	return -1;
}

// ---------------------------------------------------------------------------
//
// Find the passed in entity and move the camera to it's origin
int bsp_placeCameraAtEntity ( string param1 )
// ---------------------------------------------------------------------------
// TODO (dberry#1#): Handle missing entity
{
	glm::vec3	originCoords;

	if (param1.length() < 1)
		{
			con_print ( CON_INFO, true, "Usage: findent <str>" );
			return -1;
		}

	if ( true == verbose )
		con_print ( CON_INFO, true, "Looking for [ %s ]", param1.c_str() );

	bsp_resetEntitySearchFlag();

	if ( bsp_findEntityInfo ( param1.c_str(), "origin", &originCoords, true, -1, VAR_TYPE_VEC3 ) < 0 )
		{
			con_print ( CON_INFO, true, "String [ %s ] not found in entity list.", param1.c_str() );
			return -1;
		}
	else
		{
//			if ( true == verbose )
				con_print ( CON_INFO, true, "origin [ %4.2f ] [ %4.2f ] [ %4.2f ]", originCoords.x, originCoords.y, originCoords.z );
//
// TODO: Get orientation of player_info_start
//
			// Init the camera with known values
			cam3_initCamera ( originCoords );
			bul_setPlayerPosition(originCoords, originCoords);

			return 1;
		}

	//
	// Should never get here
	return 1;
}
