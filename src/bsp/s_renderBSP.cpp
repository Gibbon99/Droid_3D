#include "s_globals.h"
#include "s_vsProfileLib.h"
#include "s_varsBSP.h"
#include "s_maths.h"
#include "s_shaders.h"
#include "io_textures.h"
#include "s_openGLWrap.h"
#include "s_doorsBSP.h"
#include "s_shaderLights.h"
#include <vector>
#include <hdr/bsp/s_renderBSP.h>
#include <hdr/openGL/s_shaders.h>
#include <hdr/io/io_textures.h>

#define FACE_RENDER		0
#define FACE_ADD		1

bool            g_renderTextures = true;

GLuint          elementArrayID;

glm::vec3       g_cameraPosition;
int             g_currentCameraCluster;

int             g_vertIndexCounter = 0;
//int				g_texturesChanges = 0;
int				g_numVertexPerFrame = 0;

GLuint			bspVAO, bspVBO;

vector<unsigned int>				g_currentFrameVertexIndex;
vector<_myVertex>					g_levelDataVertex;
vector<int>							g_facesForFrame;

//-----------------------------------------------------------------------------
//
// Display BSP data
void bsp_debugBSPData ( tBSPFace *ptrFace, int index )
//-----------------------------------------------------------------------------
{
	static bool debugDone = false;

	if ( false == debugDone )
	{
		con_print ( CON_INFO, true, "Num verts mesh verts [ %i ] Num, Triangles [ %i ]", ptrFace->numMeshVerts, ptrFace->numMeshVerts / 3 );

		for ( int i = 0; i != ptrFace->numMeshVerts; i++ )
		{
			con_print ( CON_INFO, true, "Face meshVertIndex [ %i ] meshVertIndex [ %i ]", ptrFace->startMeshVertIndex + i, m_pMeshIndex[ptrFace->startMeshVertIndex + i] );
			con_print ( CON_INFO, true, "VertexPos [ %i ] [ %3.3f %3.3f %3.3f ]", m_pMeshIndex[ptrFace->startMeshVertIndex + i],
			            m_pVerts[m_pMeshIndex[ptrFace->startMeshVertIndex + i]].vPosition.x,
			            m_pVerts[m_pMeshIndex[ptrFace->startMeshVertIndex + i]].vPosition.y,
			            m_pVerts[m_pMeshIndex[ptrFace->startMeshVertIndex + i]].vPosition.z );
		}

		con_print ( CON_INFO, true, "Vertex information from our array - comparision" );

		debugDone = true;
	}
}

//-----------------------------------------------------------------------------
//
// Prepare to render faces - called each frame
//
// Clear the vectors and bind the data buffers
void bsp_bindLevelData()
//-----------------------------------------------------------------------------
{
	GL_CHECK ( glBindVertexArray ( bspVAO ) );
	//
	// Bind data buffer holding all the vertices
	GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, bspVBO ));

	g_facesForFrame.clear();
	g_currentFrameVertexIndex.clear();
	g_vertIndexCounter = 0;
	g_numVertexPerFrame = 0;
}

//-----------------------------------------------------------------------------
//
// Sort callback for textureID in faces
bool bsp_sortFacesCallback(int lhs, int rhs)
//-----------------------------------------------------------------------------
{
	tBSPFace		*faceOne;
	tBSPFace		*faceTwo;

	faceOne = &m_pFaces[lhs];
	faceTwo = &m_pFaces[rhs];

	return faceOne->textureID < faceTwo->textureID;
}

