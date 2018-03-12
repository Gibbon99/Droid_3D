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

// Render the mesh
//
// Pass in -1.0f to scaleBy to use setting from model loading
//
void ass_renderMesh(int whichModel, int whichShader, glm::vec3 pos, GLfloat scaleBy, glm::vec3 lightColor);

// Get all the information out of the mesh and upload to GPU
void ass_uploadMesh(aiMesh *mesh);

// Load a asset model for rendering
bool ass_loadModel(int whichModel,std::string fileName, float scaleFactor);

// Load all the textures for each of the models
void ass_loadModelTextures();
