#pragma once
#include "s_globals.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <vector>
#include "s_physics.h"
#include <bullet/BulletCollision/CollisionShapes/btConvexHullShape.h>

enum
{
	MODEL_CRATE,
	MODEL_TANK,
	MODEL_TANK_WHEELS,
	MODEL_FEMADROID,
	MODEL_SPHERE,
	MODEL_CONE,
	NUM_MODELS
};

typedef struct
{
	glm::vec3   minSize;
	glm::vec3   maxSize;
} _boundingBox;

typedef struct
{
	int					materialIndex;
	int					elementCount;
	int					numFaces;
	int					numVertices;
	bool				isPhysicsObject;
	int					physicsIndex;
	GLuint				textureID;
	GLuint				vao_ID;
	_boundingBox 		boundingBox;
	btConvexHullShape*	physicsShapeConvexHull;
	GLuint				vbo[4];	// Position, texture, normal, index
} _mesh;

typedef struct
{
	bool                loaded;
	bool				hasNormals;
	bool				hasTextures;
	bool				hasSinglePhysicsObject;
	int					numMeshes;
	int					numMaterials;
	vector<_mesh>		mesh;
	vector<aiString>	materialName;
	btConvexHullShape*	physicsObject = nullptr;		// Single physics object from multiple meshes in model
} _meshModel;

extern	_meshModel		meshModels[];

extern int      numSkippedModels;
extern bool     g_debugBoundingBox;

// Render the mesh using just a GLM::VEC3 position
void ass_renderMeshVec3Position ( int whichModel, int whichShader, glm::vec3 pos, GLfloat scaleBy, glm::vec3 lightColor );

// Render the mesh using a transform/rotation/scale matrix from bullet
void ass_renderMeshMat4 ( int whichModel, int whichShader, glm::mat4 physicsMatrix, GLfloat scaleBy, glm::vec3 lightColor );

// Get all the information out of the mesh and upload to GPU
void ass_uploadMesh ( aiMesh *mesh );

// Load a asset model for rendering
bool ass_loadModel ( int whichModel,std::string fileName );

// Load all the textures for each of the models
void ass_loadModelTextures();