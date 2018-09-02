#include "s_globals.h"
#include "s_assimp.h"
#include "s_shaders.h"
#include "s_openGLWrap.h"
#include "io_textures.h"
#include "s_lightPass.h"
#include "s_shadowMap.h"
#include "s_physics.h"
#include "s_maths.h"
#include <vector>
#include "s_defRender.h"
#include "s_camera3.h"

//#include <ProcessHelper.h>
//class 				id_textures;
Assimp::Importer	importer;
int					numSkippedModels = 0;

enum
{
	VERTEX_BUFFER,
	TEXCOORD_BUFFER,
	NORMAL_BUFFER,
	INDEX_BUFFER
};

_meshModel				meshModels[NUM_MODELS];

GLuint 					vbo[4];

int						elementCount = 0;
bool					g_debugBoundingBox;

//-----------------------------------------------------------------------------
//
// Load all the textures for each of the models
void ass_loadModelTextures()
//-----------------------------------------------------------------------------
{
	for ( int i = 0; i != NUM_MODELS; i++ )
		{
			int j = 0;	// Only load first material
//			for ( int j = 0; j != meshModels[i].numMaterials; j++ )
			{
				if (meshModels[i].materialName[j].C_Str() == "white_square")    // TODO: Fix this up to match properly
					meshModels[i].mesh[j].textureID = checkerBoardTexture;
				else
					meshModels[i].mesh[j].textureID = utilLoadTexture ( meshModels[i].materialName[j].C_Str(), -1 );
				con_print ( CON_INFO, true, "Model [ %i ] : Texture ID [ %i ]", i, meshModels[i].mesh[j].textureID );
			}
		}
}

//-----------------------------------------------------------------------------
//
// Render the mesh using a transform/rotation/scale matrix from bullet
void ass_renderMeshMat4 ( int whichModel, int whichShader, glm::mat4 physicsMatrix, GLfloat scaleBy, glm::vec3 lightColor )
//-----------------------------------------------------------------------------
{
	glm::vec4       minSize;
	glm::vec4       maxSize;

	if ( false == meshModels[whichModel].loaded )
		return;

	for (int whichMesh = 0; whichMesh != meshModels[whichModel].numMeshes; whichMesh++)
		{
			physicsMatrix = glm::scale ( physicsMatrix, glm::vec3 ( scaleBy, scaleBy, scaleBy ) );
			//
			// Translate model bounding box for testing against frustrum
			minSize = physicsMatrix * glm::vec4 ( meshModels[whichModel].mesh[whichMesh].boundingBox.minSize, 1.0 );
			maxSize = physicsMatrix * glm::vec4 ( meshModels[whichModel].mesh[whichMesh].boundingBox.maxSize, 1.0 );

			if ( COMPLETE_OUT == sys_boxInFrustum ( minSize.x, minSize.y, minSize.z, maxSize.x, maxSize.y, maxSize.z ) )
				{
					numSkippedModels++;
					return;
				}

			GL_ASSERT ( glBindVertexArray ( meshModels[whichModel].mesh[whichMesh].vao_ID ) );
			GL_ASSERT ( glUniformMatrix4fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "model" ), 1, false, glm::value_ptr ( physicsMatrix ) ) );

			if ( MODEL_SPHERE == whichModel )
				{
					GL_ASSERT ( glUniform1f ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uLightRadius" ), scaleBy ) );
					GL_ASSERT ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uLightColor" ), 1, glm::value_ptr ( lightColor ) ) );

					GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uPositionTex" ), 0 ) );
					wrapglBindTexture ( GL_TEXTURE0 + 0, id_textures[GBUFFER_TEXTURE_TYPE_POSITION] );

					GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uNormalTex" ), 1 ) );
					wrapglBindTexture ( GL_TEXTURE0 + 1, id_textures[GBUFFER_TEXTURE_TYPE_NORMAL] );

					GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uDiffuseTex" ), 2 ) );
					wrapglBindTexture ( GL_TEXTURE0 + 2, id_textures[GBUFFER_TEXTURE_TYPE_DIFFUSE] );

					GL_CHECK ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "cameraPosition" ), 1, glm::value_ptr ( cam3_Position ) ) );
				}

			else
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, checkerBoardTexture);

