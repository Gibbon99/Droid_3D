#pragma once

#include "s_physics.h"

#include <iostream>
#include <vector>

class GLDebugDrawer : public btIDebugDraw
{
	int m_debugMode;

public:

	struct LINE
	{
		LINE(glm::vec3 a, glm::vec3 b, glm::vec3 color) : a(a), b(b), color(color) {};

		glm::vec3 a;
		glm::vec3 b;
		glm::vec3 color;
	};

	GLDebugDrawer();

	virtual void   drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

	virtual void   drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

	virtual void   reportErrorWarning(const char* warningString);

	virtual void   draw3dText(const btVector3& location,const char* textString);

	virtual void   setDebugMode(int debugMode);

	virtual int    getDebugMode() const
	{
		return m_debugMode;
	}

	std::vector<LINE> & GetLines()
	{
		return lines;
	}

private:
	std::vector<LINE> lines;
};

// Show wireframe outlines of physics shapes
void bul_drawDebugLines(vector<GLDebugDrawer::LINE> & lines);
