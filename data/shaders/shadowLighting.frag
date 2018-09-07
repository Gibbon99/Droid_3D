#version 330 core
out vec4 FragColor;

struct s_lightData
{
	vec3 s_lightPosition;   // 0,1,2
	float s_lightSize;      // Pad to 3 (GLSL aligns to 4 bytes)
	vec3 s_lightColor;
};

layout (std140) uniform lightDataBlock
{
	s_lightData lightData[5];
};

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform int loopCount;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 viewPos;
uniform float shadowLevel;
uniform float far_planeTwo;

uniform float lightConstant = 10.0f;
uniform float lightLinear = 0.22f;
uniform float lightQuadratic = 0.020f; //0.0032f;     // Light casting size - lower is bigger

// array of offset direction for sampling
vec3 gridSamplingDisk[20]     = vec3[]
		(
				vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
				vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
				vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
				vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
				vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
		);

//-----------------------------------------------------------------------
//
// See if this fragment is in shadow or not
float ShadowCalculation(vec3 fragPos, int index)
//-----------------------------------------------------------------------
{
// get vector between fragment position and light position
	vec3 fragToLight          = fragPos - lightData[index].s_lightPosition;
	float currentDepth        = length(fragToLight);
	float shadow              = 0.0;
	float bias                = 0.45;
	int samples               = 20;
	float viewDistance        = length(viewPos - fragPos);
	float diskRadius          = (5.0 + (viewDistance / far_planeTwo)) / 5.0;

	for ( int i = 0; i < samples; ++i )
	{
		float closestDepth    = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth          *= far_planeTwo;   // undo mapping [0;1]
		if (currentDepth - bias > closestDepth)
			shadow            += 1.0;
	}

	shadow /= float(samples);

	return shadow;
}

//-----------------------------------------------------------------------
//
// Effect of lights on this fragment
vec3 CalcPointLight(int index, vec3 normal, vec3 fragPos, vec3 viewDir)
//-----------------------------------------------------------------------
{
	vec3 lightDiffuse;
	vec3 AB;
	vec3 ambient;
	vec3 diffuse;

	//
	// get Texture color
	diffuse = vec3(texture(diffuseTexture, fs_in.TexCoords));
	ambient = vec3(0.2, 0.2, 0.2);

	AB = fragPos - lightData[index].s_lightPosition;

	float dotResult = dot(AB, normal);

//	lightDiffuse = vec3(0.2, 0.2, 0.2);

//	vec3 lightDir = normalize(lightData[index].s_lightPosition - fragPos);

	// attenuation
	float distance    = length(lightData[index].s_lightPosition - fragPos);
	float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));

	/*
	// combine results
	if (dotResult < 0.5)
		ambient  = lightData[index].s_lightColor  * vec3(texture(diffuseTexture, fs_in.TexCoords));
	else
		ambient = vec3(texture(diffuseTexture, fs_in.TexCoords));
*/
	ambient  = lightData[index].s_lightColor  * vec3(texture(diffuseTexture, fs_in.TexCoords));

	ambient  *= attenuation;
	diffuse  *= attenuation;

	return (ambient + diffuse);
}

//-----------------------------------------------------------------------
//
// Run shader
void main()
//-----------------------------------------------------------------------
{
	// properties
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	vec3 result;
	float shadow;

	for (int i = 0; i != 1; i++)
	{
		result += CalcPointLight(i, norm, fs_in.FragPos, -viewDir);

		//shadow = ShadowCalculation (fs_in.FragPos, i);


		vec3 diffuseColor = texture (diffuseTexture, fs_in.TexCoords).rgb;

		// ambient
		vec3 ambient = shadowLevel * diffuseColor;        // Shadow darkness
		shadow = ShadowCalculation (fs_in.FragPos, i);

		// Attenuation
		float distance = length (lightData[i].s_lightPosition - fs_in.FragPos);
		float attenuation = 1.0f / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));

		vec3 colorTotal;
		colorTotal += lightData[i].s_lightColor;

		vec3 finalColor;
		finalColor = (ambient + (1.0 - shadow)) * (attenuation * diffuseColor * colorTotal);

		FragColor += vec4 (finalColor, 1.0);

		FragColor = vec4(diffuseColor,1.0);

//		FragColor = vec4(result * (1.0 - shadow), 1.0);
	}
}