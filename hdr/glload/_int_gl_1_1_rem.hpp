#ifndef OPENGL_GEN_CORE_REM1_1_HPP
#define OPENGL_GEN_CORE_REM1_1_HPP

#include "_int_load_test.hpp"
namespace gl
{
enum
{
	_2D                              = 0x0600,
	_2_BYTES                         = 0x1407,
	_3D                              = 0x0601,
	_3D_COLOR                        = 0x0602,
	_3D_COLOR_TEXTURE                = 0x0603,
	_3_BYTES                         = 0x1408,
	_4D_COLOR_TEXTURE                = 0x0604,
	_4_BYTES                         = 0x1409,
	ACCUM                            = 0x0100,
	ACCUM_ALPHA_BITS                 = 0x0D5B,
	ACCUM_BLUE_BITS                  = 0x0D5A,
	ACCUM_BUFFER_BIT                 = 0x00000200,
	ACCUM_CLEAR_VALUE                = 0x0B80,
	ACCUM_GREEN_BITS                 = 0x0D59,
	ACCUM_RED_BITS                   = 0x0D58,
	ADD                              = 0x0104,
	ALL_ATTRIB_BITS                  = 0xFFFFFFFF,
	ALPHA12                          = 0x803D,
	ALPHA16                          = 0x803E,
	ALPHA4                           = 0x803B,
	ALPHA8                           = 0x803C,
	ALPHA_BIAS                       = 0x0D1D,
	ALPHA_BITS                       = 0x0D55,
	ALPHA_SCALE                      = 0x0D1C,
	ALPHA_TEST                       = 0x0BC0,
	ALPHA_TEST_FUNC                  = 0x0BC1,
	ALPHA_TEST_REF                   = 0x0BC2,
	AMBIENT                          = 0x1200,
	AMBIENT_AND_DIFFUSE              = 0x1602,
	ATTRIB_STACK_DEPTH               = 0x0BB0,
	AUTO_NORMAL                      = 0x0D80,
	AUX0                             = 0x0409,
	AUX1                             = 0x040A,
	AUX2                             = 0x040B,
	AUX3                             = 0x040C,
	AUX_BUFFERS                      = 0x0C00,
	BITMAP                           = 0x1A00,
	BITMAP_TOKEN                     = 0x0704,
	BLUE_BIAS                        = 0x0D1B,
	BLUE_BITS                        = 0x0D54,
	BLUE_SCALE                       = 0x0D1A,
	C3F_V3F                          = 0x2A24,
	C4F_N3F_V3F                      = 0x2A26,
	C4UB_V2F                         = 0x2A22,
	C4UB_V3F                         = 0x2A23,
	CLAMP                            = 0x2900,
	CLIENT_ALL_ATTRIB_BITS           = 0xFFFFFFFF,
	CLIENT_ATTRIB_STACK_DEPTH        = 0x0BB1,
	CLIENT_PIXEL_STORE_BIT           = 0x00000001,
	CLIENT_VERTEX_ARRAY_BIT          = 0x00000002,
	CLIP_PLANE0                      = 0x3000,
	CLIP_PLANE1                      = 0x3001,
	CLIP_PLANE2                      = 0x3002,
	CLIP_PLANE3                      = 0x3003,
	CLIP_PLANE4                      = 0x3004,
	CLIP_PLANE5                      = 0x3005,
	COEFF                            = 0x0A00,
	COLOR_ARRAY                      = 0x8076,
	COLOR_ARRAY_POINTER              = 0x8090,
	COLOR_ARRAY_SIZE                 = 0x8081,
	COLOR_ARRAY_STRIDE               = 0x8083,
	COLOR_ARRAY_TYPE                 = 0x8082,
	COLOR_INDEX                      = 0x1900,
	COLOR_INDEXES                    = 0x1603,
	COLOR_MATERIAL                   = 0x0B57,
	COLOR_MATERIAL_FACE              = 0x0B55,
	COLOR_MATERIAL_PARAMETER         = 0x0B56,
	COMPILE                          = 0x1300,
	COMPILE_AND_EXECUTE              = 0x1301,
	CONSTANT_ATTENUATION             = 0x1207,
	COPY_PIXEL_TOKEN                 = 0x0706,
	CURRENT_BIT                      = 0x00000001,
	CURRENT_COLOR                    = 0x0B00,
	CURRENT_INDEX                    = 0x0B01,
	CURRENT_NORMAL                   = 0x0B02,
	CURRENT_RASTER_COLOR             = 0x0B04,
	CURRENT_RASTER_DISTANCE          = 0x0B09,
	CURRENT_RASTER_INDEX             = 0x0B05,
	CURRENT_RASTER_POSITION          = 0x0B07,
	CURRENT_RASTER_POSITION_VALID    = 0x0B08,
	CURRENT_RASTER_TEXTURE_COORDS    = 0x0B06,
	CURRENT_TEXTURE_COORDS           = 0x0B03,
	DECAL                            = 0x2101,
	DEPTH_BIAS                       = 0x0D1F,
	DEPTH_BITS                       = 0x0D56,
	DEPTH_SCALE                      = 0x0D1E,
	DIFFUSE                          = 0x1201,
	DOMAIN_                          = 0x0A02,
	DRAW_PIXEL_TOKEN                 = 0x0705,
	EDGE_FLAG                        = 0x0B43,
	EDGE_FLAG_ARRAY                  = 0x8079,
	EDGE_FLAG_ARRAY_POINTER          = 0x8093,
	EDGE_FLAG_ARRAY_STRIDE           = 0x808C,
	EMISSION                         = 0x1600,
	ENABLE_BIT                       = 0x00002000,
	EVAL_BIT                         = 0x00010000,
	EXP                              = 0x0800,
	EXP2                             = 0x0801,
	EYE_LINEAR                       = 0x2400,
	FEEDBACK                         = 0x1C01,
	FEEDBACK_BUFFER_POINTER          = 0x0DF0,
	FEEDBACK_BUFFER_SIZE             = 0x0DF1,
	FEEDBACK_BUFFER_TYPE             = 0x0DF2,
	FLAT                             = 0x1D00,
	FOG_BIT                          = 0x00000080,
	FOG_COLOR                        = 0x0B66,
	FOG_DENSITY                      = 0x0B62,
	FOG_END                          = 0x0B64,
	FOG_HINT                         = 0x0C54,
	FOG_INDEX                        = 0x0B61,
	FOG_MODE                         = 0x0B65,
	FOG_START                        = 0x0B63,
	GREEN_BIAS                       = 0x0D19,
	GREEN_BITS                       = 0x0D53,
	GREEN_SCALE                      = 0x0D18,
	HINT_BIT                         = 0x00008000,
	INDEX_ARRAY                      = 0x8077,
	INDEX_ARRAY_POINTER              = 0x8091,
	INDEX_ARRAY_STRIDE               = 0x8086,
	INDEX_ARRAY_TYPE                 = 0x8085,
	INDEX_BITS                       = 0x0D51,
	INDEX_CLEAR_VALUE                = 0x0C20,
	INDEX_LOGIC_OP                   = 0x0BF1,
	INDEX_MODE                       = 0x0C30,
	INDEX_OFFSET                     = 0x0D13,
	INDEX_SHIFT                      = 0x0D12,
	INDEX_WRITEMASK                  = 0x0C21,
	INTENSITY                        = 0x8049,
	INTENSITY12                      = 0x804C,
	INTENSITY16                      = 0x804D,
	INTENSITY4                       = 0x804A,
	INTENSITY8                       = 0x804B,
	LIGHT0                           = 0x4000,
	LIGHT1                           = 0x4001,
	LIGHT2                           = 0x4002,
	LIGHT3                           = 0x4003,
	LIGHT4                           = 0x4004,
	LIGHT5                           = 0x4005,
	LIGHT6                           = 0x4006,
	LIGHT7                           = 0x4007,
	LIGHTING                         = 0x0B50,
	LIGHTING_BIT                     = 0x00000040,
	LIGHT_MODEL_AMBIENT              = 0x0B53,
	LIGHT_MODEL_LOCAL_VIEWER         = 0x0B51,
	LIGHT_MODEL_TWO_SIDE             = 0x0B52,
	LINEAR_ATTENUATION               = 0x1208,
	LINE_BIT                         = 0x00000004,
	LINE_RESET_TOKEN                 = 0x0707,
	LINE_STIPPLE                     = 0x0B24,
	LINE_STIPPLE_PATTERN             = 0x0B25,
	LINE_STIPPLE_REPEAT              = 0x0B26,
	LINE_TOKEN                       = 0x0702,
	LIST_BASE                        = 0x0B32,
	LIST_BIT                         = 0x00020000,
	LIST_INDEX                       = 0x0B33,
	LIST_MODE                        = 0x0B30,
	LOAD                             = 0x0101,
	LOGIC_OP                         = 0x0BF1,
	LUMINANCE                        = 0x1909,
	LUMINANCE12                      = 0x8041,
	LUMINANCE12_ALPHA12              = 0x8047,
	LUMINANCE12_ALPHA4               = 0x8046,
	LUMINANCE16                      = 0x8042,
	LUMINANCE16_ALPHA16              = 0x8048,
	LUMINANCE4                       = 0x803F,
	LUMINANCE4_ALPHA4                = 0x8043,
	LUMINANCE6_ALPHA2                = 0x8044,
	LUMINANCE8                       = 0x8040,
	LUMINANCE8_ALPHA8                = 0x8045,
	LUMINANCE_ALPHA                  = 0x190A,
	MAP1_COLOR_4                     = 0x0D90,
	MAP1_GRID_DOMAIN                 = 0x0DD0,
	MAP1_GRID_SEGMENTS               = 0x0DD1,
	MAP1_INDEX                       = 0x0D91,
	MAP1_NORMAL                      = 0x0D92,
	MAP1_TEXTURE_COORD_1             = 0x0D93,
	MAP1_TEXTURE_COORD_2             = 0x0D94,
	MAP1_TEXTURE_COORD_3             = 0x0D95,
	MAP1_TEXTURE_COORD_4             = 0x0D96,
	MAP1_VERTEX_3                    = 0x0D97,
	MAP1_VERTEX_4                    = 0x0D98,
	MAP2_COLOR_4                     = 0x0DB0,
	MAP2_GRID_DOMAIN                 = 0x0DD2,
	MAP2_GRID_SEGMENTS               = 0x0DD3,
	MAP2_INDEX                       = 0x0DB1,
	MAP2_NORMAL                      = 0x0DB2,
	MAP2_TEXTURE_COORD_1             = 0x0DB3,
	MAP2_TEXTURE_COORD_2             = 0x0DB4,
	MAP2_TEXTURE_COORD_3             = 0x0DB5,
	MAP2_TEXTURE_COORD_4             = 0x0DB6,
	MAP2_VERTEX_3                    = 0x0DB7,
	MAP2_VERTEX_4                    = 0x0DB8,
	MAP_COLOR                        = 0x0D10,
	MAP_STENCIL                      = 0x0D11,
	MATRIX_MODE                      = 0x0BA0,
	MAX_ATTRIB_STACK_DEPTH           = 0x0D35,
	MAX_CLIENT_ATTRIB_STACK_DEPTH    = 0x0D3B,
	MAX_CLIP_PLANES                  = 0x0D32,
	MAX_EVAL_ORDER                   = 0x0D30,
	MAX_LIGHTS                       = 0x0D31,
	MAX_LIST_NESTING                 = 0x0B31,
	MAX_MODELVIEW_STACK_DEPTH        = 0x0D36,
	MAX_NAME_STACK_DEPTH             = 0x0D37,
	MAX_PIXEL_MAP_TABLE              = 0x0D34,
	MAX_PROJECTION_STACK_DEPTH       = 0x0D38,
	MAX_TEXTURE_STACK_DEPTH          = 0x0D39,
	MODELVIEW                        = 0x1700,
	MODELVIEW_MATRIX                 = 0x0BA6,
	MODELVIEW_STACK_DEPTH            = 0x0BA3,
	MODULATE                         = 0x2100,
	MULT                             = 0x0103,
	N3F_V3F                          = 0x2A25,
	NAME_STACK_DEPTH                 = 0x0D70,
	NORMALIZE                        = 0x0BA1,
	NORMAL_ARRAY                     = 0x8075,
	NORMAL_ARRAY_POINTER             = 0x808F,
	NORMAL_ARRAY_STRIDE              = 0x807F,
	NORMAL_ARRAY_TYPE                = 0x807E,
	OBJECT_LINEAR                    = 0x2401,
	OBJECT_PLANE                     = 0x2501,
	ORDER                            = 0x0A01,
	PASS_THROUGH_TOKEN               = 0x0700,
	PERSPECTIVE_CORRECTION_HINT      = 0x0C50,
	PIXEL_MAP_A_TO_A                 = 0x0C79,
	PIXEL_MAP_A_TO_A_SIZE            = 0x0CB9,
	PIXEL_MAP_B_TO_B                 = 0x0C78,
	PIXEL_MAP_B_TO_B_SIZE            = 0x0CB8,
	PIXEL_MAP_G_TO_G                 = 0x0C77,
	PIXEL_MAP_G_TO_G_SIZE            = 0x0CB7,
	PIXEL_MAP_I_TO_A                 = 0x0C75,
	PIXEL_MAP_I_TO_A_SIZE            = 0x0CB5,
	PIXEL_MAP_I_TO_B                 = 0x0C74,
	PIXEL_MAP_I_TO_B_SIZE            = 0x0CB4,
	PIXEL_MAP_I_TO_G                 = 0x0C73,
	PIXEL_MAP_I_TO_G_SIZE            = 0x0CB3,
	PIXEL_MAP_I_TO_I                 = 0x0C70,
	PIXEL_MAP_I_TO_I_SIZE            = 0x0CB0,
	PIXEL_MAP_I_TO_R                 = 0x0C72,
	PIXEL_MAP_I_TO_R_SIZE            = 0x0CB2,
	PIXEL_MAP_R_TO_R                 = 0x0C76,
	PIXEL_MAP_R_TO_R_SIZE            = 0x0CB6,
	PIXEL_MAP_S_TO_S                 = 0x0C71,
	PIXEL_MAP_S_TO_S_SIZE            = 0x0CB1,
	PIXEL_MODE_BIT                   = 0x00000020,
	POINT_BIT                        = 0x00000002,
	POINT_SMOOTH                     = 0x0B10,
	POINT_SMOOTH_HINT                = 0x0C51,
	POINT_TOKEN                      = 0x0701,
	POLYGON                          = 0x0009,
	POLYGON_BIT                      = 0x00000008,
	POLYGON_STIPPLE                  = 0x0B42,
	POLYGON_STIPPLE_BIT              = 0x00000010,
	POLYGON_TOKEN                    = 0x0703,
	POSITION                         = 0x1203,
	PROJECTION                       = 0x1701,
	PROJECTION_MATRIX                = 0x0BA7,
	PROJECTION_STACK_DEPTH           = 0x0BA4,
	Q                                = 0x2003,
	QUADRATIC_ATTENUATION            = 0x1209,
	QUAD_STRIP                       = 0x0008,
	R                                = 0x2002,
	RED_BIAS                         = 0x0D15,
	RED_BITS                         = 0x0D52,
	RED_SCALE                        = 0x0D14,
	RENDER                           = 0x1C00,
	RENDER_MODE                      = 0x0C40,
	RETURN                           = 0x0102,
	RGBA_MODE                        = 0x0C31,
	S                                = 0x2000,
	SCISSOR_BIT                      = 0x00080000,
	SELECT                           = 0x1C02,
	SELECTION_BUFFER_POINTER         = 0x0DF3,
	SELECTION_BUFFER_SIZE            = 0x0DF4,
	SHADE_MODEL                      = 0x0B54,
	SHININESS                        = 0x1601,
	SMOOTH                           = 0x1D01,
	SPECULAR                         = 0x1202,
	SPHERE_MAP                       = 0x2402,
	SPOT_CUTOFF                      = 0x1206,
	SPOT_DIRECTION                   = 0x1204,
	SPOT_EXPONENT                    = 0x1205,
	STENCIL_BITS                     = 0x0D57,
	T                                = 0x2001,
	T2F_C3F_V3F                      = 0x2A2A,
	T2F_C4F_N3F_V3F                  = 0x2A2C,
	T2F_C4UB_V3F                     = 0x2A29,
	T2F_N3F_V3F                      = 0x2A2B,
	T2F_V3F                          = 0x2A27,
	T4F_C4F_N3F_V4F                  = 0x2A2D,
	T4F_V4F                          = 0x2A28,
	TEXTURE_BIT                      = 0x00040000,
	TEXTURE_BORDER                   = 0x1005,
	TEXTURE_COMPONENTS               = 0x1003,
	TEXTURE_COORD_ARRAY              = 0x8078,
	TEXTURE_COORD_ARRAY_POINTER      = 0x8092,
	TEXTURE_COORD_ARRAY_SIZE         = 0x8088,
	TEXTURE_COORD_ARRAY_STRIDE       = 0x808A,
	TEXTURE_COORD_ARRAY_TYPE         = 0x8089,
	TEXTURE_ENV                      = 0x2300,
	TEXTURE_ENV_COLOR                = 0x2201,
	TEXTURE_ENV_MODE                 = 0x2200,
	TEXTURE_GEN_MODE                 = 0x2500,
	TEXTURE_GEN_Q                    = 0x0C63,
	TEXTURE_GEN_R                    = 0x0C62,
	TEXTURE_GEN_S                    = 0x0C60,
	TEXTURE_GEN_T                    = 0x0C61,
	TEXTURE_INTENSITY_SIZE           = 0x8061,
	TEXTURE_LUMINANCE_SIZE           = 0x8060,
	TEXTURE_MATRIX                   = 0x0BA8,
	TEXTURE_PRIORITY                 = 0x8066,
	TEXTURE_RESIDENT                 = 0x8067,
	TEXTURE_STACK_DEPTH              = 0x0BA5,
	TRANSFORM_BIT                    = 0x00001000,
	V2F                              = 0x2A20,
	V3F                              = 0x2A21,
	VERTEX_ARRAY_POINTER             = 0x808E,
	VERTEX_ARRAY_SIZE                = 0x807A,
	VERTEX_ARRAY_STRIDE              = 0x807C,
	VERTEX_ARRAY_TYPE                = 0x807B,
	VIEWPORT_BIT                     = 0x00000800,
	ZOOM_X                           = 0x0D16,
	ZOOM_Y                           = 0x0D17,
};

namespace _detail
{
typedef GLboolean (CODEGEN_FUNCPTR * Proc_glAreTexturesResident)(GLsizei n, const GLuint * textures, GLboolean * residences);
typedef void (CODEGEN_FUNCPTR * Proc_glArrayElement)(GLint i);
typedef void (CODEGEN_FUNCPTR * Proc_glColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (CODEGEN_FUNCPTR * Proc_glDisableClientState)(GLenum ren_array);
typedef void (CODEGEN_FUNCPTR * Proc_glEdgeFlagPointer)(GLsizei stride, const GLvoid * pointer);
typedef void (CODEGEN_FUNCPTR * Proc_glEnableClientState)(GLenum ren_array);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexPointer)(GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexub)(GLubyte c);
typedef void (CODEGEN_FUNCPTR * Proc_glIndexubv)(const GLubyte * c);
typedef void (CODEGEN_FUNCPTR * Proc_glInterleavedArrays)(GLenum format, GLsizei stride, const GLvoid * pointer);
typedef void (CODEGEN_FUNCPTR * Proc_glNormalPointer)(GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (CODEGEN_FUNCPTR * Proc_glPopClientAttrib)();
typedef void (CODEGEN_FUNCPTR * Proc_glPrioritizeTextures)(GLsizei n, const GLuint * textures, const GLfloat * priorities);
typedef void (CODEGEN_FUNCPTR * Proc_glPushClientAttrib)(GLbitfield mask);
typedef void (CODEGEN_FUNCPTR * Proc_glTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (CODEGEN_FUNCPTR * Proc_glVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
}

extern _detail::Proc_glAreTexturesResident AreTexturesResident;
extern _detail::Proc_glArrayElement ArrayElement;
extern _detail::Proc_glColorPointer ColorPointer;
extern _detail::Proc_glDisableClientState DisableClientState;
extern _detail::Proc_glEdgeFlagPointer EdgeFlagPointer;
extern _detail::Proc_glEnableClientState EnableClientState;
extern _detail::Proc_glIndexPointer IndexPointer;
extern _detail::Proc_glIndexub Indexub;
extern _detail::Proc_glIndexubv Indexubv;
extern _detail::Proc_glInterleavedArrays InterleavedArrays;
extern _detail::Proc_glNormalPointer NormalPointer;
extern _detail::Proc_glPopClientAttrib PopClientAttrib;
extern _detail::Proc_glPrioritizeTextures PrioritizeTextures;
extern _detail::Proc_glPushClientAttrib PushClientAttrib;
extern _detail::Proc_glTexCoordPointer TexCoordPointer;
extern _detail::Proc_glVertexPointer VertexPointer;

}
#endif /*OPENGL_GEN_CORE_REM1_1_HPP*/
