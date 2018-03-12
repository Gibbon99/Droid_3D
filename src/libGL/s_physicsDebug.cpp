#include "s_physics.h"
#include "s_physicsDebug.h"
#include "s_shaders.h"
#include "s_openGLWrap.h"
#include <vector>

GLDebugDrawer::GLDebugDrawer()
//    :m_debugMode(0)
{

}

//------------------------------------------------------------
//
// Get the line information from Bullet, and store in the LINE struct for later drawing
void GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
//------------------------------------------------------------
{
	static int callCount = 0;

	glm::vec3 src(from.x(), from.y(), from.z());
	glm::vec3 dst(to.x(), to.y(), to.z());
	glm::vec3 col(color.x(), color.y(), color.z());

	LINE l(src, dst, col);
	lines.push_back(l);
}

void GLDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

void GLDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
}

void GLDebugDrawer::reportErrorWarning(const char* warningString)
{
	con_print(CON_INFO, true, warningString);
}

void GLDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{

}

//------------------------------------------------------------
//
// Show wireframe outlines of physics lines
void bul_drawDebugLines(vector<GLDebugDrawer::LINE> & lines)
//------------------------------------------------------------
{
	vector<GLfloat>     vertices;
	vector<GLfloat>     color;
	vector<GLuint>      indices;
	GLuint              vboLineColors;

	GLuint			    lineVAO, lineVBO, lineColorVBO;

	wrapglDisable(GL_CULL_FACE);

	unsigned int indexI = 0;

	//
	// Extract vertice and color information from lines
	for (vector<GLDebugDrawer::LINE>::iterator it = lines.begin(); it != lines.end(); it++)
		{
			GLDebugDrawer::LINE l = (*it);
			vertices.push_back(l.a.x);
			vertices.push_back(l.a.y);
			vertices.push_back(l.a.z);
			vertices.push_back(l.b.x);
			vertices.push_back(l.b.y);
			vertices.push_back(l.b.z);

			color.push_back(l.color.r);
			color.push_back(l.color.g);
			color.push_back(l.color.b);
			color.push_back(l.color.r);
			color.push_back(l.color.g);
			color.push_back(l.color.b);

			indices.push_back(indexI);
			indices.push_back(indexI + 1);
			indexI += 2;
		}

	//
	// Enable the shader program
	glUseProgram( shaderProgram[SHADER_COLOR].programID );

	glGenVertexArrays( 1, &lineVAO );
	glBindVertexArray( lineVAO );
	//
	// Use Vertices
	glGenBuffers( 1, &lineVBO );
	glBindBuffer( GL_ARRAY_BUFFER, lineVBO );		// Allocate space and upload from CPU to GPU
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * 6, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(shaderProgram[SHADER_COLOR].inVertsID);
	glVertexAttribPointer(shaderProgram[SHADER_COLOR].inVertsID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( 0 ));
	//
	// Use Colors
	glGenBuffers(1, &vboLineColors);
	glBindBuffer(GL_ARRAY_BUFFER, vboLineColors);
	glBufferData(GL_ARRAY_BUFFER, color.size() * 6, &color[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(shaderProgram[SHADER_COLOR].inColorID);
	glVertexAttribPointer(shaderProgram[SHADER_COLOR].inColorID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( 0 ) );	// R G B
	//
	// Calculate matrix and upload to shader
//    glm::mat4   mvpMatrix = projMatrix * viewMatrix * modelMatrix;

	GL_CHECK(glUniformMatrix4fv(shaderProgram[SHADER_COLOR].modelMat, 1, false, glm::value_ptr(modelMatrix)));
	GL_CHECK(glUniformMatrix4fv(shaderProgram[SHADER_COLOR].viewProjectionMat, 1, false, glm::value_ptr(projMatrix * viewMatrix)));

//    glUniformMatrix4fv(shaderProgram[SHADER_COLOR].MVPLocation, 1, false, glm::value_ptr(mvpMatrix));
	//
	// Draw debug lines
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, (void*)&(indices[0]));

//    glDrawArrays(GL_LINES, 0, vertices.size());

	lines.clear();
	color.clear();
	indices.clear();

	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &lineVBO);
	glDeleteBuffers(1, &lineColorVBO);
	glDeleteVertexArrays(1, &lineVAO);
}
