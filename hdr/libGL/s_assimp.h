#pragma once

#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

enum
{
	MODEL_CRATE,
	MODEL_TANK,
	MODEL_TANK_WHEELS,
	MODEL_SPHERE,
	MODEL_CONE,
	NUM_MODELS
};

typedef struct
{
	glm::vec3   minSize;
	glm::vec3   maxSize;
} _boundingBox;

extern int      numSkippedModels;
extern bool     g_debugBoundingBox;

// Render the mesh using just a GLM::VEC3 position
void ass_renderMeshVec3Position ( int whichModel, int whichShader, glm::vec3 pos, GLfloat scaleBy, glm::vec3 lightColor );

// Render the mesh using a transform/rotation/scale matrix from bullet
void ass_renderMeshMat4 ( int whichModel, int whichShader, glm::mat4 physicsMatrix, GLfloat scaleBy, glm::vec3 lightColor );

// Get all the information out of the mesh and upload to GPU
void ass_uploadMesh ( aiMesh *mesh );

// Load a asset model for rendering
bool ass_loadModel ( int whichModel,std::string fileName, float scaleFactor );

// Load all the textures for each of the models
void ass_loadModelTextures();

// Return the physical size of the model after scaling
glm::vec4 ass_getModelFinalsize(int whichModel, float scaleFactor);