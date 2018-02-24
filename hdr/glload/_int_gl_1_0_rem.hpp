#ifndef OPENGL_GEN_CORE_REM1_0_HPP
#define OPENGL_GEN_CORE_REM1_0_HPP

#include "_int_load_test.hpp"
namespace gl
{
namespace _detail
{
typedef void (CODEGEN_FUNCPTR * Proc_glAccum)(GLenum op, GLfloat value);
typedef void (CODEGEN_FUNCPTR * Proc_glAlphaFunc)(GLenum func, GLfloat ref);
typedef void (CODEGEN_FUNCPTR * Proc_glBegin)(GLenum mode);
typedef void (CODEGEN_FUNCPTR * Proc_glBitmap)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap);
typedef void (CODEGEN_FUNCPTR * Proc_glCallList)(GLuint list);
typedef void (CODEGEN_FUNCPTR * Proc_glCallLists)(GLsizei n, GLenum type, const GLvoid * lists);
typedef void (CODEGEN_FUNCPTR * Proc_glClearAccum)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glClearIndex)(GLfloat c);
typedef void (CODEGEN_FUNCPTR * Proc_glClipPlane)(GLenum plane, const GLdouble * equation);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3b)(GLbyte red, GLbyte green, GLbyte blue);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3bv)(const GLbyte * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3d)(GLdouble red, GLdouble green, GLdouble blue);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3f)(GLfloat red, GLfloat green, GLfloat blue);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3i)(GLint red, GLint green, GLint blue);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3s)(GLshort red, GLshort green, GLshort blue);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3ub)(GLubyte red, GLubyte green, GLubyte blue);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3ubv)(const GLubyte * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3ui)(GLuint red, GLuint green, GLuint blue);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3uiv)(const GLuint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3us)(GLushort red, GLushort green, GLushort blue);
typedef void (CODEGEN_FUNCPTR * Proc_glColor3usv)(const GLushort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4b)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4bv)(const GLbyte * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4d)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4i)(GLint red, GLint green, GLint blue, GLint alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4s)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4ubv)(const GLubyte * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4ui)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4uiv)(const GLuint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4us)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
typedef void (CODEGEN_FUNCPTR * Proc_glColor4usv)(const GLushort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glColorMaterial)(GLenum face, GLenum mode);
typedef void (CODEGEN_FUNCPTR * Proc_glCopyPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
typedef void (CODEGEN_FUNCPTR * Proc_glDeleteLists)(GLuint list, GLsizei range);
typedef void (CODEGEN_FUNCPTR * Proc_glDrawPixels)(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
typedef void (CODEGEN_FUNCPTR * Proc_glEdgeFlag)(GLboolean flag);
typedef void (CODEGEN_FUNCPTR * Proc_glEdgeFlagv)(const GLboolean * flag);
typedef void (CODEGEN_FUNCPTR * Proc_glEnd)();
typedef void (CODEGEN_FUNCPTR * Proc_glEndList)();
typedef void (CODEGEN_FUNCPTR * Proc_glEvalCoord1d)(GLdouble u);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalCoord1dv)(const GLdouble * u);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalCoord1f)(GLfloat u);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalCoord1fv)(const GLfloat * u);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalCoord2d)(GLdouble u, GLdouble v);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalCoord2dv)(const GLdouble * u);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalCoord2f)(GLfloat u, GLfloat v);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalCoord2fv)(const GLfloat * u);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalMesh1)(GLenum mode, GLint i1, GLint i2);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalMesh2)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalPoint1)(GLint i);
typedef void (CODEGEN_FUNCPTR * Proc_glEvalPoint2)(GLint i, GLint j);
typedef void (CODEGEN_FUNCPTR * Proc_glFeedbackBuffer)(GLsizei size, GLenum type, GLfloat * buffer);
typedef void (CODEGEN_FUNCPTR * Proc_glFogf)(GLenum pname, GLfloat param);
typedef void (CODEGEN_FUNCPTR * Proc_glFogfv)(GLenum pname, const GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glFogi)(GLenum pname, GLint param);
typedef void (CODEGEN_FUNCPTR * Proc_glFogiv)(GLenum pname, const GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glFrustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef GLuint (CODEGEN_FUNCPTR * Proc_glGenLists)(GLsizei range);
typedef void (CODEGEN_FUNCPTR * Proc_glGetClipPlane)(GLenum plane, GLdouble * equation);
typedef void (CODEGEN_FUNCPTR * Proc_glGetLightfv)(GLenum light, GLenum pname, GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glGetLightiv)(GLenum light, GLenum pname, GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glGetMapdv)(GLenum target, GLenum query, GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glGetMapfv)(GLenum target, GLenum query, GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glGetMapiv)(GLenum target, GLenum query, GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glGetMaterialfv)(GLenum face, GLenum pname, GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glGetMaterialiv)(GLenum face, GLenum pname, GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glGetPixelMapfv)(GLenum map, GLfloat * values);
typedef void (CODEGEN_FUNCPTR * Proc_glGetPixelMapuiv)(GLenum map, GLuint * values);
typedef void (CODEGEN_FUNCPTR * Proc_glGetPixelMapusv)(GLenum map, GLushort * values);
typedef void (CODEGEN_FUNCPTR * Proc_glGetPolygonStipple)(GLubyte * mask);
typedef void (CODEGEN_FUNCPTR * Proc_glGetTexEnvfv)(GLenum target, GLenum pname, GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glGetTexEnviv)(GLenum target, GLenum pname, GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glGetTexGendv)(GLenum coord, GLenum pname, GLdouble * params);
typedef void (CODEGEN_FUNCPTR * Proc_glGetTexGenfv)(GLenum coord, GLenum pname, GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glGetTexGeniv)(GLenum coord, GLenum pname, GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexMask)(GLuint mask);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexd)(GLdouble c);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexdv)(const GLdouble * c);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexf)(GLfloat c);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexfv)(const GLfloat * c);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexi)(GLint c);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexiv)(const GLint * c);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexs)(GLshort c);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexsv)(const GLshort * c);
typedef void (CODEGEN_FUNCPTR * Proc_glInitNames)();
typedef GLboolean (CODEGEN_FUNCPTR * Proc_glIsList)(GLuint list);
typedef void (CODEGEN_FUNCPTR * Proc_glLightModelf)(GLenum pname, GLfloat param);
typedef void (CODEGEN_FUNCPTR * Proc_glLightModelfv)(GLenum pname, const GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glLightModeli)(GLenum pname, GLint param);
typedef void (CODEGEN_FUNCPTR * Proc_glLightModeliv)(GLenum pname, const GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glLightf)(GLenum light, GLenum pname, GLfloat param);
typedef void (CODEGEN_FUNCPTR * Proc_glLightfv)(GLenum light, GLenum pname, const GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glLighti)(GLenum light, GLenum pname, GLint param);
typedef void (CODEGEN_FUNCPTR * Proc_glLightiv)(GLenum light, GLenum pname, const GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glLineStipple)(GLint factor, GLushort pattern);
typedef void (CODEGEN_FUNCPTR * Proc_glListBase)(GLuint base);
typedef void (CODEGEN_FUNCPTR * Proc_glLoadIdentity)();
typedef void (CODEGEN_FUNCPTR * Proc_glLoadMatrixd)(const GLdouble * m);
typedef void (CODEGEN_FUNCPTR * Proc_glLoadMatrixf)(const GLfloat * m);
typedef void (CODEGEN_FUNCPTR * Proc_glLoadName)(GLuint name);
typedef void (CODEGEN_FUNCPTR * Proc_glMap1d)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points);
typedef void (CODEGEN_FUNCPTR * Proc_glMap1f)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points);
typedef void (CODEGEN_FUNCPTR * Proc_glMap2d)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points);
typedef void (CODEGEN_FUNCPTR * Proc_glMap2f)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points);
typedef void (CODEGEN_FUNCPTR * Proc_glMapGrid1d)(GLint un, GLdouble u1, GLdouble u2);
typedef void (CODEGEN_FUNCPTR * Proc_glMapGrid1f)(GLint un, GLfloat u1, GLfloat u2);
typedef void (CODEGEN_FUNCPTR * Proc_glMapGrid2d)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
typedef void (CODEGEN_FUNCPTR * Proc_glMapGrid2f)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
typedef void (CODEGEN_FUNCPTR * Proc_glMaterialf)(GLenum face, GLenum pname, GLfloat param);
typedef void (CODEGEN_FUNCPTR * Proc_glMaterialfv)(GLenum face, GLenum pname, const GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glMateriali)(GLenum face, GLenum pname, GLint param);
typedef void (CODEGEN_FUNCPTR * Proc_glMaterialiv)(GLenum face, GLenum pname, const GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glMatrixMode)(GLenum mode);
typedef void (CODEGEN_FUNCPTR * Proc_glMultMatrixd)(const GLdouble * m);
typedef void (CODEGEN_FUNCPTR * Proc_glMultMatrixf)(const GLfloat * m);
typedef void (CODEGEN_FUNCPTR * Proc_glNewList)(GLuint list, GLenum mode);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3b)(GLbyte nx, GLbyte ny, GLbyte nz);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3bv)(const GLbyte * v);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3d)(GLdouble nx, GLdouble ny, GLdouble nz);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3i)(GLint nx, GLint ny, GLint nz);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3s)(GLshort nx, GLshort ny, GLshort nz);
typedef void (CODEGEN_FUNCPTR * Proc_glNormal3sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glOrtho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (CODEGEN_FUNCPTR * Proc_glPassThrough)(GLfloat token);
typedef void (CODEGEN_FUNCPTR * Proc_glPixelMapfv)(GLenum map, GLsizei mapsize, const GLfloat * values);
typedef void (CODEGEN_FUNCPTR * Proc_glPixelMapuiv)(GLenum map, GLsizei mapsize, const GLuint * values);
typedef void (CODEGEN_FUNCPTR * Proc_glPixelMapusv)(GLenum map, GLsizei mapsize, const GLushort * values);
typedef void (CODEGEN_FUNCPTR * Proc_glPixelTransferf)(GLenum pname, GLfloat param);
typedef void (CODEGEN_FUNCPTR * Proc_glPixelTransferi)(GLenum pname, GLint param);
typedef void (CODEGEN_FUNCPTR * Proc_glPixelZoom)(GLfloat xfactor, GLfloat yfactor);
typedef void (CODEGEN_FUNCPTR * Proc_glPolygonStipple)(const GLubyte * mask);
typedef void (CODEGEN_FUNCPTR * Proc_glPopAttrib)();
typedef void (CODEGEN_FUNCPTR * Proc_glPopMatrix)();
typedef void (CODEGEN_FUNCPTR * Proc_glPopName)();
typedef void (CODEGEN_FUNCPTR * Proc_glPushAttrib)(GLbitfield mask);
typedef void (CODEGEN_FUNCPTR * Proc_glPushMatrix)();
typedef void (CODEGEN_FUNCPTR * Proc_glPushName)(GLuint name);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos2d)(GLdouble x, GLdouble y);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos2dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos2f)(GLfloat x, GLfloat y);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos2fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos2i)(GLint x, GLint y);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos2iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos2s)(GLshort x, GLshort y);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos2sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos3d)(GLdouble x, GLdouble y, GLdouble z);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos3dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos3f)(GLfloat x, GLfloat y, GLfloat z);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos3fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos3i)(GLint x, GLint y, GLint z);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos3iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos3s)(GLshort x, GLshort y, GLshort z);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos3sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos4dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos4fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos4i)(GLint x, GLint y, GLint z, GLint w);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos4iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos4s)(GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (CODEGEN_FUNCPTR * Proc_glRasterPos4sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glRectd)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
typedef void (CODEGEN_FUNCPTR * Proc_glRectdv)(const GLdouble * v1, const GLdouble * v2);
typedef void (CODEGEN_FUNCPTR * Proc_glRectf)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
typedef void (CODEGEN_FUNCPTR * Proc_glRectfv)(const GLfloat * v1, const GLfloat * v2);
typedef void (CODEGEN_FUNCPTR * Proc_glRecti)(GLint x1, GLint y1, GLint x2, GLint y2);
typedef void (CODEGEN_FUNCPTR * Proc_glRectiv)(const GLint * v1, const GLint * v2);
typedef void (CODEGEN_FUNCPTR * Proc_glRects)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
typedef void (CODEGEN_FUNCPTR * Proc_glRectsv)(const GLshort * v1, const GLshort * v2);
typedef GLint (CODEGEN_FUNCPTR * Proc_glRenderMode)(GLenum mode);
typedef void (CODEGEN_FUNCPTR * Proc_glRotated)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (CODEGEN_FUNCPTR * Proc_glRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (CODEGEN_FUNCPTR * Proc_glScaled)(GLdouble x, GLdouble y, GLdouble z);
typedef void (CODEGEN_FUNCPTR * Proc_glScalef)(GLfloat x, GLfloat y, GLfloat z);
typedef void (CODEGEN_FUNCPTR * Proc_glSelectBuffer)(GLsizei size, GLuint * buffer);
typedef void (CODEGEN_FUNCPTR * Proc_glShadeModel)(GLenum mode);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord1d)(GLdouble s);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord1dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord1f)(GLfloat s);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord1fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord1i)(GLint s);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord1iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord1s)(GLshort s);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord1sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord2d)(GLdouble s, GLdouble t);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord2dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord2f)(GLfloat s, GLfloat t);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord2fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord2i)(GLint s, GLint t);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord2iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord2s)(GLshort s, GLshort t);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord2sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord3d)(GLdouble s, GLdouble t, GLdouble r);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord3dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord3f)(GLfloat s, GLfloat t, GLfloat r);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord3fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord3i)(GLint s, GLint t, GLint r);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord3iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord3s)(GLshort s, GLshort t, GLshort r);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord3sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord4d)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord4dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord4f)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord4fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord4i)(GLint s, GLint t, GLint r, GLint q);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord4iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord4s)(GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoord4sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glTexEnvf)(GLenum target, GLenum pname, GLfloat param);
typedef void (CODEGEN_FUNCPTR * Proc_glTexEnvfv)(GLenum target, GLenum pname, const GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glTexEnvi)(GLenum target, GLenum pname, GLint param);
typedef void (CODEGEN_FUNCPTR * Proc_glTexEnviv)(GLenum target, GLenum pname, const GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glTexGend)(GLenum coord, GLenum pname, GLdouble param);
typedef void (CODEGEN_FUNCPTR * Proc_glTexGendv)(GLenum coord, GLenum pname, const GLdouble * params);
typedef void (CODEGEN_FUNCPTR * Proc_glTexGenf)(GLenum coord, GLenum pname, GLfloat param);
typedef void (CODEGEN_FUNCPTR * Proc_glTexGenfv)(GLenum coord, GLenum pname, const GLfloat * params);
typedef void (CODEGEN_FUNCPTR * Proc_glTexGeni)(GLenum coord, GLenum pname, GLint param);
typedef void (CODEGEN_FUNCPTR * Proc_glTexGeniv)(GLenum coord, GLenum pname, const GLint * params);
typedef void (CODEGEN_FUNCPTR * Proc_glTranslated)(GLdouble x, GLdouble y, GLdouble z);
typedef void (CODEGEN_FUNCPTR * Proc_glTranslatef)(GLfloat x, GLfloat y, GLfloat z);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex2d)(GLdouble x, GLdouble y);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex2dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex2f)(GLfloat x, GLfloat y);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex2fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex2i)(GLint x, GLint y);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex2iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex2s)(GLshort x, GLshort y);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex2sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex3d)(GLdouble x, GLdouble y, GLdouble z);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex3dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex3f)(GLfloat x, GLfloat y, GLfloat z);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex3fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex3i)(GLint x, GLint y, GLint z);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex3iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex3s)(GLshort x, GLshort y, GLshort z);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex3sv)(const GLshort * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex4dv)(const GLdouble * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex4fv)(const GLfloat * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex4i)(GLint x, GLint y, GLint z, GLint w);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex4iv)(const GLint * v);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex4s)(GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (CODEGEN_FUNCPTR * Proc_glVertex4sv)(const GLshort * v);
}