//-----------------------------------------------------------------------------
//
// Go through the vector face indexes and render the face - including door models
//
// Sort the faces based on textureID
void bsp_renderAllFaces(int whichShader)
//-----------------------------------------------------------------------------
{
	tBSPFace				*ptrFace;
	static int 				previousTexture = 9999;
	unsigned int 			i = 0;

	if ( g_renderTextures )
		std::sort(g_facesForFrame.begin(), g_facesForFrame.end(), bsp_sortFacesCallback);

	ptrFace = &m_pFaces[g_facesForFrame[0]];
	previousTexture = ptrFace->textureID;

	for (i = 0; i != g_facesForFrame.size(); i++)
	{
		ptrFace = &m_pFaces[g_facesForFrame[i]];

		if (previousTexture == ptrFace->textureID)
		{
			bsp_renderFace(g_facesForFrame[i], FACE_ADD, previousTexture, whichShader);
			ptrFace = &m_pFaces[g_facesForFrame[i]];
			previousTexture = ptrFace->textureID;
		}
		else
		{
			bsp_renderFace(g_facesForFrame[i], FACE_RENDER, previousTexture, whichShader);
			bsp_renderFace(g_facesForFrame[i], FACE_ADD, previousTexture, whichShader);
			ptrFace = &m_pFaces[g_facesForFrame[i]];
			previousTexture = ptrFace->textureID;
		}
	}
	//
	// Render last face from the previous FACE_ADD
	bsp_renderFace(g_facesForFrame[g_facesForFrame.size() - 1], FACE_RENDER, previousTexture, whichShader);
}

//-----------------------------------------------------------------------------
//
// Add the index for the face into vector array
void bsp_addFaceToArray(int whichFace )
//-----------------------------------------------------------------------------
{
	g_facesForFrame.push_back(whichFace);
}

//-----------------------------------------------------------------------------
//
// Create array that holds the vertex index for visible faces this frame
//
// Level data is all on GPU, this index is used to draw based on that data
void bsp_createVextexIndexArray ( tBSPFace *ptrFace )
//-----------------------------------------------------------------------------
{
	vector<GLuint>      indexes;
	_myVertex           tempVertex;

	indexes.clear();
	indexes.reserve (static_cast<unsigned long>(ptrFace->numMeshVerts));

	for ( int i = 0; i != ptrFace->numMeshVerts; i++ )
	{
		indexes[i] = static_cast<unsigned int>(m_pMeshIndex[ptrFace->startMeshVertIndex + i]);

		g_currentFrameVertexIndex.push_back (ptrFace->startVertIndex + indexes[i]);
		g_numVertexPerFrame++;
	}

	g_vertIndexCounter++;
}

