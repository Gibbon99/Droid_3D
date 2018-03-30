#include "s_physics.h"
#include "s_physicsDebug.h"
#include "s_shaders.h"
#include "s_openGLWrap.h"
#include "io_textures.h"
#include <vector>

bool				g_debugPhysics;		// Set from script

GLDebugDrawer		debugDrawer;

GLDebugDrawer::GLDebugDrawer()
//    :m_debugMode(0)
{

}

//------------------------------------------------------------
//
// Draw debug physics world
void bul_drawDebugWorld()
//------------------------------------------------------------
{
	dynamicsWorld->debugDrawWorld();
	bul_drawDebugLines ( debugDrawer.GetLines() );
}

//------------------------------------------------------------
//
// Enable / Disable debug line information
void bul_enableDebug ( bool performDebug )
//------------------------------------------------------------
{
	if ( true == performDebug )
		debugDrawer.setDebugMode ( 1 );
	else
		debugDrawer.setDebugMode ( 0 );
}

//------------------------------------------------------------
//
// Get the line information from Bullet, and store in the LINE struct for later drawing
void GLDebugDrawer::drawLine ( const btVector3& from,const btVector3& to,const btVector3& color )
//------------------------------------------------------------
{
//	static int callCount = 0;

	glm::vec3 src ( from.x(), from.y(), from.z() );
	glm::vec3 dst ( to.x(), to.y(), to.z() );
	glm::vec3 col ( color.x(), color.y(), color.z() );

	LINE l ( src, dst, col );
	lines.push_back ( l );
}

void GLDebugDrawer::setDebugMode ( int debugMode )
{
	m_debugMode = debugMode;
}

void GLDebugDrawer::draw3dText ( const btVector3& location,const char* textString )
{
}

void GLDebugDrawer::reportErrorWarning ( const char* warningString )
{
	con_print ( CON_INFO, true, warningString );
}

void GLDebugDrawer::drawContactPoint ( const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color )
{

}

//------------------------------------------------------------
//
// Show wireframe outlines of physics lines
void bul_drawDebugLines ( vector<GLDebugDrawer::LINE> & lines )
//------------------------------------------------------------
{
	static vector<GLfloat>		vertices;
	static vector<GLfloat>		color;
	static vector<GLuint>		indices;
	static vector<GLfloat>		texCoords;

	static bool			initDone = false;

	static GLuint		lineTexCoordsVBO;
	static GLuint		lineColorsVBO;
	static GLuint	    lineVAO = -1;
	static GLuint		lineVBO = -1;

	int		whichShader = SHADER_PHYSIC_DEBUG;

	unsigned int indexI = 0;

	vertices.clear();
	color.clear();
	indices.clear();
	texCoords.clear();

	//
	// Extract vertice and color information from lines
	for ( vector<GLDebugDrawer::LINE>::iterator it = lines.begin(); it != lines.end(); it++ )
		{
			GLDebugDrawer::LINE l = ( *it );

			vertices.push_back ( l.a.x );
			vertices.push_back ( l.a.y );
			vertices.push_back ( l.a.z );
			vertices.push_back ( l.b.x );
			vertices.push_back ( l.b.y );
			vertices.push_back ( l.b.z );

			color.push_back ( l.color.r );
			color.push_back ( l.color.g );
			color.push_back ( l.color.b );
			color.push_back ( l.color.r );
			color.push_back ( l.color.g );
			color.push_back ( l.color.b );

			texCoords.push_back ( 0.0f );
			texCoords.push_back ( 0.0f );
			texCoords.push_back ( 1.0f );
			texCoords.push_back ( 1.0f );

			indices.push_back ( indexI );
			indices.push_back ( indexI + 1 );
			indexI += 2;
		}

	lines.clear();

	if ( false == initDone )
		{

			//
			// Enable the shader program
			glUseProgram ( shaderProgram[whichShader].programID );

			glGenVertexArrays ( 1, &lineVAO );
			glBindVertexArray ( lineVAO );
			//
			// Use Vertices
			glGenBuffers ( 1, &lineVBO );
			glBindBuffer ( GL_ARRAY_BUFFER, lineVBO );
			glBufferData ( GL_ARRAY_BUFFER, vertices.size() * sizeof ( GLfloat ), &vertices[0], GL_STATIC_DRAW );
			glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID );
			glVertexAttribPointer ( shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) );

			//
			// Line Color
			glGenBuffers ( 1, &lineColorsVBO );
			glBindBuffer ( GL_ARRAY_BUFFER, lineColorsVBO );
			glBufferData ( GL_ARRAY_BUFFER, color.size() * sizeof ( GLfloat ), &color[0], GL_STATIC_DRAW );
			glEnableVertexAttribArray ( shaderProgram[whichShader].inColorID );
			glVertexAttribPointer ( shaderProgram[whichShader].inColorID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) );	// R G B

			//
			// Use Texture coordinate information
			glGenBuffers ( 1, &lineTexCoordsVBO );
			glBindBuffer ( GL_ARRAY_BUFFER, lineTexCoordsVBO );
			glBufferData ( GL_ARRAY_BUFFER, texCoords.size() * sizeof ( GLfloat ), &texCoords[0], GL_STATIC_DRAW );
			glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID );
			glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) );

			wrapglBindTexture ( GL_TEXTURE0, texturesLoaded[TEX_WHITE_SQUARE].texID );

			initDone = true;
		}

	//
	// Enable the shader program
	glUseProgram ( shaderProgram[whichShader].programID );

	glBindVertexArray ( lineVAO );
	glBindBuffer ( GL_ARRAY_BUFFER, lineVBO );
	glBufferData ( GL_ARRAY_BUFFER, vertices.size() * sizeof ( GLfloat ), &vertices[0], GL_STATIC_DRAW );
	glEnableVertexAttribArray ( shaderProgram[whichShader].inVertsID );
	glVertexAttribPointer ( shaderProgram[whichShader].inVertsID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) );

	//
	// Line Color
	glBindBuffer ( GL_ARRAY_BUFFER, lineColorsVBO );
	glBufferData ( GL_ARRAY_BUFFER, color.size() * sizeof ( GLfloat ), &color[0], GL_STATIC_DRAW );
	glEnableVertexAttribArray ( shaderProgram[whichShader].inColorID );
	glVertexAttribPointer ( shaderProgram[whichShader].inColorID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) );	// R G B

	//
	// Use Texture coordinate information
	glBindBuffer ( GL_ARRAY_BUFFER, lineTexCoordsVBO );
	glBufferData ( GL_ARRAY_BUFFER, texCoords.size() * sizeof ( GLfloat ), &texCoords[0], GL_STATIC_DRAW );
	glEnableVertexAttribArray ( shaderProgram[whichShader].inTextureCoordsID );
	glVertexAttribPointer ( shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) );


	//
	// Calculate matrix and upload to shader
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr ( modelMatrix ) ) );
	GL_CHECK ( glUniformMatrix4fv ( shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr ( projMatrix * viewMatrix ) ) );
	//
	// Needs a texture to write to the Diffuse FBO to show up on deferred rendering pass
	//
	wrapglBindTexture ( GL_TEXTURE0, texturesLoaded[TEX_WHITE_SQUARE].texID );
	GL_ASSERT ( glUniform1i ( shaderProgram[whichShader].inTextureUnit, 0 ) );
	//
	// Draw debug lines
	glDrawElements ( GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, ( void* ) & ( indices[0] ) );

//	glDrawArrays ( GL_LINES, 0, vertices.size() );

	glUseProgram ( 0 );
	glBindVertexArray ( 0 );
}
