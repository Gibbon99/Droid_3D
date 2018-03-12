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

//#include <ProcessHelper.h>
class id_textures;
Assimp::Importer    importer;
int     numSkippedModels = 0;

enum
{
	VERTEX_BUFFER,
	TEXCOORD_BUFFER,
	NORMAL_BUFFER,
	INDEX_BUFFER
};

typedef struct
{
	int         materialIndex;
	int         elementCount;
	int         numFaces;
	int         numVertices;
	GLuint      textureID;
	GLuint      vbo[4];
} _mesh;

typedef struct
{
	bool                loaded;
	bool				hasNormals;
	bool				hasTextures;
	int                 numMeshes;
	int                 numMaterials;
	GLuint              vao_ID;
	GLfloat             scaleFactor;
	_boundingBox        boundingBox;
//    int                 numFaces;
//    int                 numVertices;
	vector<_mesh>       mesh;
	vector<aiString>    materialName;
} _meshModel;

_meshModel              meshModels[NUM_MODELS];

GLuint vbo[4];

int      elementCount = 0;
bool     g_debugBoundingBox;

//-----------------------------------------------------------------------------
//
// Load all the textures for each of the models
void ass_loadModelTextures()
//-----------------------------------------------------------------------------
{
	int numTextures = 0;

	for (int i = 0; i != NUM_MODELS; i++)
		numTextures += meshModels[i].numMaterials;

	//
	// Allocate memory for texture structure
	io_setupModelTextureMemory(numTextures);
	//
	// Load each of the textures
	numTextures = 0;

	for (int i = 0; i != NUM_MODELS; i++)
		{
			for (int j = 0; j != meshModels[i].numMaterials; j++)
				{
					meshModels[i].mesh[j].textureID = utilLoadTexture(numTextures + NUM_TEXTURES, meshModels[i].materialName[j].C_Str());
					con_print(CON_INFO, true, "Model [ %i ] : Texture ID [ %i ]", i, meshModels[i].mesh[j].textureID);
				}
		}
}