//-----------------------------------------------------------------------------
//
// Actually draw the BSP face
void bsp_renderFace ( int whichFace, int whichAction, int whichTexture, int whichShader )
//-----------------------------------------------------------------------------
{
	static int          errorCount = 0;
	static int          errorCountLightmap = 0;
	tBSPFace			*ptrFace;

	//
	// Quit if we start accessing faces using an invalid index ( whichFace )
	if (whichFace > m_numOfFaces)
	{
		errorCount++;
		con_print(CON_INFO, true, "Invalid face index [ %i ] passed > [ %i ]", whichFace, m_numOfFaces);
		if (errorCount > 10)
		{
			con_print(CON_ERROR, true, "Memory or level corruption while indexing faces. Exiting.");
			changeMode (MODE_SHUTDOWN);
		}
	}

	switch (whichAction)
	{
		case FACE_ADD:
		{
			ptrFace = &m_pFaces[whichFace];
			bsp_createVextexIndexArray (ptrFace);
		}
		break;

		case FACE_RENDER:
		{
			switch (whichShader)
			{
				case SHADER_GEOMETRY_PASS:
				case SHADER_MODEL_PASS:
				{
					//
					// Get a pointer to the current face
					ptrFace = &m_pFaces[whichFace];
					//
					// Quit if we start accessing memory outside the lightMap array size
					if ( ptrFace->lightmapID > m_numOfLightmaps )
					{
						wrapglBindTexture (GL_TEXTURE0, m_lightmaps[0]);

						errorCountLightmap++;
						if ( errorCountLightmap > 5 )
						{
							con_print (CON_ERROR, true, "Memory or level corruption while indexing lightmap array. Exiting.");
							changeMode (MODE_SHUTDOWN);
						}
					}
					else if ( ptrFace->lightmapID < 0 )   // No lightmap for this face?
						wrapglBindTexture (GL_TEXTURE0, io_getGLTexID (whichTexture));
					else
						wrapglBindTexture (GL_TEXTURE0, m_lightmaps[ptrFace->lightmapID]);

					glUniform1i (shaderProgram[whichShader].inTextureUnit, 0);

					if ( g_renderTextures )
					{
						wrapglBindTexture (GL_TEXTURE1, io_getGLTexID (whichTexture));
						glUniform1i (shaderProgram[whichShader].inTextureUnit_1, 1);
					}

					GL_CHECK (glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, elementArrayID));

					GL_CHECK (glBufferData (GL_ELEMENT_ARRAY_BUFFER, g_currentFrameVertexIndex.size () * sizeof (unsigned int), &g_currentFrameVertexIndex[0], GL_DYNAMIC_DRAW));

					GL_ASSERT (glDrawElements (GL_TRIANGLES, g_currentFrameVertexIndex.size (), GL_UNSIGNED_INT, 0));
					g_currentFrameVertexIndex.clear ();
				}
				break;

				case SHADER_SHADOW_MAP:
				case SHADER_SHADOW_LIGHTING:
				{
					GL_CHECK ( glBindVertexArray ( bspVAO ) );
					//
					// Bind data buffer holding all the vertices
					GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, bspVBO ));

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, io_getGLTexID (whichTexture));

					GL_CHECK (glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, elementArrayID));

					GL_CHECK (glBufferData (GL_ELEMENT_ARRAY_BUFFER, g_currentFrameVertexIndex.size () * sizeof (unsigned int), &g_currentFrameVertexIndex[0], GL_DYNAMIC_DRAW));

					GL_ASSERT (glDrawElements (GL_TRIANGLES, g_currentFrameVertexIndex.size (), GL_UNSIGNED_INT, 0));
					g_currentFrameVertexIndex.clear ();
				}
				break;

				default:
					break;
			}
		}
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//
// Upload all the vertex data in our own structure used to render the level
void bsp_uploadLevelVertex()
//-----------------------------------------------------------------------------
{
	_myVertex		tempVertex;
	int stride = sizeof ( _myVertex ); // BSP Vertex, not float[3]

	for (int i = 0; i != m_numOfVerts; i++)
	{
		if (false == g_renderTextures)
		{
			tempVertex.position = m_pVerts[i].vPosition;
			tempVertex.texCoordsLightmap = m_pVerts[i].vLightmapCoord;
			tempVertex.normals = m_pVerts[i].vNormal;
		}
		else
		{
			tempVertex.position = m_pVerts[i].vPosition;
			tempVertex.texCoords = m_pVerts[i].vTextureCoord;
			tempVertex.normals = m_pVerts[i].vNormal;
			tempVertex.texCoordsLightmap = m_pVerts[i].vLightmapCoord;
		}

		g_levelDataVertex.push_back(tempVertex);
	}

	con_print(CON_INFO, true, "Upload vertices to GPU. Size [ %i bytes ]", g_levelDataVertex.size() * sizeof ( _myVertex ));

	//
	// Store model and all meshes in one VAO
	GL_CHECK ( glGenVertexArrays ( 1, &bspVAO ) );
	GL_CHECK ( glBindVertexArray ( bspVAO ) );

	con_print(CON_INFO, true, "bspVAO is [ %i ]", bspVAO);

	bspVBO = wrapglGenBuffers(1, __func__);
	GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, bspVBO ));
	GL_CHECK ( glBufferData (GL_ARRAY_BUFFER, m_numOfVerts * sizeof ( _myVertex ), &g_levelDataVertex[0], GL_DYNAMIC_DRAW ) );

	// Position
	GL_ASSERT ( glEnableVertexAttribArray ( 0) );
	GL_ASSERT ( glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, stride, ( offsetof ( _myVertex, position ) ) ) );

	// Normals
	GL_ASSERT ( glEnableVertexAttribArray ( 1 ) );
	GL_ASSERT ( glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)( offsetof ( _myVertex, normals ) ) ) );

	// Texture coords for lightmap
//	GL_ASSERT ( glEnableVertexAttribArray ( 2 ) );
//	GL_ASSERT ( glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)( offsetof ( _myVertex, texCoordsLightmap ) ) ) );

	// Texture coords for diffuse texture