//					wrapglBindTexture ( GL_TEXTURE0, checkerBoardTexture); //[whichModel].mesh[whichMesh].textureID );
//					glBindTexture ( GL_TEXTURE0, meshModels[whichModel].mesh[whichMesh].textureID );
					GL_ASSERT ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "ambientColor" ), 1, glm::value_ptr ( lightColor ) ) );
				}

			//
			// Always use vertex information
			GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[VERTEX_BUFFER] ) );
			GL_ASSERT ( glEnableVertexAttribArray ( 0) );
			GL_ASSERT ( glVertexAttribPointer ( 0, 3, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );

			if ( meshModels[whichModel].hasNormals )
				{
					//
					// Use Normal information
					GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[NORMAL_BUFFER] ) );
					GL_ASSERT ( glEnableVertexAttribArray ( 1) );
					GL_ASSERT ( glVertexAttribPointer ( 1, 3, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
				}

			if ( meshModels[whichModel].hasTextures )
				{
					//
					// Use Texture coordinate information
					GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[TEXCOORD_BUFFER] ) );
					GL_ASSERT ( glEnableVertexAttribArray ( 2 ) );
					GL_ASSERT ( glVertexAttribPointer ( 2, 2, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
				}

			GL_ASSERT ( glDrawElements ( GL_TRIANGLES, meshModels[whichModel].mesh[whichMesh].elementCount, GL_UNSIGNED_INT, nullptr) );
		}

	glBindVertexArray ( 0 );
//	glUseProgram ( 0 );
}

//-----------------------------------------------------------------------------
//
// Render the mesh using just a GLM::VEC3 position
//
// Pass in -1.0f to scaleBy to use setting from model loading
//
void ass_renderMeshVec3Position ( int whichModel, int whichShader, glm::vec3 pos, GLfloat scaleBy, glm::vec3 lightColor )
//-----------------------------------------------------------------------------
{
#define FRUSTUM_PADDING 50.0f

	static bool disabledWarning = false;

	glm::vec4       minSize;
	glm::vec4       maxSize;
	glm::mat4		scaleMatrix;

	if ( !meshModels[whichModel].loaded )
		return;

	for (int whichMesh = 0; whichMesh != meshModels[whichModel].numMeshes; whichMesh++)
		{
			scaleMatrix = glm::mat4();
			modelMatrix = glm::mat4();
			//
			// Adjust the size and position of the mesh
			scaleMatrix = glm::translate ( glm::mat4(), pos );
			scaleMatrix = glm::scale ( scaleMatrix, glm::vec3 ( scaleBy, scaleBy, scaleBy ) );
			//
			// Translate model bounding box for testing against frustrum
			minSize = scaleMatrix * glm::vec4 ( meshModels[whichModel].mesh[whichMesh].boundingBox.minSize, 1.0 );
			maxSize = scaleMatrix * glm::vec4 ( meshModels[whichModel].mesh[whichMesh].boundingBox.maxSize, 1.0 );

			if ( COMPLETE_OUT == sys_boxInFrustum ( minSize.x - FRUSTUM_PADDING, minSize.y - FRUSTUM_PADDING, minSize.z - FRUSTUM_PADDING,
			                                        maxSize.x + FRUSTUM_PADDING, maxSize.y + FRUSTUM_PADDING, maxSize.z + FRUSTUM_PADDING) )
				{
					numSkippedModels++;
					return;
				}

			GL_ASSERT ( glBindVertexArray ( meshModels[whichModel].mesh[whichMesh].vao_ID ) );
			GL_ASSERT ( glUseProgram ( shaderProgram[whichShader].programID ) );

			GL_ASSERT ( glUniformMatrix4fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "u_viewProjectionMat" ), 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );
			GL_ASSERT ( glUniformMatrix4fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "u_modelMat" ), 1, false, glm::value_ptr ( scaleMatrix ) ) );
