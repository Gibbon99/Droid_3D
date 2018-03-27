#pragma once

#include "s_globals.h"

#define MAX_TEXTURES	1000				// The maximum amount of textures to load

// This is the number that is associated with a face that is of type "polygon"
#define FACE_POLYGON	1

#define BSPCONTENTS_SOLID 1

#define MAX_ENTITY_SIZE	512					// Max size for a entity string

#define byte unsigned char


// This is our integer vector structure
struct tVector3i
{
	int x, y, z;				// The x y and z position of our integer vector
};


// This is our BSP header structure
struct tBSPHeader
{
	char strID[4];				// This should always be 'IBSP'
	int version;				// This should be 0x2e for Quake 3 files
};


// This is our BSP lump structure
struct tBSPLump
{
	int offset;					// The offset into the file for the start of this lump
	int length;					// The length in bytes for this lump
};


// This is our BSP vertex structure
struct tBSPVertex
{
	glm::vec3 vPosition;			// (x, y, z) position.
	glm::vec2 vTextureCoord;		// (u, v) texture coordinate
	glm::vec2 vLightmapCoord;	    // (u, v) lightmap coordinate
	glm::vec3 vNormal;			    // (x, y, z) normal vector
	byte color[4];				    // RGBA color for the vertex
};


// This is our BSP face structure
struct tBSPFace
{
	int textureID;				// The index into the texture array
	int effect;					// The index for the effects (or -1 = n/a)
	int type;					// 1=polygon, 2=patch, 3=mesh, 4=billboard
	int startVertIndex;			// The starting index into this face's first vertex
	int numOfVerts;				// The number of vertices for this face
	int startMeshVertIndex;		// The index into the first meshvertex
	int numMeshVerts;			// The number of mesh vertices
	int lightmapID;				// The texture index for the lightmap
	int lMapCorner[2];			// The face's lightmap corner in the image
	int lMapSize[2];			// The size of the lightmap section
	glm::vec3 lMapPos;			// The 3D origin of lightmap.
	glm::vec3 lMapVecs[2];		// The 3D space for s and t unit vectors.
	glm::vec3 vNormal;			// The face normal.
	int size[2];				// The bezier patch dimensions.
};


// This is our BSP texture structure
struct tBSPTexture
{
	char strName[64];			// The name of the texture w/o the extension
	int flags;					// The surface flags (unknown)
	int contents;				// The content flags (unknown)
};

// This is our BSP lightmap structure which stores the 128x128 RGB values
struct tBSPLightmap
{
	byte imageBits[128][128][3];   // The RGB data in a 128x128 image
};


// This stores a node in the BSP tree
struct tBSPNode
{
	int plane;					// The index into the planes array
	int front;					// The child index for the front node
	int back;					// The child index for the back node
	tVector3i min;				// The bounding box min position.
	tVector3i max;				// The bounding box max position.
};

// This stores a leaf (end node) in the BSP tree
struct tBSPLeaf
{
	int cluster;				// The visibility cluster
	int area;					// The area portal
	tVector3i min;				// The bounding box min position
	tVector3i max;				// The bounding box max position
	int leafface;				// The first index into the face array
	int numOfLeafFaces;			// The number of faces for this leaf
	int leafBrush;				// The first index for into the brushes
	int numOfLeafBrushes;		// The number of brushes for this leaf
};

// This stores a splitter plane in the BSP tree
struct tBSPPlane
{
	glm::vec3 vNormal;			// Plane normal.
	float d;					// The plane distance from origin
};

// This stores the cluster data for the PVS's
struct tBSPVisData
{
	int numOfClusters;			// The number of clusters
	int bytesPerCluster;		// The amount of bytes (8 bits) in the cluster's bitset
	byte *pBitsets;				// The array of bytes that holds the cluster bitsets
};

//	The brushes store information about a convex volume, which are defined by the brush sides.
//	Brushes are used for collision detection.  This allows the level editor to decide what is
//	collidable and what can be walked through, such as trees, bushes or certain corners.

struct tBSPBrush
{
	int brushSide;           // The starting brush side for the brush
	int numOfBrushSides;     // Number of brush sides for the brush
	int textureID;           // The texture index for the brush
};


//The brush sides lump stores information about the brush bounding surface.
//To calculate the number of brush sides, just divide the lumps
//length by sizeof(tBSPBrushSides).
struct tBSPBrushSide
{
	int plane;		// The plane index
	int textureID;	// The texture index
};

// A structure to hold all the individual entities into their full string value

struct tBSPEntity
{
	GLboolean	checked;
	char		value[MAX_ENTITY_SIZE];
};

struct tBSPModel
{
	glm::vec3	min;            // Bounding box min coord.
	glm::vec3	max;            // Bounding box max coord.
	int			firstSurface;   // First face for model.
	int			numSurfaces;    // Number of faces for model.
	int			firstBrush;     // First brush for model.
	int			numBrushes;     // Number of brushes for model.
};

