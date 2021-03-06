
#include "s_lightMaps.h"
#include "s_varsBSP.h"
#include "s_renderBSP.h"
#include "s_entitiesBSP.h"
#include "s_shaderLights.h"
#include "s_shaders.h"
#include "s_doorsBSP.h"
#include "s_convertBSP.h"

//------------------------------------------------------------
//
// Upload vertex from BSP to GPU
void bsp_uploadVertices()
//------------------------------------------------------------
{
	bsp_uploadLevelVertex();
}

//------------------------------------------------------------
//
// Swap the Z and Y values so Y is up
glm::vec3 bsp_swapValues(glm::vec3 sourceValue)
//------------------------------------------------------------
{
	float temp;

	temp = sourceValue.y;
	sourceValue.y = sourceValue.z;
	sourceValue.z = temp;

	sourceValue.x = -sourceValue.x;

	return sourceValue;
}
//------------------------------------------------------------
//
// Setup initial values for BSP structures
void bsp_initBSP()
//------------------------------------------------------------
{
	//
	// Here we simply initialize our member variables to 0
	m_numOfVerts     = 0;
	m_numOfFaces     = 0;
	m_numOfTextures  = 0;
	m_numOfLightmaps = 0;
	m_numOfLightVolumes = 0;
	m_numOfMeshIndexes = 0;

	m_numOfNodes	 = 0;
	m_numOfLeafs	 = 0;
	m_numOfLeafFaces = 0;
	m_numOfPlanes	 = 0;

	m_numOfLeafBrushes = 0;
	m_numOfBrushes = 0;
	m_numOfBrushSides = 0;
	// Initialize all the dynamic BSP data pointers to NULL

	// Free memory if it's used before

	m_pVerts		 = NULL;
	m_pFaces		 = NULL;
	m_pMeshIndex     = NULL;

	m_pNodes		 = NULL;
	m_pLeafs		 = NULL;
	m_pPlanes		 = NULL;
	m_pLeafFaces	 = NULL;
	memset ( &m_clusters, 0, sizeof ( tBSPVisData ) );

	m_pTextures     = NULL;
	m_pLeafBrushes  = NULL;
	m_pBrushes      = NULL;
	m_pBrushSides   = NULL;
	m_pLightVols    = NULL;
	m_pModels       = NULL;
	m_pEntities     = NULL;
	m_pEntitiesStruct   = NULL;

	g_numFacesDrawn       = 0;		// Faces drawn this frame
	g_numFacesNotDrawn    = 0;		// Faces not drawn this frame
	g_numLeafsDrawn       = 0;		// Leafs drawn this frame
	g_numLeafsNotDrawng    = 0;		// Leafs skipped this frame
}

//------------------------------------------------------------
//
// Free memory used by BSP
void bsp_freeMem()
//------------------------------------------------------------
{
	// If we still have valid memory for our vertices, free them
	if ( m_pVerts )
		delete [] m_pVerts;

	m_pVerts = nullptr;

	// If we still have valid memory for our faces, free them
	if ( m_pFaces )
		delete [] m_pFaces;

	m_pFaces = nullptr;

	// If we still have valid memory for our nodes, free them
	if ( m_pNodes )
		delete [] m_pNodes;

	m_pNodes = nullptr;
//------------------------------------------------------------
	// If we still have valid memory for our leafs, free them
	if ( m_pLeafs )
		delete [] m_pLeafs;

	m_pLeafs = nullptr;

	// If we still have valid memory for our leaf faces, free them
	if ( m_pLeafFaces )
		delete [] m_pLeafFaces;

	m_pLeafFaces = nullptr;

	if ( m_pLeafBrushes )

		delete [] m_pLeafBrushes;

	m_pLeafBrushes = nullptr;

	if ( m_pBrushes )
		delete [] m_pBrushes;

	m_pBrushes = nullptr;

	if ( m_pBrushSides )
		delete [] m_pBrushSides;

	m_pBrushSides = nullptr;

	// If we still have valid memory for our planes, free them
	if ( m_pPlanes )
		delete [] m_pPlanes;

	m_pPlanes = nullptr;

	if ( m_pModels )
		delete [] m_pModels;

	m_pModels = nullptr;

	// If we still have valid memory for our clusters, free them
	if ( m_clusters.pBitsets )
		delete [] m_clusters.pBitsets;

	m_clusters.pBitsets = nullptr;

	if ( m_pTextures )
		delete [] m_pTextures;

	m_pTextures = nullptr;

	if ( m_pLightVols )
		delete [] m_pLightVols;

	m_pLightVols = nullptr;

	if ( m_pEntities != nullptr)
		free(m_pEntities);

	m_pEntities = nullptr;

	if ( m_pEntitiesStruct != nullptr)
		free (m_pEntitiesStruct);

	m_pEntitiesStruct = nullptr;

	m_FacesDrawn.FreeMem();
}


