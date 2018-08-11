#include <hdr/openGL/s_shaders.h>
#include "s_shadowLighting.h"
#include "s_globals.h"

GLuint m_WVPLocation;
GLuint m_WorldMatrixLocation;
GLuint m_samplerLocation;
GLuint m_shadowMapLocation;
GLuint m_eyeWorldPosLocation;
GLuint m_matSpecularIntensityLocation;
GLuint m_matSpecularPowerLocation;

struct {
	GLuint Color;
	GLuint AmbientIntensity;
	GLuint DiffuseIntensity;
	GLuint Position;
	struct {
		GLuint Constant;
		GLuint Linear;
		GLuint Exp;
	} Atten;
} m_pointLightLocation;

void shd_lightingSetWVP(const glm::mat4& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, glm::value_ptr(WVP));
}

void shd_lightingSetWorldMatrix(const glm::mat4& WorldInverse)
{
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, glm::value_ptr(WorldInverse));
}


void shd_lightingSetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_samplerLocation, TextureUnit);
}


void shd_lightingSetShadowMapTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_shadowMapLocation, TextureUnit);
}


void shd_lightingSetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}


void shd_lightingSetMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}


void shd_lightingSetMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}


void shd_lightingSetPointLight(const PointLight& Light)
{
	glUniform3f(m_pointLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_pointLightLocation.AmbientIntensity, Light.AmbientIntensity);
	glUniform1f(m_pointLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
	glUniform3f(m_pointLightLocation.Position, Light.Position.x, Light.Position.y, Light.Position.z);
	glUniform1f(m_pointLightLocation.Atten.Constant, Light.Attenuation.Constant);
	glUniform1f(m_pointLightLocation.Atten.Linear, Light.Attenuation.Linear);
	glUniform1f(m_pointLightLocation.Atten.Exp, Light.Attenuation.Exp);
}

//-----------------------------------------------------------------------------------
//
// Setup the lighting effect variables
bool shd_shadowLightingInit(int whichShader)
//-----------------------------------------------------------------------------------
{
	glUseProgram(shaderProgram[whichShader].programID);

	m_WVPLocation = static_cast<GLuint>(glGetUniformLocation (shaderProgram[whichShader].programID, "gWVP"));
	m_WorldMatrixLocation = static_cast<GLuint>(glGetUniformLocation (shaderProgram[whichShader].programID, "gWorld"));
	m_samplerLocation = static_cast<GLuint>(glGetUniformLocation (shaderProgram[whichShader].programID, "gColorMap"));
	m_shadowMapLocation = static_cast<GLuint>(glGetUniformLocation (shaderProgram[whichShader].programID, "gShadowMap"));
	m_eyeWorldPosLocation = static_cast<GLuint>(glGetUniformLocation (shaderProgram[whichShader].programID, "gEyeWorldPos"));
	m_matSpecularIntensityLocation = static_cast<GLuint>(glGetUniformLocation (shaderProgram[whichShader].programID, "gMatSpecularIntensity"));
	m_matSpecularPowerLocation = static_cast<GLuint>(glGetUniformLocation (shaderProgram[whichShader].programID, "gSpecularPower"));

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
	    m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
	    m_samplerLocation == INVALID_UNIFORM_LOCATION ||
	    m_shadowMapLocation == INVALID_UNIFORM_LOCATION ||
	    m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
	    m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
	    m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	shd_lightingSetColorTextureUnit (COLOR_TEXTURE_UNIT_INDEX);
	shd_lightingSetShadowMapTextureUnit (SHADOW_TEXTURE_UNIT_INDEX);
	shd_lightingSetPointLight (m_pointLight);

	return true;
}