//			GL_ASSERT ( glUniformMatrix3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "u_normalMatrix" ), 1, false, glm::value_ptr ( glm::inverseTranspose ( glm::mat3 ( scaleMatrix ) ) ) ) );

			if ( MODEL_SPHERE == whichModel )
				{
					GL_ASSERT ( glUniform1f  ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uLightRadius" ), scaleBy ) );
					GL_ASSERT ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uLightColor" ), 1, glm::value_ptr ( lightColor ) ) );
					GL_ASSERT ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uLightPosition" ), 1, glm::value_ptr ( pos ) ) );

					GL_CHECK ( glUniform3fv  ( glGetUniformLocation ( shaderProgram[whichShader].programID, "cameraPosition" ), 1, glm::value_ptr ( cam3_Position ) ) );
					
					GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uPositionTex" ), 0 ) );
					wrapglBindTexture ( GL_TEXTURE0 + 0, id_textures[GBUFFER_TEXTURE_TYPE_POSITION] );

					GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uNormalTex" ), 1 ) );
					wrapglBindTexture ( GL_TEXTURE0 + 1, id_textures[GBUFFER_TEXTURE_TYPE_NORMAL] );

					GL_CHECK ( glUniform1i ( glGetUniformLocation ( shaderProgram[whichShader].programID, "uDiffuseTex" ), 2 ) );
					wrapglBindTexture ( GL_TEXTURE0 + 2, id_textures[GBUFFER_TEXTURE_TYPE_DIFFUSE] );
				}

			if ( MODEL_CRATE == whichModel)
				{
					wrapglBindTexture ( GL_TEXTURE0, meshModels[whichModel].mesh[whichMesh].textureID );
					GL_CHECK ( glUniform1i ( shaderProgram[whichShader].inTextureUnit, 0 ) );
					GL_ASSERT ( glUniform3fv ( glGetUniformLocation ( shaderProgram[whichShader].programID, "ambientColor" ), 1, glm::value_ptr ( lightColor ) ) );
				}				

			//
			// Always use vertex information
			GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[VERTEX_BUFFER] ) );
			GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID ) );
			GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );

			if (!disabledWarning)
			{
				con_print(CON_INFO, true, "WARNING: Normals and textures disabled for drawing models [ %s ]", __func__);
				disabledWarning = true;
			}

			/*
			if ( meshModels[whichModel].hasNormals == true )
				{
					//
					// Use Normal information
					GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[NORMAL_BUFFER] ) );
					GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inNormalsID ) );
					GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inNormalsID, 3, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );
				}


			if ( meshModels[whichModel].hasTextures == true )
				{
					//
					// Use Texture coordinate information
					GL_ASSERT ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[TEXCOORD_BUFFER] ) );
					GL_ASSERT ( glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID ) );
					GL_ASSERT ( glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET ( 0 ) ) );

					wrapglBindTexture ( GL_TEXTURE0, meshModels[whichModel].mesh[whichMesh].textureID );
				}
*/
			GL_ASSERT ( glDrawElements ( GL_TRIANGLES, meshModels[whichModel].mesh[whichMesh].elementCount, GL_UNSIGNED_INT, NULL ) );
		}

	glBindVertexArray ( 0 );
	glUseProgram ( 0 );
}