//------------------------------------------------------------
//
// Load a BSP file into memory
bool bsp_loadBSP ( const char *strFileName, bool verboseOutput )
//------------------------------------------------------------
{
	PHYSFS_file		*filePtr = nullptr;

	int i = 0;

	bsp_freeMem();
	bsp_initBSP();

	//
	// Check if the .bsp file could be opened
	if ( ( filePtr = PHYSFS_openRead ( strFileName ) ) == nullptr )
		{
			// Display an error message and quit if the file can't be found.
			con_print ( CON_ERROR, true, "Couldn't find BSP file to load [ %s ]", strFileName );
			return false;
		}

	// Initialize the header and lump structures
	tBSPHeader header = {0};
	tBSPLump lumps[kMaxLumps] = {0};

	// Read in the header and lump data
	PHYSFS_readBytes ( filePtr,  &header, sizeof ( tBSPHeader ));
	con_print ( CON_INFO, true, "BSP [ %s ] Version [ %i ]", header.strID, header.version );

	if ( header.version != 0x2e )
		{
			con_print ( CON_ERROR, true, "ERROR: BSP version does not match." );
			return false;
		}

	PHYSFS_readBytes ( filePtr, &lumps, sizeof ( tBSPLump ) * kMaxLumps);

	// Now we know all the information about our file.  We can
	// then allocate the needed memory for our member variables.

	// Allocate the vertex memory
	m_numOfVerts = lumps[kVertices].length / sizeof ( tBSPVertex );
	m_pVerts     = new tBSPVertex [m_numOfVerts];

	// Allocate the mesh index memory
	m_numOfMeshIndexes = lumps[kMeshVerts].length / sizeof ( int );
	m_pMeshIndex = new int [m_numOfMeshIndexes];

	// Allocate the face memory
	m_numOfFaces = lumps[kFaces].length / sizeof ( tBSPFace );
	m_pFaces     = new tBSPFace [m_numOfFaces];

	// Allocate memory to read in the texture information.
	m_numOfTextures = lumps[kTextures].length / sizeof ( tBSPTexture );
	m_pTextures = new tBSPTexture [m_numOfTextures];

	// Allocate memory to read in the lightmap data.
	m_numOfLightmaps = lumps[kLightmaps].length / sizeof ( tBSPLightmap );
	tBSPLightmap *pLightmaps = new tBSPLightmap [m_numOfLightmaps ];

	m_pEntities = ( char * ) malloc ( lumps[kEntities].length );

	if ( nullptr == m_pEntities )
		RET_FALSE ( "Out of memory error", true );

	// Get the string containing all the entities

	PHYSFS_seek ( filePtr, lumps[kEntities].offset );
	PHYSFS_readBytes ( filePtr, m_pEntities, sizeof ( char ) * lumps[kEntities].length);
// TODO (dberry#1#): Causes crash writing text to logfile - too long and overwrites memory

//	con_print(CON_INFO, true, "Entities [ %s ]", m_pEntities);
	// Seek to the position in the file that stores the vertex information
	PHYSFS_seek ( filePtr, lumps[kVertices].offset );

	// Go through all of the vertices that need to be read and swap axises
	for ( i = 0; i < m_numOfVerts; i++ )
		{
			// Read in the current vertex
			PHYSFS_readBytes ( filePtr, &m_pVerts[i], sizeof ( tBSPVertex ) * 1);

			m_pVerts[i].vPosition = bsp_swapValues (m_pVerts[i].vPosition);
			m_pVerts[i].vNormal = bsp_swapValues (m_pVerts[i].vNormal);

			// Negate the V texture coordinate because it is upside down otherwise...
			m_pVerts[i].vTextureCoord.y *= -1;
//			m_pVerts[i].vLightmapCoord.y *= -1;
		}

	PHYSFS_seek ( filePtr, lumps[kMeshVerts].offset );
	PHYSFS_readBytes ( filePtr, m_pMeshIndex, sizeof ( int ) * m_numOfMeshIndexes);

	// Seek to the position in the file that stores the face information
	PHYSFS_seek ( filePtr, lumps[kFaces].offset );

	// Read in all the face information
	PHYSFS_readBytes ( filePtr, m_pFaces, sizeof ( tBSPFace ) * m_numOfFaces);

	//
	// Swap normals for each face
	for ( i = 0; i < m_numOfFaces; i++ )
		{
			m_pFaces[i].vNormal = bsp_swapValues (m_pFaces[i].vNormal);

//			con_print(CON_INFO, true, "Face [ %i ] lightmapID [ %i ]", i, m_pFaces[i].lightmapID);
//			con_print(CON_INFO, true, "Face [ %i ] effects [ %i ]", i, m_pFaces[i].effect);
		}

	// Seek to the position in the file that stores the texture information
	PHYSFS_seek ( filePtr, lumps[kTextures].offset );

	// Read in all the texture information
	PHYSFS_readBytes ( filePtr, m_pTextures, sizeof ( tBSPTexture ) * m_numOfTextures);

	for ( i = 0; i < m_numOfTextures; i++ )
		{
			con_print ( CON_INFO, true, "Texture [ %i ] - [ %s ] flag [ %i ]", i, m_pTextures[i].strName, m_pTextures[i].contents );
		}

	// Seek to the position in the file that stores the lightmap information
	PHYSFS_seek ( filePtr, lumps[kLightmaps].offset );

	// Go through all of the lightmaps and read them in
	for ( i = 0; i < m_numOfLightmaps ; i++ )
		{
			// Read in the RGB data for each lightmap
			PHYSFS_readBytes ( filePtr, &pLightmaps[i].imageBits, sizeof ( tBSPLightmap ) * 1);

			// Create a texture map for each lightmap that is read in.  The lightmaps
			// are always 128 by 128.
			bsp_createLightmapTexture ( m_lightmaps[i], ( unsigned char * ) pLightmaps[i].imageBits, 128, 128 );

			printf ( "lightmap ID after texture [ %i ]\n", m_lightmaps[i] );
		}

	// Delete the image bits because we are already done with them
//	delete [] pLightmaps;

	m_numOfLightVolumes = lumps[kLightVolumes].length / sizeof (tBSPLightVols);
	m_pLightVols = new tBSPLightVols [m_numOfLightVolumes];

	// Seek to the position in the file that stores the light volume information
	PHYSFS_seek ( filePtr, lumps[kLightVolumes].offset );
	PHYSFS_readBytes (filePtr, m_pLightVols, sizeof (tBSPLightVols) * m_numOfLightVolumes );

	// In this function we read from a bunch of new lumps.  These include
	// the BSP nodes, the leafs, the leaf faces, BSP splitter planes and
	// visibility data (clusters).

	// Store the number of nodes and allocate the memory to hold them
	m_numOfNodes = lumps[kNodes].length / sizeof ( tBSPNode );
	m_pNodes     = new tBSPNode [m_numOfNodes];

	// Seek to the position in the file that hold the nodes and store them in m_pNodes
	PHYSFS_seek ( filePtr, lumps[kNodes].offset );
	PHYSFS_readBytes ( filePtr, m_pNodes, sizeof ( tBSPNode ) * m_numOfNodes);

	for ( i = 0; i < m_numOfNodes; i++ )
		{
			// Swap the y and z values, and negate the new z so Y is up.
			int temp = m_pNodes[i].max.y;
			m_pNodes[i].max.y = m_pNodes[i].max.z;
			m_pNodes[i].max.z = temp;
			m_pNodes[i].max.x = -m_pNodes[i].max.x;

			temp = m_pNodes[i].min.y;
			m_pNodes[i].min.y = m_pNodes[i].min.z;
			m_pNodes[i].min.z = temp;
			m_pNodes[i].min.x = -m_pNodes[i].min.x;
		}

	// Store the number of leafs and allocate the memory to hold them
	m_numOfLeafs = lumps[kLeafs].length / sizeof ( tBSPLeaf );
	m_pLeafs     = new tBSPLeaf [m_numOfLeafs];

	// Seek to the position in the file that holds the leafs and store them in m_pLeafs
	PHYSFS_seek ( filePtr, lumps[kLeafs].offset );
	PHYSFS_readBytes ( filePtr, m_pLeafs, sizeof ( tBSPLeaf ) * m_numOfLeafs);

	// Now we need to go through and convert all the leaf bounding boxes
	// to the normal OpenGL Y up axis.
	for ( i = 0; i < m_numOfLeafs; i++ )
		{
			// Swap the min y and z values, then negate the new Z
			int temp = m_pLeafs[i].min.y;
			m_pLeafs[i].min.y = m_pLeafs[i].min.z;
			m_pLeafs[i].min.z = temp;
			m_pLeafs[i].min.x = -m_pLeafs[i].min.x;

			// Swap the max y and z values, then negate the new Z
			temp = m_pLeafs[i].max.y;
			m_pLeafs[i].max.y = m_pLeafs[i].max.z;
			m_pLeafs[i].max.z = temp;
			m_pLeafs[i].max.x = -m_pLeafs[i].max.x;
		}

	// Store the number of leaf faces and allocate the memory for them
	m_numOfLeafFaces = lumps[kLeafFaces].length / sizeof ( int );
	m_pLeafFaces     = new int [m_numOfLeafFaces];

	// Seek to the leaf faces lump, then read it's data
	PHYSFS_seek ( filePtr, lumps[kLeafFaces].offset );
	PHYSFS_readBytes ( filePtr, m_pLeafFaces, sizeof ( int ) * m_numOfLeafFaces);

	// Store the number of planes, then allocate memory to hold them
	m_numOfPlanes = lumps[kPlanes].length / sizeof ( tBSPPlane );
	m_pPlanes     = new tBSPPlane [m_numOfPlanes];

	// Seek to the planes lump in the file, then read them into m_pPlanes
	PHYSFS_seek ( filePtr, lumps[kPlanes].offset );
	PHYSFS_readBytes ( filePtr, m_pPlanes, sizeof ( tBSPPlane ) * m_numOfPlanes);

	// Go through every plane and convert it's normal to the Y-axis being up
	for ( i = 0; i < m_numOfPlanes; i++ )
		{
			m_pPlanes[i].vNormal = bsp_swapValues (m_pPlanes[i].vNormal);
		}

	// Seek to the position in the file that holds the visibility lump
	PHYSFS_seek ( filePtr, lumps[kVisData].offset );

	// Check if there is any visibility information first
	if ( lumps[kVisData].length )
		{
			// Read in the number of vectors and each vector's size
			PHYSFS_readBytes ( filePtr, & ( m_clusters.numOfClusters ), sizeof ( int ) * 1);
			PHYSFS_readBytes ( filePtr, & ( m_clusters.bytesPerCluster ), sizeof ( int ) * 1);

			// Allocate the memory for the cluster bitsets
			int size = m_clusters.numOfClusters * m_clusters.bytesPerCluster;
			m_clusters.pBitsets = new byte [size];

			// Read in the all the visibility bitsets for each cluster
			PHYSFS_readBytes ( filePtr, m_clusters.pBitsets, sizeof ( byte ) * size * 1);
		}

	// Otherwise, we don't have any visibility data (prevents a crash)
	else
		m_clusters.pBitsets = nullptr;

	// Read in index to leaf brushes
	m_numOfLeafBrushes = lumps[kLeafBrushes].length / sizeof ( int );
	m_pLeafBrushes = new int[m_numOfLeafBrushes];
	PHYSFS_seek ( filePtr, lumps[kLeafBrushes].offset );
	PHYSFS_readBytes ( filePtr, m_pLeafBrushes, sizeof ( int ) * m_numOfLeafBrushes);

	// Read in information about the solid models used - doors etc
	m_numOfModels = lumps[kModels].length / sizeof ( tBSPModel );
	m_pModels = new tBSPModel[m_numOfModels];
	PHYSFS_seek ( filePtr, lumps[kModels].offset );
	PHYSFS_readBytes ( filePtr, m_pModels, sizeof ( tBSPModel ) * m_numOfModels);

	//
	// Now we need to convert all the bounding boxes to OpenGL - Y axis up
	for ( i = 0; i < m_numOfModels; i++ )
		{
			m_pModels[i].min = bsp_swapValues (m_pModels[i].min);
			m_pModels[i].max = bsp_swapValues (m_pModels[i].max);
		}

	// Read in the brushes
	m_numOfBrushes = lumps[kBrushes].length / sizeof ( tBSPBrush );
	m_pBrushes = new tBSPBrush[m_numOfBrushes];

	PHYSFS_seek ( filePtr, lumps[kBrushes].offset );
	PHYSFS_readBytes ( filePtr, m_pBrushes, sizeof ( tBSPBrush ) * m_numOfBrushes);

	// Read in the brush sides
	m_numOfBrushSides =	lumps[kBrushSides].length / sizeof ( tBSPBrushSide );
	m_pBrushSides = new tBSPBrushSide[m_numOfBrushSides];
	PHYSFS_seek ( filePtr, lumps[kBrushSides].offset );
	PHYSFS_readBytes ( filePtr, m_pBrushSides, sizeof ( tBSPBrushSide ) * m_numOfBrushSides);
	//
	// Close the file
	PHYSFS_close ( filePtr );

	// Here we allocate enough bits to store all the faces for our bitset
	m_FacesDrawn.Resize ( m_numOfFaces );

	//
	// Setup the entities into searchable string array
	bsp_setupEntities();

	if ( verboseOutput )
		{
			con_print ( CON_INFO, true, "Info for loaded BSP file [ %s ]", strFileName );
			con_print ( CON_INFO, true, "numVerts [ %i ] numFaces [ %i ] numTextures [ %i ] numLightmaps [ %i ]", m_numOfVerts, m_numOfFaces, m_numOfTextures, m_numOfLightmaps );
			con_print ( CON_INFO, true, "numLightVolumes [ %i ] numMeshIndexes [ %i ] nodes [ %i ] leafs [ %i ] leafFaces [ %i ] planes [ %i ]", m_numOfLightVolumes, m_numOfNodes, m_numOfLeafs, m_numOfLeafFaces, m_numOfPlanes );
			con_print ( CON_INFO, true, "leafBrushes [ %i ] brushes [ %i ] brushSides [ %i ]", m_numOfLeafBrushes, m_numOfBrushes, m_numOfBrushSides );
			con_print ( CON_INFO, true, "---------------------------------------------------------------" );
		}

	if ( -1 == bsp_placeCameraAtEntity ("info_player_start" ) )
		printf ( "Couldn't find player start\n" );

	bsp_setLightArrayData();

	bsp_setupLightVolumeData();

	printf ( "Num doors [ %i ]\n", bsp_findNumOfDoors() );

	bspConvertMesh(1.0f);

	bsp_uploadVertices();
	
	// Return a success
	return true;
}