// This is our lumps enumeration
enum eLumps
{
	kEntities = 0,				// Stores player/object positions, etc...
	kTextures,					// Stores texture information
	kPlanes,				    // Stores the splitting planes
	kNodes,						// Stores the BSP nodes
	kLeafs,						// Stores the leafs of the nodes
	kLeafFaces,					// Stores the leaf's indices into the faces
	kLeafBrushes,				// Stores the leaf's indices into the brushes
	kModels,					// Stores the info of world models
	kBrushes,					// Stores the brushes info (for collision)
	kBrushSides,				// Stores the brush surfaces info
	kVertices,					// Stores the level vertices
	kMeshVerts,					// Stores the model vertices offsets
	kShaders,					// Stores the shader files (blending, anims..)
	kFaces,						// Stores the faces for the level
	kLightmaps,					// Stores the lightmaps for the level
	kLightVolumes,				// Stores extra world lighting information
	kVisData,					// Stores PVS and cluster info (visibility)
	kMaxLumps					// A constant to store the number of lumps
};


// This is our bitset class for storing which face has already been drawn.
// The bitset functionality isn't really taken advantage of in this version
// since we aren't rendering by leafs and nodes.
class CBitset
{

public:

	// Initialize all the data members
	CBitset() : m_bits ( 0 ), m_size ( 0 ) {}

	// This is our deconstructor
	~CBitset()
	{
		// If we have valid memory, get rid of it
		if ( m_bits )
			{
				delete m_bits;
				m_bits = NULL;
			}
	}

	// Free memory used by the class - isn't calling the deconstructor ?
	void FreeMem()
	{
		if ( m_bits )
			{
				delete [] m_bits;
				m_bits = NULL;
			}
	}

	// This resizes our bitset to a size so each face has a bit associated with it
	void Resize ( int count )
	{
		// Get the size of integers we need
		m_size = count/32 + 1;

		// Make sure we haven't already allocated memory for the bits
		if ( m_bits )
			{
				delete [] m_bits;
				m_bits = 0;
			}

		// Allocate the bits and initialize them
		m_bits = new unsigned int[m_size];
		ClearAll();
	}

	// This does the binary math to set the desired bit
	void Set ( int i )
	{
		m_bits[i >> 5] |= ( 1 << ( i & 31 ) );
	}

	// This returns if the desired bit slot is a 1 or a 0
	int On ( int i )
	{
		return m_bits[i >> 5] & ( 1 << ( i & 31 ) );
	}

	// This clears a bit to 0
	void Clear ( int i )
	{
		m_bits[i >> 5] &= ~ ( 1 << ( i & 31 ) );
	}

	// This initializes the bits to 0
	void ClearAll()
	{
		memset ( m_bits, 0, sizeof ( unsigned int ) * m_size );
	}

private:

	// Our private bit data that holds the bits and size
	unsigned int *m_bits;
	int m_size;
};

//-----------------------------------------------------------------------------
//
// Used to sort faces according to textureID
//
//-----------------------------------------------------------------------------

typedef struct
{
	int				faceID;
	int				textureID;
	int				leafIndex;			// which leaf in the BSP tree does this face belong to
} _sortedFaces;

extern _sortedFaces	*sortedFaces;

extern CBitset			m_FacesDrawn;		// The bitset for the faces that have/haven't been drawn


extern int m_numOfVerts;			// The number of verts in the bsp
extern int m_numOfFaces;			// The number of faces in the bsp
extern int m_numOfTextures;		    // The number of texture maps
extern int m_numOfLightmaps;		// The number of light maps
extern int m_numOfMeshIndexes;      // Number of indexes into mesh data

extern int m_numOfNodes;
extern int m_numOfLeafs;
extern int m_numOfLeafFaces;
extern int m_numOfPlanes;

extern int m_numOfLeafBrushes;
extern int m_numOfBrushes;
extern int m_numOfBrushSides;

extern tBSPVertex		*m_pVerts;		// The object's vertices
extern tBSPFace			*m_pFaces;		// The faces information of the object
extern int              *m_pMeshIndex;  // Indexes for the mesh

extern tBSPNode			*m_pNodes;
extern tBSPLeaf			*m_pLeafs;
extern tBSPPlane		*m_pPlanes;
extern int				*m_pLeafFaces;
extern tBSPVisData		m_clusters;

extern tBSPTexture		*m_pTextures;
extern int				*m_pLeafBrushes;           // The index into the brush array
extern tBSPBrush		*m_pBrushes;
extern tBSPBrushSide	*m_pBrushSides;

extern tBSPModel		*m_pModels;
extern char				*m_pEntities;				// Pointer to entity string
extern tBSPEntity		*m_pEntitiesStruct;			// Hold each of the entities

extern float            g_Gamma;                    // How much to lighten lightmaps by
extern unsigned int	    m_lightmaps[MAX_TEXTURES];	        // The lightmap texture array
extern int              m_numOfModels;			        // Number of models

extern CBitset			m_FacesDrawn;				// The bitset for the faces that have/haven't been drawn

extern int              sortCurrentFaceCount;       // Counter for sorting faces

extern float            m_Frustum[6][4];

extern int              numFacesDrawn;			// Faces drawn this frame
extern int              numFacesNotDrawn;		// Faces not drawn this frame
extern int              numLeafsDrawn;			// Leafs drawn this frame
extern int              numLeafsNotDrawn;		// Leafs skipped this frame


//
// struct array to hold each token from entity string

#define MAX_ENTITY_STRING_SIZE  64
#define NUM_TOKEN_KEYS          25

extern      uint     g_numEntityKeys;
extern      uint     g_numEntities;

typedef struct
{
	int     setID;  // associate each token together
	bool    checked;    // have we checked this key yet
	char    tokenName   [MAX_ENTITY_STRING_SIZE];
	char    tokenValue  [MAX_ENTITY_STRING_SIZE];
} _entity;

extern _entity  *entityList;
