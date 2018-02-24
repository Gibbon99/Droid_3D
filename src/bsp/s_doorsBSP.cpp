#include "s_varsBSP.h"
#include "s_entitiesBSP.h"
#include "../../hdr/bsp/s_doorsBSP.h"

int		numOfDoors;

//------------------------------------------------------------------
//
// Get the number of doors in the level
int bsp_findNumOfDoors()
//------------------------------------------------------------------
{
	//
	// Save value for use later on
	numOfDoors = bsp_getNumEntities("func_door");

	if (numOfDoors == 0)
		return 0;
}