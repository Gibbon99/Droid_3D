#include <AntTweakBar.h>
#include "s_globals.h"
#include "s_vsProfileLib.h"
#include "s_fontUtil.h"
#include "s_bmpFont.h"
#include "s_ttfFont.h"
#include "s_timing.h"
#include "s_window.h"
#include "s_shaders.h"

#include "s_openGL.h"
#include "s_maths.h"
#include "s_camera.h"
#include "s_openGLWrap.h"
#include "s_physics.h"
#include "s_antBar.h"
#include "s_shadowMap.h"
#include "io_textures.h"
#include "s_assimp.h"
#include "s_defRender.h"
#include "s_shaderLights.h"
#include "s_renderBSP.h"
#include "s_lightPass.h"
#include "s_bullet.h"
#include "s_doorsBSP.h"

GLuint mvp_loc;
GLuint world_loc;
GLuint light_mvp_loc;
GLuint light_color_loc;
GLuint light_pos_loc;

float aspect;
//glm::mat4 proj_mat = glm::perspective(60.0f, aspect, 0.1f, 100.0f);
glm::mat4 proj_mat;
glm::mat4 light_view_mat;
glm::mat4 cam_view_mat;
glm::mat4 world_mat;
glm::mat4 light_mvp_mat;
glm::mat4 light_view_proj_mat;
glm::mat4 cam_mvp_mat;

glm::mat4 shadowMat;

glm::mat4 shadowViewMat;
glm::mat4 shadowProjMat;

bool        drawWireframe = false;
bool        showGBuffers = false;

GLfloat cube_colors[] =
{
	0.6, 0.6, 0.6, 0.5,
	0.6, 0.6, 0.6, 0.5,
	0.6, 0.6, 0.6, 0.5,
	0.6, 0.6, 0.6, 0.5,
};

#define GROUND_SIZE 50.0

GLfloat cubeVertsBig[] =
{
	-GROUND_SIZE,  0.0,  GROUND_SIZE,
	-GROUND_SIZE,  0.0, -GROUND_SIZE,
	GROUND_SIZE,  0.0, -GROUND_SIZE,
	GROUND_SIZE,  0.0,  GROUND_SIZE,
};

GLfloat groundTexCoords[] =
{
	0.0, 0.0,
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
};

unsigned int faceIndex[] =
{
	0,1,2,0,2,3,
	4,5,6,4,6,7,
	2,5,4,2,4,3,
	2,1,6,2,5,6,
	0,6,1,0,7,6,
	3,0,7,3,7,4
};

glm::vec3   verts[8];
glm::vec4   colors[8];

//-----------------------------------------------------------------------------
//
// Render all the models - pass in shader to use
void sys_renderModels(int whichShader)
//-----------------------------------------------------------------------------
{

	//
	// Reset counter
	numSkippedModels = 0;

	GL_CHECK(glUniform3fv(glGetUniformLocation(shaderProgram[whichShader].programID, "materialSpecularColor"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0))));
	GL_CHECK(glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID,  "materialShininess"), 200.0f));

	ass_renderMesh(MODEL_TANK,  whichShader, glm::vec3(10.0, 15.0, 50.0), 0.3f);
	ass_renderMesh(MODEL_TANK,  whichShader, glm::vec3(-40.0, 15.0, 60.0), 0.3f);
	ass_renderMesh(MODEL_TANK,  whichShader, glm::vec3(-80.0, 15.0, 70.0), 0.3f);
	ass_renderMesh(MODEL_CRATE, whichShader, glm::vec3(50.0, 25.0, -10.0), 0.1f);
	ass_renderMesh(MODEL_CRATE, whichShader, glm::vec3(40.0, 25.0, -30.0), 0.1f);
	ass_renderMesh(MODEL_CRATE, whichShader, glm::vec3(30.0, 25.0, -50.0), 0.1f);

	gam_drawBullets(whichShader);


//    ass_renderMesh(MODEL_CRATE, whichShader, glm::vec3(-50.0, -280.0, -100.0), 5.0f);

	switch (whichShader)
		{
			case SHADER_SHADOWMAP:
			case SHADER_POINT_LIGHT:
				glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID, "materialShininess"),1000.0f);
