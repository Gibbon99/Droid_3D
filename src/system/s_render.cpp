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

//-----------------------------------------------------------------------------
//
// Render all the models - pass in shader to use
void sys_renderModels(int whichShader)
//-----------------------------------------------------------------------------
{

	//
	// Reset counter
	numSkippedModels = 0;

	ass_renderMesh(MODEL_TANK,  whichShader, glm::vec3(10.0, 15.0, 50.0), 0.3f);
	ass_renderMesh(MODEL_TANK,  whichShader, glm::vec3(-40.0, 15.0, 60.0), 0.3f);
	ass_renderMesh(MODEL_TANK,  whichShader, glm::vec3(-80.0, 15.0, 70.0), 0.3f);
	ass_renderMesh(MODEL_CRATE, whichShader, glm::vec3(50.0, 25.0, -10.0), 0.1f);
	ass_renderMesh(MODEL_CRATE, whichShader, glm::vec3(40.0, 25.0, -30.0), 0.1f);
	ass_renderMesh(MODEL_CRATE, whichShader, glm::vec3(30.0, 25.0, -50.0), 0.1f);

	gam_drawBullets(whichShader);

	switch (whichShader)
		{
			case SHADER_SHADOWMAP:
			case SHADER_POINT_LIGHT:
				glUniform1f(glGetUniformLocation(shaderProgram[whichShader].programID, "materialShininess"),1000.0f);
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
	glDepthMask(true);
	glClearDepth(1.0f);
	//
	// Enable depth test
	wrapglEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	//
	// Disable blending
	wrapglDisable(GL_BLEND);
	wrapglDisable(GL_CULL_FACE);
	
	gl_set3DMode();
	
	gl_bindForWriting();
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GL_CHECK(glUseProgram(shaderProgram[SHADER_GEOMETRY_PASS].programID));
	
	GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(shaderProgram[SHADER_GEOMETRY_PASS].programID, "u_viewProjectionMat"), 1, false, glm::value_ptr(projMatrix * viewMatrix)));
	
}

//-----------------------------------------------------------------------------
//
// Draw everything to the screen
void updateScreen(float interpolate)
//-----------------------------------------------------------------------------
{
//    char profileText[64];

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
        PROFILE("Frame");

				gl_set2DMode();

				gl_setFontColor(0.7f, 0.7f, 0.0f, 0.5);
				ttf_addText (FONT_SMALL, 50.0f, winHeight - 16, "FPS [ %i ] ThinkFPS [ %i ] Frametime [ %3.3f ]", fpsPrint, thinkFpsPrint, frameTimeTakenPrint);

				gl_setFontColor(1.0f, 0.0f, 1.0f, 1.0f);
				gl_drawScreen(false);

				// Draw GUI
				{
            PROFILE("GUI");
					char        antParams[32];
					sprintf(antParams, "Frame size='%i %i'", antBarWidth, antBarHeight);
					TwDefine(antParams); // resize bar

					sprintf(antParams, "Frame position='%i %i'", antPosX, antPosY);
					TwDefine(antParams); // position bar
					TwDraw();
				}
				
				
#ifdef SHOW_PROFILE_STATS
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
#endif
			}	// end of case
			break;

			case MODE_GAME:
				lib_setMouseCursor(false);
				
				sys_renderToFBO();

				bsp_renderLevel(cam_getPosition(), SHADER_GEOMETRY_PASS);

				bspDrawAllDoorTriggerZones();

				sys_renderModels(SHADER_GEOMETRY_PASS);

//				ass_renderMesh(MODEL_CRATE, SHADER_GEOMETRY_PASS, vec3(144.0f, 64.0f, 8.0f), 0.5f);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glUseProgram(0);
				
				if (true == showGBuffers)
					gl_showGBuffers();

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glUseProgram(0);

//				wrapglEnable(GL_DEPTH_TEST);
//				wrapglDisable(GL_CULL_FACE);
				
				if (false == showGBuffers)
				{
					gl_bindForReading();
					lt_renderFullscreenQuad(SHADER_DIR_LIGHT);
					
					lt_renderPointLights(SHADER_POINT_LIGHT);
				}
					

				if (true == g_debugLightPos)
					{
						for (int i = 0; i != numOfLights; i++)
						{
							drawLightPos(SHADER_COLOR, allLights[i].position);
							drawDebugLine( allLights[i].position, gl_lightDir(), allLights[i].position, DRAW_LINE, 1000, true, 1.0f );
						}
					}

//				bsp_sendLightArrayToShader(SHADER_RENDER_BSP);

//				TwDraw();
				
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
}
