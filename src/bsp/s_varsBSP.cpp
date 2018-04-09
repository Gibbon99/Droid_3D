#include "s_varsBSP.h"

int m_numOfVerts;			// The number of verts in the bsp
int m_numOfFaces;			// The number of faces in the bsp
int m_numOfTextures;		    // The number of texture maps
int m_numOfLightmaps;		// The number of light maps
int m_numOfMeshIndexes;      // Number of indexes into mesh data

int m_numOfNodes;
int m_numOfLeafs;
int m_numOfLeafFaces;
int m_numOfPlanes;

int m_numOfLeafBrushes;
int m_numOfBrushes;
int m_numOfBrushSides;

tBSPVertex		*m_pVerts;		// The object's vertices
tBSPFace		*m_pFaces;		// The faces information of the object
int             *m_pMeshIndex;  // Indexes for the mesh

tBSPNode		*m_pNodes;
tBSPLeaf		*m_pLeafs;
tBSPPlane		*m_pPlanes;
int				*m_pLeafFaces;
tBSPVisData		 m_clusters;

tBSPTexture		*m_pTextures;
int				*m_pLeafBrushes;           // The index into the brush array
tBSPBrush		*m_pBrushes;
tBSPBrushSide	*m_pBrushSides;

tBSPModel		*m_pModels;
char			*m_pEntities;				// Pointer to entity string
tBSPEntity		*m_pEntitiesStruct;			// Hold each of the entities

CBitset			m_FacesDrawn;				// The bitset for the faces that have/haven't been drawn

float            g_Gamma = 0.3f;                    // How much to lighten lightmaps by
unsigned int	 m_lightmaps[MAX_TEXTURES];	        // The lightmap texture array
int              m_numOfModels;			        // Number of models
int              sortCurrentFaceCount;       // Counter for sorting faces

float            m_Frustum[6][4];

int              numFacesDrawn;			// Faces drawn this frame
int              numFacesNotDrawn;		// Faces not drawn this frame
int              numLeafsDrawn;			// Leafs drawn this frame
int              numLeafsNotDrawn;		// Leafs skipped this frame

//_sortedFaces	*sortedFaces;
vector<_sortedFaces>	sortedFaces;

_entity         *entityList;