//        ass_renderMesh(MODEL_CRATE, whichShader, glm::vec3(-50.0, -280.0, -100.0), 5.0f);
				break;
		}

}

//-----------------------------------------------------------------------------
//
// Render models to the shadowDepthMap
void sys_renderToShadowMap(int whichShader)
//-----------------------------------------------------------------------------
{
//
// offset matrix that maps from [-1, 1] to [0, 1] range
	glm::mat4 offsetMat = glm::mat4(
	                          glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
	                          glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
	                          glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
	                          glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
	                      );

	GL_CHECK(glUseProgram(shaderProgram[whichShader].programID));

	gl_set3DMode();
	//
	// Get shadowDepth map from camera viewpoint
	cam_look(gl_lightPos(), gl_lightDir());
	//
	// Pass these to second pass shader
	shadowViewMat = viewMatrix;
	shadowProjMat = projMatrix;
	//
	// combination of matrices into shadowMat
	shadowMat = offsetMat * shadowProjMat * shadowViewMat;

	gl_startShadowMap();

	glFrontFace(GL_CW);
	//
	// Clear FBO - not main visible framebuffer
	glClear (GL_DEPTH_BUFFER_BIT);

// activate offset for polygons
	glEnable(GL_POLYGON_OFFSET_FILL);
// offset by two units equal to smallest value of change in the shadow map
// and offset by two units depending on the slope of the polygon
	glPolygonOffset(1.5f, 1.5f);

	GL_CHECK(glUniformMatrix4fv(shaderProgram[whichShader].viewProjectionMat, 1, false, glm::value_ptr(projMatrix * viewMatrix)));

	sys_renderModels(whichShader);

//           testDrawVoxel(SHADER_COLOR);

	glDisable(GL_POLYGON_OFFSET_FILL);
	gl_stopShadowMap();
	glUseProgram(0);

	glFrontFace(GL_CCW);
}

