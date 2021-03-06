#version 330

uniform sampler2D   inTexture0;

layout (points) in;
layout (triangle_strip) out;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 u_viewProjectionMat;
uniform vec3 gCameraPos;

in float alphaValue_geom[];
in float sizeValue_geom[];

out float alphaValue_frag;
out float sizeValue_frag;

out vec2 TexCoord0;

void main()
{
	alphaValue_frag = alphaValue_geom[0];
	sizeValue_frag = sizeValue_geom[0];

	vec3 Pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(gCameraPos - Pos);
	vec3 up = vec3(0.0, 1.0, 0.0);
//	vec3 right = cross(toCamera, up);
	vec3 right = cross(up, toCamera);

	right = right * (sizeValue_geom[0] + 0.5f);
	Pos -= right;
	gl_Position = u_viewProjectionMat * vec4(Pos, 1.0);
	TexCoord0 = vec2(0.0, 0.0);
	EmitVertex();

	Pos.y += sizeValue_geom[0];
	gl_Position = u_viewProjectionMat * vec4(Pos, 1.0);
	TexCoord0 = vec2(0.0, 1.0);
	EmitVertex();

	Pos.y -= sizeValue_geom[0];
	Pos += right;
	gl_Position = u_viewProjectionMat * vec4(Pos, 1.0);
	TexCoord0 = vec2(1.0, 0.0);
	EmitVertex();

	Pos.y += sizeValue_geom[0];
	gl_Position = u_viewProjectionMat * vec4(Pos, 1.0);
	TexCoord0 = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}