//-----------------------------------------------------------------------------
//
// Get all the information out of the mesh and upload to GPU
void ass_uploadMesh ( aiMesh *mesh, int whichModel, int whichMesh )
//-----------------------------------------------------------------------------
{
	_boundingBox    tempBox;
	glm::vec3       tempVec3;

	aiVector3D      boxMin;
	aiVector3D      boxMax;
	aiVector3D      boxCenter;

	tempBox.minSize = glm::vec3 ( 1000 );
	tempBox.maxSize = glm::vec3 ( -1000 );

	meshModels[whichModel].mesh[whichMesh].elementCount = mesh->mNumFaces * 3;
	//
	// Create physics ConvexHull from mesh data
	meshModels[whichModel].mesh[whichMesh].physicsShapeConvexHull = new btConvexHullShape();

	if ( mesh->HasPositions() )
		{
			float *vertices = new float[mesh->mNumVertices * 3];

			for ( uint i = 0; i < mesh->mNumVertices; ++i )
				{
					vertices[i * 3] = mesh->mVertices[i].x;
					vertices[i * 3 + 1] = mesh->mVertices[i].y;
					vertices[i * 3 + 2] = mesh->mVertices[i].z;

					btVector3 tempVector = btVector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

					meshModels[whichModel].mesh[whichMesh].physicsShapeConvexHull->addPoint(tempVector);

					tempVec3.x = mesh->mVertices[i].x;
					tempVec3.y = mesh->mVertices[i].y;
					tempVec3.z = mesh->mVertices[i].z;

					//
					// Get smallest point
					if ( tempBox.minSize.x > tempVec3.x )
						tempBox.minSize.x = tempVec3.x;

					if ( tempBox.minSize.y > tempVec3.y )
						tempBox.minSize.y = tempVec3.y;

					if ( tempBox.minSize.z > tempVec3.z )
						tempBox.minSize.z = tempVec3.z;

					//
					// Get the largest point
					if ( tempBox.maxSize.x < tempVec3.x )
						tempBox.maxSize.x = tempVec3.x;

					if ( tempBox.maxSize.y < tempVec3.y )
						tempBox.maxSize.y = tempVec3.y;

					if ( tempBox.maxSize.z < tempVec3.z )
						tempBox.maxSize.z = tempVec3.z;
				}

//        Assimp::FindMeshCenter (mesh, boxCenter, boxMin, boxMax );

			meshModels[whichModel].mesh[whichMesh].boundingBox = tempBox;
			/*
			      btShapeHull* HullOfOriginalShape = new btShapeHull(NewStaticMesh);
			      btScalar CurrentMargin = NewStaticMesh->getMargin();
			      HullOfOriginalShape->buildHull(CurrentMargin);
			      ReducedPolygonStaticMesh = new btConvexHullShape();

			      for (int i = 0; i < HullOfOriginalShape->numVertices(); i++) {
			         ReducedPolygonStaticMesh->addPoint(HullOfOriginalShape->getVertexPointer()[i], false);
			      }

			      ReducedPolygonStaticMesh->recalcLocalAabb();
			*/
			con_print ( CON_INFO, true, "Bounding box size:" );
			con_print ( CON_INFO, true, "Min [ %3.3f %3.3f %3.3f ]", meshModels[whichModel].mesh[whichMesh].boundingBox.minSize.x, meshModels[whichModel].mesh[whichMesh].boundingBox.minSize.y, meshModels[whichModel].mesh[whichMesh].boundingBox.minSize.z );
			con_print ( CON_INFO, true, "Max [ %3.3f %3.3f %3.3f ]", meshModels[whichModel].mesh[whichMesh].boundingBox.maxSize.x, meshModels[whichModel].mesh[whichMesh].boundingBox.maxSize.y, meshModels[whichModel].mesh[whichMesh].boundingBox.maxSize.z );

			//GL_CHECK ( glGenBuffers ( 1, &meshModels[whichModel].mesh[whichMesh].vbo[VERTEX_BUFFER] ) );
			meshModels[whichModel].mesh[whichMesh].vbo[VERTEX_BUFFER] = wrapglGenBuffers(1, __func__);
			GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[VERTEX_BUFFER] ) );
			GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof ( GLfloat ), vertices, GL_STATIC_DRAW ) );

			delete vertices;
		}

	meshModels[whichModel].hasTextures = false;

	if ( mesh->HasTextureCoords ( 0 ) )
		{
			float *texCoords = new float[mesh->mNumVertices * 2];

			for ( unsigned int i = 0; i < mesh->mNumVertices; ++i )
				{
					texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
					texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
				}

			//GL_CHECK ( glGenBuffers ( 1, &meshModels[whichModel].mesh[whichMesh].vbo[TEXCOORD_BUFFER] ) );
			meshModels[whichModel].mesh[whichMesh].vbo[TEXCOORD_BUFFER] = wrapglGenBuffers(1, __func__);
			GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[TEXCOORD_BUFFER] ) );
			GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof ( GLfloat ), texCoords, GL_STATIC_DRAW ) );

			meshModels[whichModel].hasTextures = true;
			delete texCoords;
		}

	meshModels[whichModel].hasNormals = false;

	if ( mesh->HasNormals() )
		{

			float *normals = new float[mesh->mNumVertices * 3];

			for ( unsigned int i = 0; i < mesh->mNumVertices; ++i )
				{
					normals[i * 3] = mesh->mNormals[i].x;
					normals[i * 3 + 1] = mesh->mNormals[i].y;
					normals[i * 3 + 2] = mesh->mNormals[i].z;
				}

			//GL_CHECK ( glGenBuffers ( 1, &meshModels[whichModel].mesh[whichMesh].vbo[NORMAL_BUFFER] ) );
			meshModels[whichModel].mesh[whichMesh].vbo[NORMAL_BUFFER] = wrapglGenBuffers(1, __func__);
			GL_CHECK ( glBindBuffer ( GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[NORMAL_BUFFER] ) );
			GL_CHECK ( glBufferData ( GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof ( GLfloat ), normals, GL_STATIC_DRAW ) );

			meshModels[whichModel].hasNormals = true;
			delete normals;
		}

	if ( mesh->HasFaces() )
		{
			unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];

			for ( unsigned int i = 0; i < mesh->mNumFaces; ++i )
				{
					indices[i * 3] = mesh->mFaces[i].mIndices[0];
					indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
					indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
				}

			//GL_CHECK ( glGenBuffers ( 1, &meshModels[whichModel].mesh[whichMesh].vbo[INDEX_BUFFER] ) );
			meshModels[whichModel].mesh[whichMesh].vbo[INDEX_BUFFER] = wrapglGenBuffers(1, __func__);
			GL_CHECK ( glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[INDEX_BUFFER] ) );
			GL_CHECK ( glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof ( GLuint ), indices, GL_STATIC_DRAW ) );

			delete indices;
		}

	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
}

