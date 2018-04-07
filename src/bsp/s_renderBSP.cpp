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

GLuint          bspVAO_ID;
GLuint          bspVBO_VERT;

GLuint          elementArrayID;

glm::vec3       g_cameraPosition;
int             g_currentCameraCluster;

int             vertIndexCounter = 0;

GLuint          vao;
GLuint          vbo_ID;

typedef struct
{
	glm::vec3   position;
	glm::vec2   texCoords;
	glm::vec2   lightmapCoords;
	glm::vec3   normals;
} _myVertex;

typedef struct
{
	int                 vertIndex;      // Index into vertex array - starting point
	int                 numVerts;
	int                 textureID;
	int                 lightmapID;
} _myFace;

vector<_myVertex>       vertex;
vector<_myFace>         faces;
vector<int>             arrayIndex;

int stride = sizeof ( _myVertex );

/*

Create seperate arrays

1 - by shader
2 - by texture
3 - by lightmap

texture     lightmap
1           0
2           0
2           1
1           0
1           1
3           1

Shader #1 -> texture 0 -> lightmap 0
Shader #1 -> texture 0 -> lightmap 1
Shader #1 -> texture 1 -> lightmap 0
Shader #1 -> texture 2 -> lightmap 0
Shader #1 -> texture 3 -> lightmap 1

typedef struct
{
    int textureID;
    int lightMapID;
    int arrayIndex;
    int numVerts;
    <vector>int     arrayIndexes;   // point to each entry in face array for this frame

} _renderQueueFace;

typedef struct
{
    int numEntries;
} _renderQueue;


_renderQueue    renderQueue[NUM_SHADERS];

renderQueue[SHADER_ONE].numEntries;

renderQueue[SHADER_ONE].queueFace[TEX_ID_0].numFaces;
renderQueue[SHADER_ONE].queueFace[TEX_ID_0].numVerts;
renderQueue[SHADER_ONE].queueFace[TEX_ID_0].textureID;
renderQueue[SHADER_ONE].queueFace[TEX_ID_0].lightMapID;
renderQueue[SHADER_ONE].queueFace[TEX_ID_0].arrayIndexs;   // index into faces array containing vertex, UV info


Render

for ( each shader )

map texID to index array ?

if new texID, add new index array??

when adding new face, compare texID to existing list, and get matching index array;


    When adding face from BSP walk

    addFace(whichShader, textureID, lightMapID
*/

//-----------------------------------------------------------------------------
// Walk the vertex array and find the matching sets for lightmapID's
//
// Create an index for those faces and draw with glDrawElements pointing to the index array
void bsp_processLightMapFaces ( int whichShader )
//-----------------------------------------------------------------------------
{
	int indexCounter = 0;
	int matchCounter = 0;
	int vertCounter = 0;
	int checkForID = 0;

	std::vector<_myFace>::const_iterator j;

	printf ( "There are [ %i ] lightmaps\n", m_numOfLightmaps );

	for ( int i = 0; i != m_numOfLightmaps; i++ )
		{
			arrayIndex.clear();
			indexCounter = 0;
			matchCounter = 0;
			vertCounter = 0;

			for ( j = faces.begin(); j != faces.end(); ++j )
				{
					checkForID = faces[indexCounter].lightmapID;    // Which ID are we looking for
					// Do a sort by lightMapID first??
//            printf("Pass [ %i ] index [ %i ] Check [ %i ] vs [ %i ]\n", i, checkForID,  m_lightmaps[checkForID], m_lightmaps[i]);

					if ( m_lightmaps[checkForID] == m_lightmaps[i] )
						{
							// Add it's index to the array
							arrayIndex.push_back ( faces[indexCounter].vertIndex - faces[indexCounter].numVerts );
							vertCounter += faces[indexCounter].numVerts;
							matchCounter++;

							printf ( "Adding index [ %i ] to array\n", faces[indexCounter].vertIndex );
						}

					indexCounter++;
				}

			printf ( "There are [ %i ] faces with lightmapID [ %i ] numVerts [ %i ]\n", matchCounter, m_lightmaps[i], vertCounter );

			if ( matchCounter > 0 )
				{
					// Finished finding all the first lightmap ID's - so now draw them
//            GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayID));
//            GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, matchCounter * sizeof(int), arrayIndex[0], GL_STATIC_DRAW));


					GL_CHECK ( glBindVertexArray ( vao ) );
					GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, vbo_ID ) );

					GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, vertIndexCounter * sizeof ( _myVertex ), &vertex[0], GL_STATIC_DRAW ) );
					//
					// position
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
					GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inVertsID,3, GL_FLOAT,GL_FALSE, stride, offsetof ( _myVertex, position ) ) );
					//
					// Texture coordinates
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
					GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, stride, offsetof ( _myVertex, texCoords ) ) );
					//
					// Lightmap coordinates
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID_1 ) );
					GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID_1, 2, GL_FLOAT, false, stride, offsetof ( _myVertex, lightmapCoords ) ) );
					//
					// Normals
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );
					GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inNormalsID, 3, GL_FLOAT, false, stride, offsetof ( _myVertex, normals ) ) );

					//
					// Enable attribute to hold vertex information
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID_1 ) );
					GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );

					//wrapglBindTexture(GL_TEXTURE1, texturesLoaded[TEX_WALL].texID);
					wrapglBindTexture ( GL_TEXTURE1, m_lightmaps[i] );
					GL_ASSERT ( glUniform1i ( shaderProgram[whichShader].inTextureUnit_1, 1 ) );

					for ( int i = 0; i != matchCounter; i++ )
						{
							glDrawArrays ( GL_TRIANGLES, arrayIndex[i], faces[arrayIndex[i]].numVerts );
//                GL_CHECK(glDrawElements(GL_TRIANGLES, vertCounter, GL_UNSIGNED_INT, &arrayIndex));
						}
				}
		}
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


			con_print ( CON_INFO, true, "numVerts [ %i ]", faces[sortedFaces[index].faceID].numVerts );

			for ( int i = 0; i != faces[sortedFaces[index].faceID].numVerts; i++ )
				{
					con_print ( CON_INFO, true, "VertexPos [ %i ] [ %3.3f %3.3f %3.3f ]", faces[i].vertIndex,
					            vertex[faces[i].vertIndex + i].position.x,
					            vertex[faces[i].vertIndex + i].position.y,
					            vertex[faces[i].vertIndex + i].position.z );
				}

			debugDone = true;
		}
}

