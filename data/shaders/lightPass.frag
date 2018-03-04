#version 330

//#define MAX_LIGHTS 10

uniform sampler2D   tPosition;
uniform sampler2D   tNormals;
uniform sampler2D   tDiffuse;

/*
//uniform sampler2D   tShadowMap;
//uniform sampler2D   tDepthMap;

//uniform vec3        cameraPosition;

//uniform mat4        cameraToLight;

//uniform float       materialShininess;
//uniform vec3        materialSpecularColor;

//uniform int numLights;

uniform struct Light
{
    vec4     position;
    vec3     intensities; //a.k.a the color of the light
    float    attenuation;
    float    ambientCoefficient;
    float    coneAngle;
    vec3     coneDirection;
} allLights[MAX_LIGHTS];

 * */
in vec2     fragTexCoord0;

out vec4    outColor;
//vec4        ShadowCoord;
/*
//-----------------------------------------------------------------------------
//
// Debug depth texture
float LinearizeDepth(float zoverw)
//-----------------------------------------------------------------------------
{
    float n = 0.01;     // camera z near
    float f = 1000.0;   // camera z far
    return (2.0 * n) / (f + n - zoverw * (f - n));
}

//-----------------------------------------------------------------------------
//
// Return Boolean indicating if the current pixel is shaded or not
bool readShadowMap()
//-----------------------------------------------------------------------------
{
    vec4 pos = texture(tPosition,  fragTexCoord0.xy);

    ShadowCoord = cameraToLight * pos;

    ShadowCoord /= ShadowCoord.w;

    ShadowCoord.xy = ShadowCoord.xy * vec2 (0.5f, 0.5f) + vec2 (0.5f, 0.5f);

    if (ShadowCoord.w <= 0 || (ShadowCoord.x <=0 || ShadowCoord.y < 0) || (ShadowCoord.x >= 1 || ShadowCoord.y >= 1))
    {
        return false;
    }

    float a = texture(tShadowMap, ShadowCoord.xy).r;

// Debug to see the texture
//  a = LinearizeDepth(a); // * 5;
//  outColor = vec4(a, a, a, 1.0);

    float epsilon = 0.00;
    if (a + epsilon > texture(tDepthMap, fragTexCoord0).x)
    {
        return false;
    }
    else
    {
        return true;
    }

//    return false; //a < texture(tPosition, fragTexCoord0).z;
}






// number of samples
const int numSamplingPositions = 4;

// offsets for rectangular PCF sampling
vec2 kernel[4] = vec2[]
                 (
                     vec2(1.0, 1.0), vec2(-1.0, 1.0), vec2(-1.0, -1.0), vec2(1.0, -1.0)
                 );

// performs sampling and accumulates shadowing factor
float sample(in vec2 coords, in vec2 offset,
             inout float factor, inout float numSamplesUsed)
{
    factor += texture(tShadowMap, coords.xy + offset).r;
    max(factor, 1);
    numSamplesUsed += 1;

    return factor;
}

//-----------------------------------------------------------------------------
//
// Soften edges of shadow
float getShadowFactor()
//-----------------------------------------------------------------------------s
{
    // sample each PCF texel around current fragment
    float shadowFactor = 0.0;
    float numSamplesUsed = 0.0;
    float PCFRadius = 4;
    float shadowMapStep = 1;

    float addShadow = 0;

    for(int i = 1; i < numSamplingPositions; i++)
    {
        addShadow += sample(ShadowCoord.xy,
                            kernel[i] * shadowMapStep * PCFRadius,
                            shadowFactor,
                            numSamplesUsed
                           );
    }

    // normalize shadowing factor
    return addShadow / 16; //numSamplesUsed;
}


//-----------------------------------------------------------------------------
//
// Calcuate the color the pixel based on parameters in light structure
//
vec3 ApplyLight(int index, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera)
//-----------------------------------------------------------------------------
{
    vec3 surfaceToLight;
    float attenuation = 1.0;
    float shadowIntensity = 1.0;

    if (allLights[index].position.w == 0.0)
    {
        //directional light
        surfaceToLight = normalize(allLights[index].position.xyz);
        attenuation = 1.0; //no attenuation for directional lights
    }
    else
    {
        //point light
        surfaceToLight = normalize(allLights[index].position.xyz - surfacePos);
        float distanceToLight = length(allLights[index].position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + allLights[index].attenuation * pow(distanceToLight, 2));

        //cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(allLights[index].coneDirection))));
        if(lightToSurfaceAngle > allLights[index].coneAngle)
        {
            attenuation = 0.0;
            //
            // Shadows
            bool shadow = readShadowMap();

            if (true == shadow)
            {
                shadowIntensity = 0.5; //getShadowFactor();
            }
            else
            {
                shadowIntensity = 1.0;
            }

//            return vec3(0,0,0);
        }
    }

    //ambient
    vec3 ambient = allLights[index].ambientCoefficient * surfaceColor.rgb * allLights[index].intensities * shadowIntensity;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * allLights[index].intensities;

    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);

    vec3 specular = specularCoefficient * materialSpecularColor * allLights[index].intensities;

    //linear color (color before gamma correction)
    return ambient + attenuation*(diffuse + specular);
}
*/

void main()
{
    vec3 normal =       texture(tNormals,   fragTexCoord0.xy).rgb;
    vec4 color   =      texture(tDiffuse,   fragTexCoord0.xy);
    vec3 position =     texture(tPosition,  fragTexCoord0.xy).rgb;

    outColor.rgb = texture2D(tDiffuse, fragTexCoord0).rgb;

    vec3 gamma = vec3(1.0/0.8);
	
    outColor.rgb = vec3(pow(outColor.rgb, gamma)) * 4;
}