//	if ( g_renderTextures )
	{
		// Texture coords
		GL_ASSERT (glEnableVertexAttribArray (2));
		GL_ASSERT (glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (offsetof (_myVertex, texCoords))));
	}

	//
	// Generate indexArray ID - indexes into currently visible faces in current frame
	elementArrayID = wrapglGenBuffers(1, __func__);

	GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, 0 ));
	GL_CHECK ( glBindVertexArray(0) );
}

//-----------------------------------------------------------------------------
//
// This returns the leaf the camera is in
int bsp_findLeaf ( const glm::vec3 &vPos )
//-----------------------------------------------------------------------------
{
	int i = 0;
	float distance = 0.0f;

	// Continue looping until we find a negative index
	while ( i >= 0 )
	{
		const tBSPNode&  node = m_pNodes[i];
		const tBSPPlane& plane = m_pPlanes[node.plane];

		// Use the Plane Equation (Ax + by + Cz + D = 0) to find if the
		// camera is in front of or behind the current splitter plane.
		distance =	plane.vNormal.x * vPos.x +
		              plane.vNormal.y * vPos.y +
		              plane.vNormal.z * vPos.z - plane.d;

		// If the camera is in front of the plane
		if ( distance >= 0 )
		{
			// Assign the current node to the node in front of itself
			i = node.front;
		}

			// Else if the camera is behind the plane
		else
		{
			// Assign the current node to the node behind itself
			i = node.back;
		}
	}

	// Return the leaf index (same thing as saying:  return -(i + 1)).
	return ~i;  // Binary operation
}


//-----------------------------------------------------------------------------
//
//	This tells us if the "current" cluster can see the "test" cluster
inline int bsp_isClusterVisible ( int current, int test )
//-----------------------------------------------------------------------------
{
	if ( !m_clusters.pBitsets || current < 0 )
		return 1;

	// Use binary math to get the 8 bit visibility set for the current cluster
	byte visSet = m_clusters.pBitsets[ ( current*m_clusters.bytesPerCluster ) + ( test / 8 )];

	// Now that we have our vector (bitset), do some bit shifting to find if
	// the "test" cluster is visible from the "current" cluster, according to the bitset.
	int result = visSet & ( 1 << ( ( test ) & 7 ) );

	// Return the result ( either 1 (visible) or 0 (not visible) )
	return ( result );
}

//-----------------------------------------------------------------------------
//
// Render the faces in this leaf if they are in the frustum
void bsp_renderLeaf ( int iLeaf, bool CheckFrustum )
//-----------------------------------------------------------------------------
{
	// get the leaf we are in
	tBSPLeaf* pLeaf = &m_pLeafs[ iLeaf ];

	// perform PVS as in the previus tutorial
	if ( !bsp_isClusterVisible ( g_currentCameraCluster, pLeaf->cluster ) )    // If the current leaf can't be seen from our cluster, go to the next leaf
		return;

	if ( CheckFrustum )
		if ( COMPLETE_OUT == sys_boxInFrustum ( ( float ) pLeaf->min.x, ( float ) pLeaf->min.y, ( float ) pLeaf->min.z,
		                                        ( float ) pLeaf->max.x, ( float ) pLeaf->max.y, ( float ) pLeaf->max.z ) )
			return;

	int faceCount = pLeaf->numOfLeafFaces;

	// Loop through and render all of the faces in this leaf
	while ( faceCount-- )
	{
		// Grab the current face index from our leaf faces array
		int faceIndex = m_pLeafFaces[pLeaf->leafface + faceCount];

		if ( m_pFaces[faceIndex].type != FACE_POLYGON )
		{
			//
			// TODO: Do patches?
			//
			continue;
		}

		// Since many faces are duplicated in other leafs, we need to
		// make sure this face already hasn't been drawn.
		if ( !m_FacesDrawn.On ( faceIndex ) )
		{
			// Set this face as drawn and render it
			m_FacesDrawn.Set ( faceIndex );
			bsp_addFaceToArray(faceIndex);
			g_numFacesDrawn++;
		}

		else
			g_numFacesNotDrawn++;
	}
}