//-----------------------------------------------------------------------------
//
// Create array for all the visible faces this frame
void bsp_createFaceGPUInfo ( tBSPFace *ptrFace )
//-----------------------------------------------------------------------------
{
	vector<int>     indexes;
	_myVertex       tempVertex;
	_myFace         tempFace;

	indexes.clear();
	indexes.reserve ( ptrFace->numMeshVerts );

	for ( int i = 0; i != ptrFace->numMeshVerts; i++ )
		{
			indexes[i] = m_pMeshIndex[ptrFace->startMeshVertIndex + i];

			tempVertex.position =       m_pVerts[ptrFace->startVertIndex + indexes[i]].vPosition;
			tempVertex.texCoords =      m_pVerts[ptrFace->startVertIndex + indexes[i]].vTextureCoord;
			tempVertex.lightmapCoords = m_pVerts[ptrFace->startVertIndex + indexes[i]].vLightmapCoord;
			tempVertex.normals =        m_pVerts[ptrFace->startVertIndex + indexes[i]].vNormal;

			vertex.push_back ( tempVertex );
		}

	vertIndexCounter += ptrFace->numMeshVerts;  // Starting index

	tempFace.numVerts = ptrFace->numMeshVerts;
	tempFace.vertIndex = vertIndexCounter;      // Starting point when rendering multiple textures
	tempFace.textureID = ptrFace->textureID;
	tempFace.lightmapID = ptrFace->lightmapID;

	faces.push_back ( tempFace );
}

//-----------------------------------------------------------------------------
//
// Actually draw the BSP face
void bsp_renderAllFaces ( tBSPFace *ptrFace, int whichShader )
//-----------------------------------------------------------------------------
{

//	PROFILE("BSP");

#define SLOW_WAY 1

#ifdef SLOW_WAY

	// create the VAO

	GL_CHECK ( glBindVertexArray ( vao ) );
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, vbo_ID ) );

	GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, vertIndexCounter * sizeof ( _myVertex ), &vertex[0], GL_STATIC_DRAW ) );
	//
	// position
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inVertsID,3, GL_FLOAT,GL_FALSE, stride, offsetof ( _myVertex, position ) ) );
	// Normals
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inNormalsID, 3, GL_FLOAT, false, stride, offsetof ( _myVertex, normals ) ) );
	//
	// Texture coordinates
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
	GL_CHECK ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, stride, offsetof ( _myVertex, texCoords ) ) );
	//
	// Lightmap coordinates
//	GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID_1));
//	GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inTextureCoordsID_1, 2, GL_FLOAT, false, stride, offsetof(_myVertex, lightmapCoords)));
	//

	//
	// Enable attribute to hold vertex information
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );
	GL_CHECK ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
//	GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID_1));

//	wrapglBindTexture(GL_TEXTURE1, m_lightmaps[0]);
//	GL_ASSERT(glUniform1i(shaderProgram[whichShader].inTextureUnit_1, 1));

//    GL_CHECK(glDrawElements(GL_TRIANGLES, ptrFace->numMeshVerts, GL_UNSIGNED_INT, 0));

	glDrawArrays ( GL_TRIANGLES, 0, vertIndexCounter );