//-----------------------------------------------------------------------------
//
// Render models to the bound FBO textures
void sys_renderToFBO()
//-----------------------------------------------------------------------------
{
	static bool debugOnce = false;

	gl_set3DMode();
	modelMatrix = mat4();

	cam_look(camPosition, camDirection);
	//
	// Bind FBO for writing
	gl_bindForWriting();
	//
	// Clear FBO - not main visible framebuffer
	glClearColor(0.1f, 0.1f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	GL_CHECK(glUniformMatrix4fv(shaderProgram[SHADER_GEOMETRY_PASS].modelMat, 1, false, glm::value_ptr(modelMatrix)));	
//	GL_CHECK(glUniformMatrix4fv(shaderProgram[SHADER_GEOMETRY_PASS].viewProjectionMat, 1, false, glm::value_ptr(projMatrix * viewMatrix)));
//    GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(shaderProgram[SHADER_GEOMETRY_PASS].programID, "u_shadowMat"), 1, false, glm::value_ptr(shadowMat)));

}

//-----------------------------------------------------------------------------
//
// Draw everything to the screen
void updateScreen(float interpolate)
//-----------------------------------------------------------------------------
{
//    char profileText[64];

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glViewport(0, 0, winWidth, winHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	// Get memory to hold vertex and char information
	ttf_startText();

	switch (currentMode)
		{
			case MODE_CONSOLE:
			{

				lib_setMouseCursor(true);
//        PROFILE("Frame");

				gl_set2DMode();

				gl_setFontColor(0.7f, 0.7f, 0.0f, 0.5);
				ttf_addText (FONT_SMALL, 50.0f, winHeight - 16, "FPS [ %i ] ThinkFPS [ %i ] Frametime [ %3.3f ]", fpsPrint, thinkFpsPrint, frameTimeTakenPrint);

				gl_setFontColor(1.0f, 0.0f, 1.0f, 1.0f);
				gl_drawScreen(false);

				// Draw GUI
				{
//            PROFILE("GUI");
					char        antParams[32];
					sprintf(antParams, "Frame size='%i %i'", antBarWidth, antBarHeight);
					TwDefine(antParams); // resize bar

					sprintf(antParams, "Frame position='%i %i'", antPosX, antPosY);
					TwDefine(antParams); // position bar
					TwDraw();
				}
				/*
				std::string s = VSProfileLib::DumpLevels();

				char tempString[128];
				int j = 0;
				int posY = 300;
				int yCount = 0;
				strcpy(tempString, "");

				//    ttf_printString(FONT_SMALL, 300, 100, s.c_str());
				ttf_addText(FONT_SMALL, 50, 200, s.c_str());
				io_logToFile("Render \n %s", s.c_str());


				        for (int i = 0; i != s.length(); i++)
				        {
				            tempString[j++] = s.c_str()[i];
				            if (s.c_str()[i] == '\n')
				            {
				                j = 0;
				                yCount++;
				                ttf_addText(0, 450, posY + (conFontSize * yCount), tempString);
				            }
				        }
				*/
			}


			break;

			case MODE_GAME:
				lib_setMouseCursor(false);
				glDepthMask(true);
				glClearDepth(1.0f);
				//
				// Enable depth test
				wrapglEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				//
				// Disable blending
				wrapglDisable(GL_BLEND);

//        sys_renderToShadowMap(SHADER_COLOR);
#define USE_DEF_RENDER
//#define USE_NORMAL_RENDER

#ifdef USE_DEF_RENDER
				sys_renderToFBO();
				
//				bsp_renderLevel(cam_getPosition(), SHADER_GEOMETRY_PASS);

//				sys_renderModels(SHADER_GEOMETRY_PASS);
ass_renderMesh(MODEL_CRATE, SHADER_GEOMETRY_PASS, cam_getPosition(), 0.5f);

			//	bsp_sendLightArrayToShader(SHADER_GEOMETRY_PASS);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glUseProgram(0);
				
				showGBuffers = true;

				if (true == showGBuffers)
					gl_showGBuffers();

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				gl_bindForReading();

				lt_renderFullscreenQuad(SHADER_DIR_LIGHT);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				
ass_renderMesh(MODEL_CRATE, SHADER_TTF_FONT, cam_getPosition() - vec3(0, 100, 0), 0.5f);



#endif
//-------------------Draw BSP with shader lighting --------------------
 
#ifdef USE_NORMAL_RENDER
				wrapglEnable(GL_DEPTH_TEST);
				wrapglDisable(GL_CULL_FACE);

				g_debugLightPos = true;

				if (true == g_debugLightPos)
					{
						for (int i = 0; i != numOfLights; i++)
						{
							drawLightPos(SHADER_COLOR, allLights[i].position);
							drawDebugLine( allLights[i].position, gl_lightDir(), allLights[i].position, DRAW_LINE, 1000, true, 1.0f );						
						}
					}

				bsp_renderLevel(cam_getPosition(), SHADER_RENDER_BSP);

				sys_renderModels(SHADER_RENDER_BSP);

				bsp_sendLightArrayToShader(SHADER_RENDER_BSP);

				bspDrawAllDoorTriggerZones();
				
// -------------------- End Draw BSP with shader lighting ----------------------
#endif
				glUseProgram(0);

/*
				if (false == drawWireframe)
					{
						bul_enableDebug(drawWireframe);

					}
				else
					{
						bul_enableDebug(drawWireframe);
					}
*/
				gl_getAllGLErrors (NULL, __func__, __LINE__);

				TwDraw();
				
				gl_setFontColor(0.7f, 0.7f, 0.0f, 1.0);
				ttf_addText (FONT_SMALL, 0.0f, 16.0f, "FPS [ %i ] ThinkFPS [ %i ] Frametime [ %3.3f ]", fpsPrint, thinkFpsPrint, frameTimeTakenPrint);

				break;

			default:
				break;
		}

	//
	// Render all text in VBO memory
	gl_set2DMode();
	ttf_displayText(FONT_SMALL);

	lib_swapBuffers();
	glfwPollEvents();
}