//-----------------------------------------------------------------------------
//
// This function checks against the frustum and sorts leafs front to back
void bsp_renderTree ( int Node, bool CheckFrustum )
//-----------------------------------------------------------------------------
{

	// Get a pointer to the node we are now in
	tBSPNode* pNode = &m_pNodes[Node];

	// if the node we came from is COMPLETE_IN then CheckFrustum will hold false and we
	// won't check this node against the frustum because we know we are COMPLETE_IN the
	// frustum
	if ( CheckFrustum )
	{
		// CheckFrustum = true so our node parent was intersecting the frustum and so
		// he have to continue checking this one
		switch ( sys_boxInFrustum ( ( float ) pNode->min.x, ( float ) pNode->min.y, ( float ) pNode->min.z, ( float ) pNode->max.x, ( float ) pNode->max.y, ( float ) pNode->max.z ) )
		{
			case COMPLETE_OUT:
				g_numLeafsNotDrawng++;
				return;
				// If this one is complete_out, stop analysing this branch
				// all leafs that come from this node are also complete_out

			case COMPLETE_IN:
				CheckFrustum = false;
				g_numLeafsDrawn++;
				break;
				// if we are complete_in, Set CheckFrustum to false to not
				// check frustum on any of the child nodes, CheckFrustum will be used
				// to call my children

				// if intersecting continue as usual, test all my children against the frustum
		}
	}

	// A node is an structure that hold a partition plane, all leafs are split in two
	// groups: FRONT and BACK. plane is the partition plane for this node
	const tBSPPlane* plane = &m_pPlanes[ pNode->plane ];

	// We will render leafs in a front to back order

	// If the camera position is in front the plane
	if ( ( plane->vNormal.x * g_cameraPosition.x + plane->vNormal.y * g_cameraPosition.y + plane->vNormal.z * g_cameraPosition.z -  plane->d  > 0 ) )
	{
		if ( pNode->front>=0 )  bsp_renderTree ( pNode->front, CheckFrustum );

		else bsp_renderLeaf ( ~pNode->front, CheckFrustum );

		// now that we render or leaf and nodes that are in front of the plane,
		// go with the BACK ones
		if ( pNode->back>=0 ) bsp_renderTree ( pNode->back, CheckFrustum );

		else bsp_renderLeaf ( ~pNode->back, CheckFrustum );
	}// else if is CameraPostion is back of the partition plane, draw back to front

	else
	{
		if ( pNode->back>=0 ) bsp_renderTree ( pNode->back, CheckFrustum );

		else bsp_renderLeaf ( ~pNode->back, CheckFrustum );

		if ( pNode->front>=0 )  bsp_renderTree ( pNode->front, CheckFrustum );

		else bsp_renderLeaf ( ~pNode->front, CheckFrustum );
	}
}

//-----------------------------------------------------------------------------
//
// Goes through all of the faces and draws them if doRender is true
// Otherise it prepares the face indexes and uploads the door vertices
void bsp_renderLevel ( const glm::vec3 &vPos, int whichShader, bool doRender )
//-----------------------------------------------------------------------------
{
	glm::vec3		drawAtModel;

	g_numFacesDrawn = 0;
	g_numFacesNotDrawn = 0;

	// Reset our bitset so all the slots are zero.
	m_FacesDrawn.ClearAll();

	sys_calculateFrustum();

	// CameraPosition is used as a global variable that can be accessed from renderTree
	// so, we haven't to pass it through the arguments, which slows down the performance
	// because of the stack overload

	g_cameraPosition = vPos;
	int leafIndex = bsp_findLeaf ( g_cameraPosition );
	g_currentCameraCluster = m_pLeafs[leafIndex].cluster;

	// Start adding faces to faceArray from the tree from the Root (Node 0 ) and start checking the frustum (true)
	bsp_renderTree ( 0, true );

	bsp_uploadDoorVertex ();

	bsp_addDoorFaces ();    //bind the buffer for vertex - not UNIFORM

	if (doRender)
		bsp_renderAllFaces(whichShader);

	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glBindVertexArray ( 0 );
}