extern _detail::Proc_glAccum Accum;
extern _detail::Proc_glAlphaFunc AlphaFunc;
extern _detail::Proc_glBegin Begin;
extern _detail::Proc_glBitmap Bitmap;
extern _detail::Proc_glCallList CallList;
extern _detail::Proc_glCallLists CallLists;
extern _detail::Proc_glClearAccum ClearAccum;
extern _detail::Proc_glClearIndex ClearIndex;
extern _detail::Proc_glClipPlane ClipPlane;
extern _detail::Proc_glColor3b Color3b;
extern _detail::Proc_glColor3bv Color3bv;
extern _detail::Proc_glColor3d Color3d;
extern _detail::Proc_glColor3dv Color3dv;
extern _detail::Proc_glColor3f Color3f;
extern _detail::Proc_glColor3fv Color3fv;
extern _detail::Proc_glColor3i Color3i;
extern _detail::Proc_glColor3iv Color3iv;
extern _detail::Proc_glColor3s Color3s;
extern _detail::Proc_glColor3sv Color3sv;
extern _detail::Proc_glColor3ub Color3ub;
extern _detail::Proc_glColor3ubv Color3ubv;
extern _detail::Proc_glColor3ui Color3ui;
extern _detail::Proc_glColor3uiv Color3uiv;
extern _detail::Proc_glColor3us Color3us;
extern _detail::Proc_glColor3usv Color3usv;
extern _detail::Proc_glColor4b Color4b;
extern _detail::Proc_glColor4bv Color4bv;
extern _detail::Proc_glColor4d Color4d;
extern _detail::Proc_glColor4dv Color4dv;
extern _detail::Proc_glColor4f Color4f;
extern _detail::Proc_glColor4fv Color4fv;
extern _detail::Proc_glColor4i Color4i;
extern _detail::Proc_glColor4iv Color4iv;
extern _detail::Proc_glColor4s Color4s;
extern _detail::Proc_glColor4sv Color4sv;
extern _detail::Proc_glColor4ub Color4ub;
extern _detail::Proc_glColor4ubv Color4ubv;
extern _detail::Proc_glColor4ui Color4ui;
extern _detail::Proc_glColor4uiv Color4uiv;
extern _detail::Proc_glColor4us Color4us;
extern _detail::Proc_glColor4usv Color4usv;
extern _detail::Proc_glColorMaterial ColorMaterial;
extern _detail::Proc_glCopyPixels CopyPixels;
extern _detail::Proc_glDeleteLists DeleteLists;
extern _detail::Proc_glDrawPixels DrawPixels;
extern _detail::Proc_glEdgeFlag EdgeFlag;
extern _detail::Proc_glEdgeFlagv EdgeFlagv;
extern _detail::Proc_glEnd End;
extern _detail::Proc_glEndList EndList;
extern _detail::Proc_glEvalCoord1d EvalCoord1d;
extern _detail::Proc_glEvalCoord1dv EvalCoord1dv;
extern _detail::Proc_glEvalCoord1f EvalCoord1f;
extern _detail::Proc_glEvalCoord1fv EvalCoord1fv;
extern _detail::Proc_glEvalCoord2d EvalCoord2d;
extern _detail::Proc_glEvalCoord2dv EvalCoord2dv;
extern _detail::Proc_glEvalCoord2f EvalCoord2f;
extern _detail::Proc_glEvalCoord2fv EvalCoord2fv;
extern _detail::Proc_glEvalMesh1 EvalMesh1;
extern _detail::Proc_glEvalMesh2 EvalMesh2;
extern _detail::Proc_glEvalPoint1 EvalPoint1;
extern _detail::Proc_glEvalPoint2 EvalPoint2;
extern _detail::Proc_glFeedbackBuffer FeedbackBuffer;
extern _detail::Proc_glFogf Fogf;
extern _detail::Proc_glFogfv Fogfv;
extern _detail::Proc_glFogi Fogi;
extern _detail::Proc_glFogiv Fogiv;
extern _detail::Proc_glFrustum Frustum;
extern _detail::Proc_glGenLists GenLists;
extern _detail::Proc_glGetClipPlane GetClipPlane;
extern _detail::Proc_glGetLightfv GetLightfv;
extern _detail::Proc_glGetLightiv GetLightiv;
extern _detail::Proc_glGetMapdv GetMapdv;
extern _detail::Proc_glGetMapfv GetMapfv;
extern _detail::Proc_glGetMapiv GetMapiv;
extern _detail::Proc_glGetMaterialfv GetMaterialfv;
extern _detail::Proc_glGetMaterialiv GetMaterialiv;
extern _detail::Proc_glGetPixelMapfv GetPixelMapfv;
extern _detail::Proc_glGetPixelMapuiv GetPixelMapuiv;
extern _detail::Proc_glGetPixelMapusv GetPixelMapusv;
extern _detail::Proc_glGetPolygonStipple GetPolygonStipple;
extern _detail::Proc_glGetTexEnvfv GetTexEnvfv;
extern _detail::Proc_glGetTexEnviv GetTexEnviv;
extern _detail::Proc_glGetTexGendv GetTexGendv;
extern _detail::Proc_glGetTexGenfv GetTexGenfv;
extern _detail::Proc_glGetTexGeniv GetTexGeniv;
extern _detail::Proc_glIndexMask IndexMask;
extern _detail::Proc_glIndexd Indexd;
extern _detail::Proc_glIndexdv Indexdv;
extern _detail::Proc_glIndexf Indexf;
extern _detail::Proc_glIndexfv Indexfv;
extern _detail::Proc_glIndexi Indexi;
extern _detail::Proc_glIndexiv Indexiv;
extern _detail::Proc_glIndexs Indexs;
extern _detail::Proc_glIndexsv Indexsv;
extern _detail::Proc_glInitNames InitNames;
extern _detail::Proc_glIsList IsList;
extern _detail::Proc_glLightModelf LightModelf;
extern _detail::Proc_glLightModelfv LightModelfv;
extern _detail::Proc_glLightModeli LightModeli;
extern _detail::Proc_glLightModeliv LightModeliv;
extern _detail::Proc_glLightf Lightf;
extern _detail::Proc_glLightfv Lightfv;
extern _detail::Proc_glLighti Lighti;
extern _detail::Proc_glLightiv Lightiv;
extern _detail::Proc_glLineStipple LineStipple;
extern _detail::Proc_glListBase ListBase;
extern _detail::Proc_glLoadIdentity LoadIdentity;
extern _detail::Proc_glLoadMatrixd LoadMatrixd;
extern _detail::Proc_glLoadMatrixf LoadMatrixf;
extern _detail::Proc_glLoadName LoadName;
extern _detail::Proc_glMap1d Map1d;
extern _detail::Proc_glMap1f Map1f;
extern _detail::Proc_glMap2d Map2d;
extern _detail::Proc_glMap2f Map2f;
extern _detail::Proc_glMapGrid1d MapGrid1d;
extern _detail::Proc_glMapGrid1f MapGrid1f;
extern _detail::Proc_glMapGrid2d MapGrid2d;
extern _detail::Proc_glMapGrid2f MapGrid2f;
extern _detail::Proc_glMaterialf Materialf;
extern _detail::Proc_glMaterialfv Materialfv;
extern _detail::Proc_glMateriali Materiali;
extern _detail::Proc_glMaterialiv Materialiv;
extern _detail::Proc_glMatrixMode MatrixMode;
extern _detail::Proc_glMultMatrixd MultMatrixd;
extern _detail::Proc_glMultMatrixf MultMatrixf;
extern _detail::Proc_glNewList NewList;
extern _detail::Proc_glNormal3b Normal3b;
extern _detail::Proc_glNormal3bv Normal3bv;
extern _detail::Proc_glNormal3d Normal3d;
extern _detail::Proc_glNormal3dv Normal3dv;
extern _detail::Proc_glNormal3f Normal3f;
extern _detail::Proc_glNormal3fv Normal3fv;
extern _detail::Proc_glNormal3i Normal3i;
extern _detail::Proc_glNormal3iv Normal3iv;
extern _detail::Proc_glNormal3s Normal3s;
extern _detail::Proc_glNormal3sv Normal3sv;
extern _detail::Proc_glOrtho Ortho;
extern _detail::Proc_glPassThrough PassThrough;
extern _detail::Proc_glPixelMapfv PixelMapfv;
extern _detail::Proc_glPixelMapuiv PixelMapuiv;
extern _detail::Proc_glPixelMapusv PixelMapusv;
extern _detail::Proc_glPixelTransferf PixelTransferf;
extern _detail::Proc_glPixelTransferi PixelTransferi;
extern _detail::Proc_glPixelZoom PixelZoom;
extern _detail::Proc_glPolygonStipple PolygonStipple;
extern _detail::Proc_glPopAttrib PopAttrib;
extern _detail::Proc_glPopMatrix PopMatrix;
extern _detail::Proc_glPopName PopName;
extern _detail::Proc_glPushAttrib PushAttrib;
extern _detail::Proc_glPushMatrix PushMatrix;
extern _detail::Proc_glPushName PushName;
extern _detail::Proc_glRasterPos2d RasterPos2d;
extern _detail::Proc_glRasterPos2dv RasterPos2dv;
extern _detail::Proc_glRasterPos2f RasterPos2f;
extern _detail::Proc_glRasterPos2fv RasterPos2fv;
extern _detail::Proc_glRasterPos2i RasterPos2i;
extern _detail::Proc_glRasterPos2iv RasterPos2iv;
extern _detail::Proc_glRasterPos2s RasterPos2s;
extern _detail::Proc_glRasterPos2sv RasterPos2sv;
extern _detail::Proc_glRasterPos3d RasterPos3d;
extern _detail::Proc_glRasterPos3dv RasterPos3dv;
extern _detail::Proc_glRasterPos3f RasterPos3f;
extern _detail::Proc_glRasterPos3fv RasterPos3fv;
extern _detail::Proc_glRasterPos3i RasterPos3i;
extern _detail::Proc_glRasterPos3iv RasterPos3iv;
extern _detail::Proc_glRasterPos3s RasterPos3s;
extern _detail::Proc_glRasterPos3sv RasterPos3sv;
extern _detail::Proc_glRasterPos4d RasterPos4d;
extern _detail::Proc_glRasterPos4dv RasterPos4dv;
extern _detail::Proc_glRasterPos4f RasterPos4f;
extern _detail::Proc_glRasterPos4fv RasterPos4fv;
extern _detail::Proc_glRasterPos4i RasterPos4i;
extern _detail::Proc_glRasterPos4iv RasterPos4iv;
extern _detail::Proc_glRasterPos4s RasterPos4s;
extern _detail::Proc_glRasterPos4sv RasterPos4sv;
extern _detail::Proc_glRectd Rectd;
extern _detail::Proc_glRectdv Rectdv;
extern _detail::Proc_glRectf Rectf;
extern _detail::Proc_glRectfv Rectfv;
extern _detail::Proc_glRecti Recti;
extern _detail::Proc_glRectiv Rectiv;
extern _detail::Proc_glRects Rects;
extern _detail::Proc_glRectsv Rectsv;
extern _detail::Proc_glRenderMode RenderMode;
extern _detail::Proc_glRotated Rotated;
extern _detail::Proc_glRotatef Rotatef;
extern _detail::Proc_glScaled Scaled;
extern _detail::Proc_glScalef Scalef;
extern _detail::Proc_glSelectBuffer SelectBuffer;
extern _detail::Proc_glShadeModel ShadeModel;
extern _detail::Proc_glTexCoord1d TexCoord1d;
extern _detail::Proc_glTexCoord1dv TexCoord1dv;
extern _detail::Proc_glTexCoord1f TexCoord1f;
extern _detail::Proc_glTexCoord1fv TexCoord1fv;
extern _detail::Proc_glTexCoord1i TexCoord1i;
extern _detail::Proc_glTexCoord1iv TexCoord1iv;
extern _detail::Proc_glTexCoord1s TexCoord1s;
extern _detail::Proc_glTexCoord1sv TexCoord1sv;
extern _detail::Proc_glTexCoord2d TexCoord2d;
extern _detail::Proc_glTexCoord2dv TexCoord2dv;
extern _detail::Proc_glTexCoord2f TexCoord2f;
extern _detail::Proc_glTexCoord2fv TexCoord2fv;
extern _detail::Proc_glTexCoord2i TexCoord2i;
extern _detail::Proc_glTexCoord2iv TexCoord2iv;
extern _detail::Proc_glTexCoord2s TexCoord2s;
extern _detail::Proc_glTexCoord2sv TexCoord2sv;
extern _detail::Proc_glTexCoord3d TexCoord3d;
extern _detail::Proc_glTexCoord3dv TexCoord3dv;
extern _detail::Proc_glTexCoord3f TexCoord3f;
extern _detail::Proc_glTexCoord3fv TexCoord3fv;
extern _detail::Proc_glTexCoord3i TexCoord3i;
extern _detail::Proc_glTexCoord3iv TexCoord3iv;
extern _detail::Proc_glTexCoord3s TexCoord3s;
extern _detail::Proc_glTexCoord3sv TexCoord3sv;
extern _detail::Proc_glTexCoord4d TexCoord4d;
extern _detail::Proc_glTexCoord4dv TexCoord4dv;
extern _detail::Proc_glTexCoord4f TexCoord4f;
extern _detail::Proc_glTexCoord4fv TexCoord4fv;
extern _detail::Proc_glTexCoord4i TexCoord4i;
extern _detail::Proc_glTexCoord4iv TexCoord4iv;
extern _detail::Proc_glTexCoord4s TexCoord4s;
extern _detail::Proc_glTexCoord4sv TexCoord4sv;
extern _detail::Proc_glTexEnvf TexEnvf;
extern _detail::Proc_glTexEnvfv TexEnvfv;
extern _detail::Proc_glTexEnvi TexEnvi;
extern _detail::Proc_glTexEnviv TexEnviv;
extern _detail::Proc_glTexGend TexGend;
extern _detail::Proc_glTexGendv TexGendv;
extern _detail::Proc_glTexGenf TexGenf;
extern _detail::Proc_glTexGenfv TexGenfv;
extern _detail::Proc_glTexGeni TexGeni;
extern _detail::Proc_glTexGeniv TexGeniv;
extern _detail::Proc_glTranslated Translated;
extern _detail::Proc_glTranslatef Translatef;
extern _detail::Proc_glVertex2d Vertex2d;
extern _detail::Proc_glVertex2dv Vertex2dv;
extern _detail::Proc_glVertex2f Vertex2f;
extern _detail::Proc_glVertex2fv Vertex2fv;
extern _detail::Proc_glVertex2i Vertex2i;
extern _detail::Proc_glVertex2iv Vertex2iv;
extern _detail::Proc_glVertex2s Vertex2s;
extern _detail::Proc_glVertex2sv Vertex2sv;
extern _detail::Proc_glVertex3d Vertex3d;
extern _detail::Proc_glVertex3dv Vertex3dv;
extern _detail::Proc_glVertex3f Vertex3f;
extern _detail::Proc_glVertex3fv Vertex3fv;
extern _detail::Proc_glVertex3i Vertex3i;
extern _detail::Proc_glVertex3iv Vertex3iv;
extern _detail::Proc_glVertex3s Vertex3s;
extern _detail::Proc_glVertex3sv Vertex3sv;
extern _detail::Proc_glVertex4d Vertex4d;
extern _detail::Proc_glVertex4dv Vertex4dv;
extern _detail::Proc_glVertex4f Vertex4f;
extern _detail::Proc_glVertex4fv Vertex4fv;
extern _detail::Proc_glVertex4i Vertex4i;
extern _detail::Proc_glVertex4iv Vertex4iv;
extern _detail::Proc_glVertex4s Vertex4s;
extern _detail::Proc_glVertex4sv Vertex4sv;

}
#endif /*OPENGL_GEN_CORE_REM1_0_HPP*/
