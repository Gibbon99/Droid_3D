#version 330

uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uDiffuseTex;
uniform sampler2D uDepthTex;

out vec4 outColor;

in vec4 fsPos;

uniform float uLightRadius;
uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform vec3 cameraPosition;

void main()
{
	
// Check size of passed in color - from entities - could be 0 - 255

	// get screen-space position of light sphere
	// (remember to do perspective division.)
	vec2 uv = (fsPos.xy / fsPos.w) * 0.5 + 0.5;

	// now we can sample from the gbuffer for every fragment the light sphere covers.
	vec3 albedo = texture(uDiffuseTex, uv).xyz;
	vec3 n = normalize(texture(uNormalTex, uv).xyz);
	vec3 pos = texture(uPositionTex, uv).xyz;

	vec3 lightToPosVector = pos.xyz - uLightPosition;
	float lightDist = length(lightToPosVector);  // position from light.

	// implement fake z-test. If too far from light center, then 0.
	float ztest = step(0.0, uLightRadius - lightDist);
	
	// light attenuation.
	float d = lightDist / uLightRadius;
	float attenuation = 1.0 - d;
	
	attenuation /= 10.0f;
	
	outColor = vec4(albedo, 0.40f ) * vec4(uLightColor * attenuation, 0.9f);
}