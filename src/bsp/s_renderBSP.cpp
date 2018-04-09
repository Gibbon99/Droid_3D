#include "s_globals.h"
#include "s_vsProfileLib.h"
#include "s_varsBSP.h"
#include "s_maths.h"
#include "s_shaders.h"
#include "io_textures.h"
#include "s_openGLWrap.h"
#include "s_doorsBSP.h"
#include <vector>

#include "s_shaderLights.h"
//#include "../s_openGL/s_lightPass.h"

GLuint          elementArrayID;

glm::vec3       g_cameraPosition;
int             g_currentCameraCluster;

int             vertIndexCounter = 0;

GLuint			bspVAO, bspVBO;

vector<unsigned int>				g_currentFrameVertex;
vector<_myVertex>					g_levelDataVertex;

//-----------------------------------------------------------------------------
//
// Upload all the vertex data in our own structure used to render the level
void bsp_uploadLevelVertex()
//-----------------------------------------------------------------------------
{
	_myVertex		tempVertex;

	for (int i = 0; i != m_numOfVerts; i++)
		{
			tempVertex.position = m_pVerts[i].vPosition;
			tempVertex.texCoords = m_pVerts[i].vTextureCoord;
			tempVertex.normals = m_pVerts[i].vNormal;

			g_levelDataVertex.push_back(tempVertex);
		}

	con_print(CON_INFO, true, "Upload vertices to GPU. Size [ %i bytes ]", g_levelDataVertex.size() * sizeof ( _myVertex ));

	//
	// Store model and all meshes in one VAO
	GL_CHECK ( glGenVertexArrays ( 1, &bspVAO ) );
	GL_CHECK ( glBindVertexArray ( bspVAO ) );

	GL_CHECK ( glGenBuffers (1, &bspVBO) );
	GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, bspVBO ));
	GL_CHECK ( glBufferData (GL_ARRAY_BUFFER, m_numOfVerts * sizeof ( _myVertex ), &g_levelDataVertex[0], GL_DYNAMIC_DRAW ) );
	//
	// Generate indexArray ID - indexes into currently visible faces in current frame
	GL_CHECK ( glGenBuffers ( 1, &elementArrayID ) );
}

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
// Predicate function used by std::sort
bool bsp_faceSorterCallback(_sortedFaces const& lhs, _sortedFaces const& rhs)
//-----------------------------------------------------------------------------
{
	return lhs.textureID < rhs.textureID;
}

//-----------------------------------------------------------------------------
//
// Sort the vector of vertex and TexID's into texID order
void bsp_sortFaceArray()
//-----------------------------------------------------------------------------
{

	std::sort(sortedFaces.begin(), sortedFaces.end(), &bsp_faceSorterCallback);

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
// Add the faces to the unsorted face array
void bsp_addFaceToArray ( int whichFace )
//-----------------------------------------------------------------------------
{
	_sortedFaces		tempFace;

	tempFace.textureID = m_pFaces[whichFace].textureID;
	tempFace.faceID = whichFace;

	sortedFaces.push_back(tempFace);

	sortCurrentFaceCount++;
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
					// bspRenderFace(faceIndex);
					bsp_addFaceToArray ( faceIndex );
					numFacesDrawn++;

				}

			else
				numFacesNotDrawn++;
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
						numLeafsNotDrawn++;
						return;

					// if this one is complete_out, stop analysing this branch
					// all leaf that come from this node are also complete_out

					case COMPLETE_IN:
						CheckFrustum = false;
						numLeafsDrawn++;
						break;
						// if we are complete_in, Set CheckFrustum to false to not
						// check frustum on any of my childs, CheckFrustum will be used
						// to call my children

						// if intersecting continue as usual, test all my children against the frustum
				}
		}

	// a node is an structure that hold a partition plane, all leaf are splited in two
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
// Draw the face array - can be sorted or in raw order from BSP
void bsp_drawFacesInArray ( int whichShader )
//-----------------------------------------------------------------------------
{
	tBSPFace		*ptrFace;
	tBSPFace		*ptrFaceNext;

	int stride = sizeof ( _myVertex ); // BSP Vertex, not float[3]
	unsigned int		currentTexID;


	GL_ASSERT ( glUseProgram ( shaderProgram[whichShader].programID ) );

	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr ( modelMatrix ) ) );

	GL_CHECK ( glBindVertexArray ( bspVAO ) );
	//
	// Bind data buffer holding all the vertices
	GL_CHECK ( glBindBuffer (GL_ARRAY_BUFFER, bspVBO ));
	//
	// Update vertex data stored on card for moving doors
	bspUploadDoorVertex();

	// Position
	GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
	GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, GL_FALSE, stride, ( offsetof ( _myVertex, position ) ) ) );

	// Normals
	GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );
	GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inNormalsID, 3, GL_FLOAT, GL_FALSE, stride, ( offsetof ( _myVertex, normals ) ) ) );

	// Texture coords
	GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
	GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, GL_FALSE, stride, ( offsetof ( _myVertex, texCoords ) ) ) );

	//
	// Enable attribute to hold vertex information
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );

	//
	// Upload vertex indexes
	GL_CHECK ( glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, elementArrayID ) );

	vertIndexCounter = 0;
	g_currentFrameVertex.clear();

	for (int i = 0; i != sortCurrentFaceCount; i++)
		con_print(CON_INFO, true, "[ %i ] faceID [ %i ] textureID [ %i ]", i, sortedFaces[i].faceID, sortedFaces[i].textureID);

	bsp_sortFaceArray();