#else

	int stride = sizeof ( tBSPVertex ); // BSP Vertex, not float[3]

	GL_CHECK ( glUseProgram ( shaderProgram[whichShader].programID ) );

	GL_CHECK ( glBindVertexArray ( bspVAO_ID ) );

	GL_CHECK ( glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, elementArrayID ) );
	GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, bspVBO_VERT ) );

	// Position
	GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
	GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, false, stride, ( offsetof ( tBSPVertex, vPosition ) ) ) );

	// Normals
	GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );
	GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inNormalsID, 3, GL_FLOAT, false, stride, offsetof ( tBSPVertex, vNormal ) ) );

	// Texture coords
	GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
	GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, stride, offsetof ( tBSPVertex, vTextureCoord ) ) );

	// Lightmap coords - not used in shader yet
//	GL_ASSERT(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID_1));
//	GL_ASSERT(glVertexAttribPointer(shaderProgram[whichShader].inTextureCoordsID_1, 2, GL_FLOAT, false, stride, offsetof(tBSPVertex, vLightmapCoord)));


//	GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr(modelMatrix)));
//	GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr(projMatrix * viewMatrix)));

	GL_ASSERT ( glDrawElementsBaseVertex ( GL_TRIANGLES, ptrFace->numMeshVerts, GL_UNSIGNED_INT, ( const GLvoid * ) ptrFace->startMeshVertIndex, ptrFace->startVertIndex ) );

	glBindBuffer ( GL_ARRAY_BUFFER, 0 );

	gl_getAllGLErrors ( glGetError(), __FILE__, __LINE__ );

#endif
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
	if ( !m_clusters.pBitsets || current < 0 ) return 1;

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
void bsp_addFaceToArray ( int whichFace, int leafIndex )
//-----------------------------------------------------------------------------
{

	sortedFaces[sortCurrentFaceCount].textureID = m_pFaces[whichFace].textureID;
	sortedFaces[sortCurrentFaceCount].faceID = whichFace;
	sortedFaces[sortCurrentFaceCount].leafIndex = leafIndex;
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
//			conPrint (GL_TRUE, "Found non polygon face [ %i ]", m_pFaces[faceIndex].type);
					continue;
				}

			// Since many faces are duplicated in other leafs, we need to
			// make sure this face already hasn't been drawn.
			if ( !m_FacesDrawn.On ( faceIndex ) )
				{
					// Set this face as drawn and render it
					m_FacesDrawn.Set ( faceIndex );
					// bspRenderFace(faceIndex);
					bsp_addFaceToArray ( faceIndex, iLeaf );
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

//	conPrint(true, "Info: Node value [ %i ]", Node);

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
	glm::vec3       pos;
	glm::mat4       scaleMatrix;

	//
	// create buffers for our vertex data
	GL_ASSERT ( glGenBuffers ( 1, &vbo_ID ) );
	GL_ASSERT ( glGenVertexArrays ( 1, &vao ) );

	//
	// Generate indexArray ID
	GL_CHECK ( glGenBuffers ( 1, &elementArrayID ) );

	GL_ASSERT ( glUseProgram ( shaderProgram[whichShader].programID ) );

	wrapglBindTexture ( GL_TEXTURE0, texturesLoaded[TEX_WALL].texID );

	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr ( modelMatrix ) ) );

	vertIndexCounter = 0;
	vertex.clear();
	faces.clear();

	for ( int i = 0; i != sortCurrentFaceCount; i++ )
		{
			ptrFace = &m_pFaces[sortedFaces[i].faceID];

			bsp_createFaceGPUInfo ( ptrFace );
//        bspTextureSortOrNot(ptrFace);

//        doLightmapsOrNot(ptrFace);
		}

	bsp_renderAllFaces ( ptrFace, whichShader );
//  bsp_processLightMapFaces(whichShader);
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

	sys_calculateFrustum();

	// CameraPosition is used as a global variable that can be accessed from renderTree
	// so, we haven't to pass it through the arguments, which slows down the performace
	// because of the stack overload

	g_cameraPosition = vPos;
	int leafIndex = bsp_findLeaf ( g_cameraPosition );
	g_currentCameraCluster = m_pLeafs[leafIndex].cluster;

	// Start adding faces to faceArray from the tree from the Root (Node 0 ) and start checking the frustum (true)
	bsp_renderTree ( 0, true );

	// Sort faces and timing here


	bsp_drawAllDoors();


	//
	// Draw all the sorted/unsorted faces - including visible models
	bsp_drawFacesInArray ( whichShader );



	/*
	if (GL_TRUE == renderModels)
		bspRenderModels();		// Puts models faces into the faceArray for sorting
	else
		m_numOfModelsSkipped = m_numOfModels;
	*/
}