//-----------------------------------------------------------------------------
//
// Get material information - textures for model
bool ass_getMaterials ( aiScene *scene, int whichModel )
//-----------------------------------------------------------------------------
{
	aiString    texturePath;

	for ( uint i = 0; i < scene->mNumMaterials; i++ )
		{
			aiMaterial*     pMaterial = scene->mMaterials[i];

			if ( AI_SUCCESS == pMaterial->GetTexture ( aiTextureType_DIFFUSE, 0, &texturePath, NULL, NULL, NULL, NULL, NULL ) )
				{
					texturePath.data[strlen ( texturePath.data ) - 4] = '\0';
					meshModels[whichModel].materialName[i] = texturePath.data;
					con_print ( CON_INFO, true, "Model [ %i ] Material [ %i ] : [ %s ]", whichModel, i, meshModels[whichModel].materialName[i].C_Str() );
					return true;
				}

			else
				{
					meshModels[whichModel].materialName[i] = "white_square";
					con_print ( CON_INFO, true, "Model [ %i ] Material [ %i ] : [ %s ]", whichModel, i, meshModels[whichModel].materialName[i].C_Str() );
					return false;
				}
		}

	return false;
}

//-----------------------------------------------------------------------------
//
// Load a asset model for rendering
//bool ass_loadModel(int whichModel, const char *fileName, float scaleFactor)
bool ass_loadModel ( int whichModel, std::string fileName )
//-----------------------------------------------------------------------------
{
	// aiProcess_GenNormals
	// aiProcess_GenSmoothNormals

	//
	// Don't draw it later if it's not loaded
	meshModels[whichModel].loaded = false;

	importer.SetPropertyInteger ( AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS );

	aiScene *scene = (aiScene *)importer.ReadFile ( fileName, aiProcess_RemoveComponent | aiProcess_Triangulate |  aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if ( !scene )
		{
			con_print ( CON_ERROR, true, "Error: Could not load mesh [ %s - %s ]", fileName.c_str(), importer.GetErrorString() );
			return false;
		}

	meshModels[whichModel].loaded = true;

	meshModels[whichModel].numMeshes = scene->mNumMeshes;
	meshModels[whichModel].numMaterials = scene->mNumMaterials;
	//
	// Allocate memory to hold material filenames and meshes for the model
	meshModels[whichModel].materialName.reserve ( meshModels[whichModel].numMaterials );
	meshModels[whichModel].mesh.reserve ( meshModels[whichModel].numMeshes );

//	con_print ( CON_INFO, true, "Mesh [ %i ] : [ %s ] numMeshes [ %i ] numMaterials [ %i ]", whichModel, fileName.c_str(), meshModels[whichModel].numMeshes, meshModels[whichModel].numMaterials );

	for ( unsigned int i = 0; i < scene->mNumMeshes; ++i )
		{
			//
			// Store each mesh in the model in it's own VAO
			glGenVertexArrays ( 1, &meshModels[whichModel].mesh[i].vao_ID );
			glBindVertexArray ( meshModels[whichModel].mesh[i].vao_ID );

			ass_uploadMesh ( scene->mMeshes[i], whichModel, i );
			meshModels[whichModel].mesh[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
			meshModels[whichModel].mesh[i].numFaces = scene->mMeshes[i]->mNumFaces;
			meshModels[whichModel].mesh[i].numVertices = scene->mMeshes[i]->mNumVertices;
//			con_print ( CON_INFO, true, "Mesh [ %i ] : Material Index [ %i ] numFaces [ %i ] numVertices [ %i ]", i, meshModels[whichModel].mesh[i].materialIndex, meshModels[whichModel].mesh[i].numFaces, meshModels[whichModel].mesh[i].numVertices );
		}

	ass_getMaterials ( scene, whichModel );
	//
	// Finished with this VAO now
	glBindVertexArray ( 0 );

	return true;
}