con_print(CON_INFO, true, "After sorting....");

	for (int i = 0; i != sortCurrentFaceCount; i++)
		con_print(CON_INFO, true, "[ %i ] faceID [ %i ] textureID [ %i ]", i, sortedFaces[i].faceID, sortedFaces[i].textureID);

	//
	// Get first texture ID
	ptrFace = &m_pFaces[sortedFaces[0].faceID];
	currentTexID = sortedFaces[0].textureID;

con_print(CON_INFO, true, "START--------------------");

con_print(CON_INFO, true, "currentTexID [ %i ] faceID [ %i ]", currentTexID, sortedFaces[0].faceID);

	for ( int i = 0; i != sortCurrentFaceCount; i++ )
		{
			con_print(CON_INFO, true, "Count [ %i ] if [ %i ] == [ %i ]", i, currentTexID, sortedFaces[i].textureID);

			if (currentTexID == sortedFaces[i].textureID)
				{
					ptrFace = &m_pFaces[sortedFaces[i].faceID];

					con_print(CON_INFO, true, "Count [ %i ] Still the same texture - get all the verts", i);
					
					for ( int j = 0; j != ptrFace->numMeshVerts; j++ )
						{
							g_currentFrameVertex.push_back(m_pMeshIndex[ptrFace->startMeshVertIndex + j]);
							vertIndexCounter++;
						}
					con_print(CON_INFO, true, "Count [ %i ] Vert count is now [ %i ]", i, vertIndexCounter);
				}
			else
				{
					
					con_print(CON_INFO, true, "Count [ %i ] currentTexID [ %i ] is not equal to sortedFaces.textureID [ %i ]", i, currentTexID, sortedFaces[i].textureID);
					
					con_print(CON_INFO, true, "Cound [ %i ] Upload Vertexes [ %i ] to GPU", i, g_currentFrameVertex.size());
					
					GL_CHECK ( glBufferData ( GL_ELEMENT_ARRAY_BUFFER, g_currentFrameVertex.size() * sizeof(unsigned int), &g_currentFrameVertex[0], GL_STATIC_DRAW));
					wrapglBindTexture ( GL_TEXTURE0, io_getGLTexID(currentTexID) );
					GL_ASSERT ( glDrawElements ( GL_TRIANGLES, g_currentFrameVertex.size(), GL_UNSIGNED_INT, 0));
					currentTexID = sortedFaces[i].textureID;
					g_currentFrameVertex.clear(); // or use vertIndexCounter = 0;

con_print(CON_INFO, true, "Count [ %i ] currentTexID is now [ %i ]", i, currentTexID);

				}
		}

	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

	gl_getAllGLErrors ( glGetError(), __FILE__, __LINE__ );
}

//-----------------------------------------------------------------------------
//
//	Goes through all of the faces and draws them if the type is FACE_POLYGON
void bsp_renderLevel ( const glm::vec3 &vPos, int whichShader )
//-----------------------------------------------------------------------------
{
	glm::vec3		drawAtModel;

	sortCurrentFaceCount = 0;
	numFacesDrawn = 0;
	numFacesNotDrawn = 0;

	// Reset our bitset so all the slots are zero.
	m_FacesDrawn.ClearAll();
	sortedFaces.clear();

	sys_calculateFrustum();

	// CameraPosition is used as a global variable that can be accessed from renderTree
	// so, we haven't to pass it through the arguments, which slows down the performace
	// because of the stack overload

	g_cameraPosition = vPos;
	int leafIndex = bsp_findLeaf ( g_cameraPosition );
	g_currentCameraCluster = m_pLeafs[leafIndex].cluster;

	// Start adding faces to faceArray from the tree from the Root (Node 0 ) and start checking the frustum (true)
	bsp_renderTree ( 0, true );

	bsp_drawAllDoors();
	//
	// Draw all the sorted/unsorted faces - including visible models
	bsp_drawFacesInArray ( whichShader );
}