//-----------------------------------------------------------------------------
//
// Render the mesh
//
// Pass in -1.0f to scaleBy to use setting from model loading
//
void ass_renderMesh(int whichModel, int whichShader, glm::vec3 pos, GLfloat scaleBy, glm::vec3 lightColor)
//-----------------------------------------------------------------------------
{
	GLfloat     	scaleFactor;
	glm::vec4       minSize;
	glm::vec4       maxSize;

	if (false == meshModels[whichModel].loaded)
		return;

		// overwriting shader?
//	if (true == g_debugBoundingBox)
//		debug_showBoundingBox(meshModels[whichModel].boundingBox, pos, scaleBy);

	//
	// Work out translation and scale matrix
	if (-1.0f == scaleBy)
		scaleFactor = meshModels[whichModel].scaleFactor;
	else
		scaleFactor = scaleBy;

	glm::mat4   scaleMatrix;
	
	scaleMatrix = glm::mat4();
	modelMatrix = glm::mat4();
	
	int whichMesh = 0;
	//
	// Adjust the size and position of the mesh
	scaleMatrix = glm::translate(glm::mat4(), pos);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	//
	// Translate model bounding box for testing against frustrum
	minSize = scaleMatrix * glm::vec4(meshModels[whichModel].boundingBox.minSize, 1.0);
	maxSize = scaleMatrix * glm::vec4(meshModels[whichModel].boundingBox.maxSize, 1.0);

	if (COMPLETE_OUT == sys_boxInFrustum( minSize.x, minSize.y, minSize.z,
	                                      maxSize.x, maxSize.y, maxSize.z))
		{
			numSkippedModels++;
			return;
		}


	GL_ASSERT(glBindVertexArray(meshModels[whichModel].vao_ID));
	GL_ASSERT(glUseProgram(shaderProgram[whichShader].programID));


	GL_ASSERT(glUniformMatrix4fv(glGetUniformLocation(shaderProgram[whichShader].programID, "u_viewProjectionMat"), 1, false, glm::value_ptr(projMatrix * viewMatrix)));
	GL_ASSERT(glUniformMatrix4fv(glGetUniformLocation(shaderProgram[whichShader].programID, "u_modelMat"), 1, false, glm::value_ptr(scaleMatrix) ));
	GL_ASSERT(glUniformMatrix3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "u_normalMatrix"), 1, false, glm::value_ptr(glm::inverseTranspose(glm::mat3(scaleMatrix)))) );

	if (MODEL_SPHERE == whichModel)
	{
		
		GL_ASSERT(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID, "uLightRadius"), scaleBy));
		GL_ASSERT(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "uLightColor"), 1, glm::value_ptr(lightColor)));
		GL_ASSERT(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "uLightPosition"), 1, glm::value_ptr(pos)));

		GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID, "uPositionTex"), 0));
		wrapglBindTexture(GL_TEXTURE0 + 0, id_textures[GBUFFER_TEXTURE_TYPE_POSITION]);

		GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID, "uNormalTex"), 1));
		wrapglBindTexture(GL_TEXTURE0 + 1, id_textures[GBUFFER_TEXTURE_TYPE_NORMAL]);

		GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID, "uDiffuseTex"), 2));
		wrapglBindTexture(GL_TEXTURE0 + 2, id_textures[GBUFFER_TEXTURE_TYPE_DIFFUSE]);
		
		GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID, "uDepthTex"), 3));
		wrapglBindTexture(GL_TEXTURE0 + 3, id_depthTexture);
	

		GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "cameraPosition"), 1, glm::value_ptr(camPosition)));
	}
	
	//
	// Always use vertex information
	GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[VERTEX_BUFFER]));
	GL_ASSERT(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
	GL_ASSERT(glVertexAttribPointer(shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 ) ));

	if (meshModels[whichModel].hasNormals == true)
		{
			//
			// Use Normal information
			GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[NORMAL_BUFFER]));
			GL_ASSERT(glEnableVertexAttribArray(shaderProgram[whichShader].inNormalsID));
			GL_ASSERT(glVertexAttribPointer(shaderProgram[whichShader].inNormalsID, 3, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 )));
		}

	if (meshModels[whichModel].hasTextures == true)
		{
			//
			// Use Texture coordinate information
			GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[TEXCOORD_BUFFER]));
			GL_ASSERT(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
			GL_ASSERT(glVertexAttribPointer(shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 ) ));
			
			wrapglBindTexture(GL_TEXTURE0, meshModels[whichModel].mesh[whichMesh].textureID);
		}

	GL_ASSERT(glDrawElements(GL_TRIANGLES, meshModels[whichModel].mesh[whichMesh].elementCount, GL_UNSIGNED_INT, NULL));

	glBindVertexArray(0);
    glUseProgram(0);
}

