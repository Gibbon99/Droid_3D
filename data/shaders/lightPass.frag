#version 330

uniform sampler2D   tPosition;
uniform sampler2D   tNormals;
uniform sampler2D   tDiffuse;

in vec2 fragTexCoord0;

out vec4 outColor;

uniform vec3 cameraPosition;

void main()
{
	vec3 albedo = texture(tDiffuse, fragTexCoord0).xyz;
	vec3 n = normalize(texture(tNormals, fragTexCoord0).xyz);
	vec3 pos = texture(tPosition, fragTexCoord0).xyz;

	vec3 l = normalize(vec3(-0.7, 0.3, 0.1));
	vec3 v = normalize(cameraPosition - pos);
	vec3 h = normalize(l + v);

	vec3 color =
	// diffuse
	0.7 * albedo.xyz * max(0.0, dot(n.xyz, l)) +
	// specular
	0.4 * pow(max(0.0, dot(h, n)), 32.0) +
	// ambient.
	0.6 * albedo.xyz;

	outColor = vec4(color, 1.0);


	outColor.rgb = texture(tDiffuse, fragTexCoord0).rgb;
}