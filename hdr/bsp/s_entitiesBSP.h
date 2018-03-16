#pragma once

// Setup all the entities into a string array
bool bsp_setupEntities();

// Get number of entities matching the string
int bsp_getNumEntities ( char *whichEntity );

// Reset the searched state for each entity
void bsp_resetEntitySearchFlag();

// Return the setID for the entity
int bsp_getEntitySetID ( char *entityStr, bool checkAll );

// Find the location and values for a entity class - return value in entityValue
int bsp_findEntityInfo ( char *entityStr, char *entityKey, glm::vec3 *entityValue, bool swapValues, int whichSetID, int valueType );

// Find the location and values for a entity class - return value in entityValue
char *bsp_findEntityInfoString ( char *entityStr, char *entityKey, int whichSetID );

// Get the number of models in the level
int bsp_entityGetNumDoors();

// Find the passed in entity and move the camera to it's origin
int bsp_placeCameraAtEntity ( char *param1 );