//-----------------------------------------------------------------------------
//
// Get all the information out of the mesh and upload to GPU
void ass_uploadMesh(aiMesh *mesh, int whichModel, int whichMesh)
//-----------------------------------------------------------------------------
{
	_boundingBox    tempBox;
	glm::vec3       tempVec3;

	aiVector3D      boxMin;
	aiVector3D      boxMax;
	aiVector3D      boxCenter;

	tempBox.minSize = glm::vec3(1000);
	tempBox.maxSize = glm::vec3(-1000);

	meshModels[whichModel].mesh[whichMesh].elementCount = mesh->mNumFaces * 3;

	if(mesh->HasPositions())
		{
			float *vertices = new float[mesh->mNumVertices * 3];

			for(int i = 0; i < mesh->mNumVertices; ++i)
				{
					vertices[i * 3] = mesh->mVertices[i].x;
					vertices[i * 3 + 1] = mesh->mVertices[i].y;
					vertices[i * 3 + 2] = mesh->mVertices[i].z;

					tempVec3.x = mesh->mVertices[i].x;
					tempVec3.y = mesh->mVertices[i].y;
					tempVec3.z = mesh->mVertices[i].z;

					//
					// Get smallest point
					if (tempBox.minSize.x > tempVec3.x)
						tempBox.minSize.x = tempVec3.x;

					if (tempBox.minSize.y > tempVec3.y)
						tempBox.minSize.y = tempVec3.y;

					if (tempBox.minSize.z > tempVec3.z)
						tempBox.minSize.z = tempVec3.z;

					//
					// Get the largest point
					if (tempBox.maxSize.x < tempVec3.x)
						tempBox.maxSize.x = tempVec3.x;

					if (tempBox.maxSize.y < tempVec3.y)
						tempBox.maxSize.y = tempVec3.y;

					if (tempBox.maxSize.z < tempVec3.z)
						tempBox.maxSize.z = tempVec3.z;
				}

//        Assimp::FindMeshCenter (mesh, boxCenter, boxMin, boxMax );

			meshModels[whichModel].boundingBox = tempBox;

			con_print(CON_INFO, true, "Bounding box size:");
			con_print(CON_INFO, true, "Min [ %3.3f %3.3f %3.3f ]", meshModels[whichModel].boundingBox.minSize.x, meshModels[whichModel].boundingBox.minSize.y, meshModels[whichModel].boundingBox.minSize.z);
			con_print(CON_INFO, true, "Max [ %3.3f %3.3f %3.3f ]", meshModels[whichModel].boundingBox.maxSize.x, meshModels[whichModel].boundingBox.maxSize.y, meshModels[whichModel].boundingBox.maxSize.z);

			GL_CHECK(glGenBuffers(1, &meshModels[whichModel].mesh[whichMesh].vbo[VERTEX_BUFFER]));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[VERTEX_BUFFER]));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW));

//            GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL));
//            GL_CHECK(glEnableVertexAttribArray (0));

			delete vertices;
		}

	meshModels[whichModel].hasTextures = false;
	if(mesh->HasTextureCoords(0))
		{
			float *texCoords = new float[mesh->mNumVertices * 2];

			for(int i = 0; i < mesh->mNumVertices; ++i)
				{
					texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
					texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
				}

			GL_CHECK(glGenBuffers(1, &meshModels[whichModel].mesh[whichMesh].vbo[TEXCOORD_BUFFER]));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[TEXCOORD_BUFFER]));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW));

//            GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL));
//            GL_CHECK(glEnableVertexAttribArray (1));
			meshModels[whichModel].hasTextures = true;
			delete texCoords;
		}

	meshModels[whichModel].hasNormals = false;
	if(mesh->HasNormals())
		{
			
			con_print(CON_INFO, true, "Model [ %i ] contains normals", whichModel);
			
			float *normals = new float[mesh->mNumVertices * 3];

			for(int i = 0; i < mesh->mNumVertices; ++i)
				{
					normals[i * 3] = mesh->mNormals[i].x;
					normals[i * 3 + 1] = mesh->mNormals[i].y;
					normals[i * 3 + 2] = mesh->mNormals[i].z;
				}

			GL_CHECK(glGenBuffers(1, &meshModels[whichModel].mesh[whichMesh].vbo[NORMAL_BUFFER]));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[NORMAL_BUFFER]));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW));

//            GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL));
//            GL_CHECK(glEnableVertexAttribArray (2));
			meshModels[whichModel].hasNormals = true;
			delete normals;
		}

	if(mesh->HasFaces())
		{
			unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];

			for(int i = 0; i < mesh->mNumFaces; ++i)
				{
					indices[i * 3] = mesh->mFaces[i].mIndices[0];
					indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
					indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
				}

			GL_CHECK(glGenBuffers(1, &meshModels[whichModel].mesh[whichMesh].vbo[INDEX_BUFFER]));
			GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshModels[whichModel].mesh[whichMesh].vbo[INDEX_BUFFER]));
			GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW));

