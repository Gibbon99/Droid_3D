#pragma once

// Setup all the entities into a string array
bool bsp_setupEntities();

// Get number of entities matching the string
int bsp_getNumEntities ( string whichEntity );

// Reset the searched state for each entity
void bsp_resetEntitySearchFlag();

// Return the setID for the entity
int bsp_getEntitySetID ( string entityStr, bool checkAll );

// Find the location and values for a entity class - return value in entityValue
int bsp_findEntityInfo ( string entityStr, string entityKey, glm::vec3 *entityValue, bool swapValues, int whichSetID, int valueType );

// Get the number of models in the level
int bsp_entityGetNumDoors();

// Find the passed in entity and move the camera to it's origin
int bsp_placeCameraAtEntity ( string param1 );
