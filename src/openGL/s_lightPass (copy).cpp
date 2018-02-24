#include "../s_system/s_globals.h"
#include "../s_system/s_camera.h"
#include "s_openGLWrap.h"
#include "s_shaders.h"
#include "s_lightPass.h"
#include "s_defRender.h"

#include "../s_io/io_textures.h"
#include "s_shadowMap.h"
#include "../s_system/s_render.h"

GLuint	m_diffuseID;
GLuint	m_positionID;
GLuint	m_normalsID;
GLuint  m_camPositionID;
GLuint  m_lightPositionID;
GLuint  m_mvMatrix;
GLuint  m_normalMatrix;
GLuint  m_tShadowMapID;
GLuint	m_DepthMapID;

GLuint  m_PositionShadowMap;

//-----------------------------------------------------------------------------
//
// Get shader values for caching
void lt_initShaderValues(int whichShader)
//-----------------------------------------------------------------------------
{
    GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

    gl_getUniformVariable(whichShader, (char *)"tDiffuse",            gl_getShaderName(whichShader), &m_diffuseID);
    gl_getUniformVariable(whichShader, (char *)"tPosition",           gl_getShaderName(whichShader), &m_positionID);
    gl_getUniformVariable(whichShader, (char *)"tNormals",            gl_getShaderName(whichShader), &m_normalsID);
    gl_getUniformVariable(whichShader, (char *)"tShadowMap",          gl_getShaderName(whichShader), &m_tShadowMapID);
    gl_getUniformVariable(whichShader, (char *)"tDepthMap",           gl_getShaderName(whichShader), &m_DepthMapID);
    gl_getUniformVariable(whichShader, (char *)"cameraPosition",      gl_getShaderName(whichShader), &m_camPositionID);
    gl_getUniformVariable(whichShader, (char *)"lightPosition",       gl_getShaderName(whichShader), &m_lightPositionID);

    gl_getUniformVariable(whichShader, (char *)"tPositionShadowMap",  gl_getShaderName(whichShader), &m_PositionShadowMap);



}



//-----------------------------------------------------------------------------
//
// Start shader pass to render scene
void lt_beginLightPass()
//-----------------------------------------------------------------------------
{
    wrapglEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    wrapglDisable(GL_BLEND);
//    gl_bindForReading();
//    glClear(GL_COLOR_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
//
// Setup light values for each light
void lt_sendLightValuesToShader(int whichShader)
//-----------------------------------------------------------------------------
{
    #define LIGHT_SPOT_TYPE 0
    #define LIGHT_POINT_TYPE 1

    GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[0].type"), LIGHT_SPOT_TYPE));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[0].spotCutOff"), 0.7));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[0].spotExponent"), 0.819));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[0].direction"), 1, glm::value_ptr(gl_lightDir())));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[0].constant"), 1.0f));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[0].linear"), 0.09f));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[0].quadratic"), 0.032f));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[0].position"), 1, glm::value_ptr(gl_lightPos())));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[0].color"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[0].diffuse"), 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[0].specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[0].ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5))));

    GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[1].type"), LIGHT_POINT_TYPE));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[1].constant"), 1.0f));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[1].linear"), 0.09f));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[1].quadratic"), 0.032f));
//    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[1].position"), 1, glm::value_ptr(glm::vec3(50, -10, 90.0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[1].position"), 1, glm::value_ptr(camPosition)));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[1].color"), 1, glm::value_ptr(glm::vec3(1.0, 0, 0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[1].diffuse"), 1, glm::value_ptr(glm::vec3(1, 0, 0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[1].specular"), 1, glm::value_ptr(glm::vec3(1.0, 0, 0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[1].ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0, 0.0))));

    GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[2].type"), LIGHT_POINT_TYPE));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[2].constant"), 1.0f));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[2].linear"), 0.09f));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[2].quadratic"), 0.032f));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[2].position"), 1, glm::value_ptr(glm::vec3(-50, -10, -50.0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[2].color"), 1, glm::value_ptr(glm::vec3(0, 1, 0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[2].diffuse"), 1, glm::value_ptr(glm::vec3(0, 0.5, 0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[2].specular"), 1, glm::value_ptr(glm::vec3(0, 1, 0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[2].ambient"), 1, glm::value_ptr(glm::vec3(0.0, 0.5, 0))));

    GL_CHECK(glUniform1i(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[3].type"), LIGHT_POINT_TYPE));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[3].constant"), 1.0f));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[3].linear"), 0.07f));
    GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "light[3].quadratic"), 0.017f));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[3].position"), 1, glm::value_ptr(glm::vec3(50, -15.0, 90.0))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[3].color"), 1, glm::value_ptr(glm::vec3(1, 0, 1))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[3].diffuse"), 1, glm::value_ptr(glm::vec3(0.5, 0.0, 0.5))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[3].specular"), 1, glm::value_ptr(glm::vec3(1, 0, 1))));
    GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "light[3].ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.0, 0.5))));

}