//            GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL));
//            GL_CHECK(glEnableVertexAttribArray (3));

			delete indices;
		}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
//
// Get material information - textures for model
bool ass_getMaterials(aiScene *scene, int whichModel)
//-----------------------------------------------------------------------------
{
	aiString    texturePath;

	for (uint i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial*     pMaterial = scene->mMaterials[i];

			if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath, NULL, NULL, NULL, NULL, NULL))
				{
					texturePath.data[strlen(texturePath.data) - 4] = '\0';
					strcpy(meshModels[whichModel].materialName[i].C_Str(), texturePath.data);
					con_print(CON_INFO, true, "Model [ %i ] Material [ %i ] : [ %s ]", whichModel, i, meshModels[whichModel].materialName[i].C_Str());

				}

			else
				{
					strcpy(meshModels[whichModel].materialName[i].C_Str(), "Material load error");
					con_print(CON_INFO, true, "Model [ %i ] Material [ %i ] : [ %s ]", whichModel, i, meshModels[whichModel].materialName[i].C_Str());
				}
		}

}

//-----------------------------------------------------------------------------
//
// Load a asset model for rendering
//bool ass_loadModel(int whichModel, const char *fileName, float scaleFactor)
bool ass_loadModel(int whichModel, std::string fileName, float scaleFactor)
//-----------------------------------------------------------------------------
{
	// aiProcess_GenNormals
	// aiProcess_GenSmoothNormals

	//
	// Don't draw it later if it's not loaded
	meshModels[whichModel].loaded = false;

	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS);

	const aiScene *scene = importer.ReadFile(fileName, aiProcess_RemoveComponent | aiProcess_Triangulate |  aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (!scene)
		{
			con_print(CON_ERROR, true, "Error: Could not load mesh [ %s - %s ]", fileName.c_str(), importer.GetErrorString());
			return false;
		}

	meshModels[whichModel].loaded = true;
	//
	// Store model and all meshes in one VAO
	glGenVertexArrays(1, &meshModels[whichModel].vao_ID);
	glBindVertexArray(meshModels[whichModel].vao_ID);

	meshModels[whichModel].numMeshes = scene->mNumMeshes;
	meshModels[whichModel].numMaterials = scene->mNumMaterials;
	meshModels[whichModel].scaleFactor = scaleFactor;
	//
	// Allocate memory to hold material filenames and meshes for the model
	meshModels[whichModel].materialName.reserve(meshModels[whichModel].numMaterials);
	meshModels[whichModel].mesh.reserve(meshModels[whichModel].numMeshes);

	con_print(CON_INFO, true, "Mesh [ %i ] : [ %s ] numMeshes [ %i ] numMaterials [ %i ]", whichModel, fileName.c_str(), meshModels[whichModel].numMeshes, meshModels[whichModel].numMaterials);

	for(int i = 0; i < scene->mNumMeshes; ++i)
		{
			ass_uploadMesh(scene->mMeshes[i], whichModel, i);
			meshModels[whichModel].mesh[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
			meshModels[whichModel].mesh[i].numFaces = scene->mMeshes[i]->mNumFaces;
			meshModels[whichModel].mesh[i].numVertices = scene->mMeshes[i]->mNumVertices;
			con_print(CON_INFO, true, "Mesh [ %i ] : Material Index [ %i ] numFaces [ %i ] numVertices [ %i ]", i, meshModels[whichModel].mesh[i].materialIndex, meshModels[whichModel].mesh[i].numFaces, meshModels[whichModel].mesh[i].numVertices);
		}

	ass_getMaterials(scene, whichModel);
	//
	// Finished with this VAO now
	glBindVertexArray(0);

	return true;
}

//-----------------------------------------------------------------------------
//
// Load all the model resources