//-----------------------------------------------------------------------------
//
// Render a full screen quad
void lt_renderFullscreenQuad(int whichShader)
//-----------------------------------------------------------------------------
{

    GLfloat quadVerts[] =
    {

        0.0,        0.0,
        winWidth / 2,   0.0,
        winWidth / 2,   winHeight / 2,
        0.0,        winHeight / 2,
    };

    GLfloat quadTexCoords[] =
    {
        0.0,      1.0,
        1.0,      1.0,
        1.0,      0.0,
        0.0,      0.0,
    };

    static bool initDone = false;
    static GLuint vboQuad, vboQuadTex, vaoQuad;

    if (true == showGBuffers)
    {
        quadVerts[2] = winWidth / 2;
        quadVerts[4] = winWidth / 2;
        quadVerts[5] = winHeight / 2;
        quadVerts[7] = winHeight / 2;
    }
    else
    {
        quadVerts[2] = winWidth;
        quadVerts[4] = winWidth;
        quadVerts[5] = winHeight;
        quadVerts[7] = winHeight;
        initDone = false;
    }

    if (false == initDone)
    {
        //
        // Enable the shader program
        GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

        GL_ASSERT(glGenVertexArrays(1, &vaoQuad));
        GL_CHECK(glBindVertexArray(vaoQuad));

        GL_ASSERT(glGenBuffers(1, &vboQuad));
        //
        // Use Vertices
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboQuad));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8, &quadVerts, GL_STATIC_DRAW));
        GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
        GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inVertsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 ) ));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

        GL_ASSERT(glGenBuffers(1, &vboQuadTex));
        //
        // Use texture coords
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboQuadTex));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8, &quadTexCoords, GL_STATIC_DRAW));
        GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
        GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 ) ));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
        //
        // Draw in single color using currently bound program
//            GL_CHECK(glUniform4f(shaderProgram[whichShader].inColorID, 0.5, 0.0, 0.3, 0.5));

        glBindVertexArray(0);
        glUseProgram(0);
        initDone = true;
    }

    //
    // Draw in ortho mode
    gl_set2DMode();
    //
    // Use the program to make the attributes available
    GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

    if (m_diffuseID > 0)
    {
        glEnable(GL_TEXTURE_2D);
        GL_CHECK(glActiveTexture(GL_TEXTURE0));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_returnTexID(GBUFFER_TEXTURE_TYPE_DIFFUSE)));
        GL_CHECK(glUniform1i ( m_diffuseID, 0 ));
    }

    if (m_positionID > 0)
    {
        glEnable(GL_TEXTURE_2D);
        GL_CHECK(glActiveTexture(GL_TEXTURE1));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_returnTexID(GBUFFER_TEXTURE_TYPE_POSITION)));
        GL_CHECK(glUniform1i ( m_positionID, 1 ));
    }

    if (m_normalsID > 0)
    {
        glEnable(GL_TEXTURE_2D);
        GL_CHECK(glActiveTexture(GL_TEXTURE2));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_returnTexID(GBUFFER_TEXTURE_TYPE_NORMAL)));
        GL_CHECK(glUniform1i ( m_normalsID, 2 ));
    }

    if (m_tShadowMapID > 0)
    {
        glEnable(GL_TEXTURE_2D);
        GL_CHECK(glActiveTexture(GL_TEXTURE3));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_getShadowMapTextureID()));
        GL_CHECK(glUniform1i ( m_tShadowMapID, 3 ));
    }

    if (m_DepthMapID > 0)
    {
        glEnable(GL_TEXTURE_2D);
        GL_CHECK(glActiveTexture(GL_TEXTURE4));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_returnDepthTexID()));
        GL_CHECK(glUniform1i ( m_DepthMapID, 4 ));
    }

    if (m_PositionShadowMap > 0)
    {
        glEnable(GL_TEXTURE_2D);
        GL_CHECK(glActiveTexture(GL_TEXTURE5));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_getShadowMapPositionsID()));
        GL_CHECK(glUniform1i ( m_PositionShadowMap, 5 ));
    }

    if (true == showGBuffers)
        modelMatrix = glm::translate(glm::mat4(), glm::vec3(winWidth / 4, winHeight / 4, 1.0));
    else
        modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, 1.0));
    //
    // Load the matrixes into the vertex shader
    GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr(modelMatrix)));
    GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr(projMatrix * viewMatrix)));

    glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "cameraPosition"), 1, glm::value_ptr(camPosition));

    lt_sendLightValuesToShader(whichShader);

    GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(shaderProgram[whichShader].programID, "cameraToLight"), 1, false, glm::value_ptr((shadowProjMat * shadowViewMat))));

    GL_CHECK(glBindVertexArray(vaoQuad));
    GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
    GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
    //
    // Draw a triangle
    GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));

    glDisableVertexAttribArray(shaderProgram[whichShader].inVertsID);
    glDisableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID);

    glBindVertexArray(0);
    glUseProgram(0);

    gl_set3DMode();
}


//-----------------------------------------------------------------------------
//
// Render a full screen quad
void lt_renderDepthQuad(int whichShader)
//-----------------------------------------------------------------------------
{

    GLfloat quadVertsDepthMap[] =
    {
        0.0,        0.0,
        winWidth / 2,   0.0,
        winWidth / 2,   winHeight / 2,
        0.0,        winHeight / 2,
    };

    GLfloat quadTexCoordsDepthMap[] =
    {
        0.0,      1.0,
        1.0,      1.0,
        1.0,      0.0,
        0.0,      0.0,
    };

    static bool initDone = false;
    static GLuint vboQuadDepthMap, vboQuadTexDepthMap, vaoQuadDepthMap;

    if (false == initDone)
    {
        //
        // Enable the shader program
        GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

        GL_ASSERT(glGenVertexArrays(1, &vaoQuadDepthMap));
        GL_CHECK(glBindVertexArray(vaoQuadDepthMap));

        GL_ASSERT(glGenBuffers(1, &vboQuadDepthMap));
        //
        // Use Vertices
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboQuadDepthMap));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8, &quadVertsDepthMap, GL_STATIC_DRAW));
        GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
        GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inVertsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 ) ));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

        GL_ASSERT(glGenBuffers(1, &vboQuadTexDepthMap));
        //
        // Use texture coords
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboQuadTexDepthMap));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8, &quadTexCoordsDepthMap, GL_STATIC_DRAW));
        GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
        GL_CHECK(glVertexAttribPointer(shaderProgram[whichShader].inTextureCoordsID, 2, GL_FLOAT, false, 0, BUFFER_OFFSET( 0 ) ));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

        glBindVertexArray(0);
        glUseProgram(0);
        initDone = true;
    }
    //
    // Draw in ortho mode
    gl_set2DMode();

    modelMatrix = glm::translate(glm::mat4(), glm::vec3(winWidth / 2, winHeight / 2, 1.0));
    //
    // Use the program to make the attributes available
    GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));
    //
    // Bind texture if it's not already bound as current texture
//    wrapglBindTexture(GL_TEXTURE0, gl_returnDepthTexID());

    wrapglBindTexture(GL_TEXTURE0, gl_getShadowMapTextureID());


    GL_CHECK(glUniform1i(shaderProgram[whichShader].inTextureUnit, 0));
    //
    // Load the matrixes into the vertex shader
    GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].modelMat, 1, false, glm::value_ptr(modelMatrix)));
    GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr(projMatrix * viewMatrix)));

    GL_CHECK(glBindVertexArray(vaoQuadDepthMap));
    GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inVertsID));
    GL_CHECK(glEnableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID));
    //
    // Draw a triangle
    GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));

    glDisableVertexAttribArray(shaderProgram[whichShader].inVertsID);
    glDisableVertexAttribArray(shaderProgram[whichShader].inTextureCoordsID);

    glBindVertexArray(0);
    glUseProgram(0);

    gl_set3DMode();
}
