
#version 330

#define LIGHT_SPOT_TYPE 0
#define LIGHT_POINT_TYPE 1

uniform sampler2D   tDiffuse;
uniform sampler2D   tPosition;
uniform sampler2D   tNormals;
uniform sampler2D   tShadowMap;
uniform sampler2D   tDepthMap;

uniform sampler2D   tPositionShadowMap;

uniform vec3        cameraPosition;

uniform mat4        cameraToLight;

uniform struct Light
{
    int      type;
    vec3     position;
    vec3     color;
    vec3     specular;
    vec3     diffuse;
    vec3     ambient;
    vec3     direction;
    float    spotCutOff;
    float    spotExponent;  // Larger is more decay
    float    constant;
    float    linear;
    float    quadratic;
} light[4];

vec3 ambient = vec3(0.01, 0.01, 0.01);

in vec2     fragTexCoord0;

out vec4    outColor;
vec4        ShadowCoord;

float LinearizeDepth(float zoverw)
{
    float n = 0.01;     // camera z near
    float f = 1000.0;   // camera z far
    return (2.0 * n) / (f + n - zoverw * (f - n));
}

//-----------------------------------------------------------------------------
//
// Return Boolean indicating if the current pixel is shaded or not
//
bool readShadowMap()
//-----------------------------------------------------------------------------
{
    vec4 pos = texture(tPosition,  fragTexCoord0.xy);

    ShadowCoord = cameraToLight * pos;

    ShadowCoord /= ShadowCoord.w;

    ShadowCoord.xy = ShadowCoord.xy * vec2 (0.5f, 0.5f) + vec2 (0.5f, 0.5f);

    if (ShadowCoord.w <= 0 || (ShadowCoord.x <=0 || ShadowCoord.y < 0) || (ShadowCoord.x >= 1 || ShadowCoord.y >= 1))
    {
//      discard;
//      outColor = vec4(0, 1, 0, 1);
        return false;
    }

    float a = texture(tShadowMap, ShadowCoord.xy).r;

// Debug to see the texture
//  a = LinearizeDepth(a); // * 5;
//  outColor = vec4(a, a, a, 1.0);

    float epsilon = 0.00;
    if (a + epsilon > texture(tDepthMap, fragTexCoord0).x)
    {

//      outColor = vec4(1, 0, 0, 1);
        return false;
    }
    else
    {
//      discard;
//      outColor = vec4(0, 0, 1, 1);
        return true;
    }

    return false; //a < texture(tPosition, fragTexCoord0).z;
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
//-----------------------------------------------------------------------------s
float getShadowFactor()
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



float materialShininess = 50.0;
vec3 materialSpecularColor = vec3(1, 1, 1);
float ambientCoefficient = .5;

//-----------------------------------------------------------------------------
//
// Calcuate the color the pixel based on parameters passed from
// 'index' into light structure
//
vec3 getLightColor(vec4 surfaceColor, vec3 normal, vec3 surfacePos, int index)
//-----------------------------------------------------------------------------
{
    float shininess = 50.0;
    float intensity = 5.0;

    vec3 lightDir = normalize(light[index].position - surfacePos);
    vec3 viewDir = normalize(cameraPosition - surfacePos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Diffuse shading
    float diffuse = max(dot(normal, lightDir), 0.0);

    if (LIGHT_POINT_TYPE == light[index].type)
    {
        // Specular shading
        float specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

        // Attenuation
        float distance = length(light[index].position - surfacePos);
        float attenuation = 1.0f / (light[index].constant + light[index].linear * distance + light[index].quadratic * (distance * distance));

        // Combine results
        vec3 ambientColor = light[index].ambient * vec3(surfaceColor.rgb);
        vec3 diffuseColor = light[index].diffuse * diffuse * vec3(surfaceColor) * intensity;
        vec3 specularColor = light[index].specular * specular * intensity;
        ambientColor *= attenuation;
        diffuseColor *= attenuation;
        specularColor *= attenuation;
        return (ambientColor + diffuseColor + specularColor);
    }

    if (LIGHT_SPOT_TYPE == light[index].type)
    {
       float dist;

        // Compute the distance to the light source
        dist = length(lightDir);

        // Compute the dot product between normal and ldir
        float NdotL = max(dot(normal,normalize(lightDir)),0.0);

        if (NdotL > 0.0)
        {
            float spotEffect = dot(normalize(light[index].direction), normalize(lightDir));

            if (spotEffect > light[index].spotCutOff)
            {

                if (true == readShadowMap())
                {
                    return vec3(0,0,0);
                }
                // Specular shading
                float specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

               // Attenuation
                float distance = length(light[index].position - surfacePos);
                float attenuation = 1.0f / (light[index].constant + light[index].linear * distance + light[index].quadratic * (distance * distance));

                // Combine results
                vec3 ambientColor = light[index].ambient * vec3(surfaceColor.rgb);
                vec3 diffuseColor = light[index].diffuse * diffuse * vec3(surfaceColor) * intensity;
                vec3 specularColor = light[index].specular * specular * intensity;
                ambientColor *= attenuation;
                diffuseColor *= attenuation;
                specularColor *= attenuation;
                return (ambientColor + diffuseColor + specularColor);
            }
        }
        return vec3(0.0,0.0,0.0);
    }
}


void main()
{
    vec3 normal =       texture(tNormals,   fragTexCoord0.xy).rgb;
    vec4 color   =      texture(tDiffuse,   fragTexCoord0.xy);
    vec3 position =     texture(tPosition,  fragTexCoord0.xy).rgb;

    outColor = vec4(0);

    outColor.rgb = color.rgb * ambient.rgb;

    for (int index = 0; index < 4; index++)
    {
        outColor.rgb += getLightColor(color, normal, position, index);
    }

//            outColor.rgb *= getLightColor(color, normal, position, 0);
    //
    // Gamma correction
    vec3 gamma = vec3(1.0/2.2);
    outColor.rgb = vec3(pow(outColor.rgb, gamma));

    return;

    //
    // Shadows
    bool shadow = readShadowMap();
    float shadowIntensity = 1.0;

    if (true == shadow)
    {
        shadowIntensity = 0.5; //getShadowFactor();
    }
    else
    {
        shadowIntensity = 1.0;
    }

    outColor.rgb *= shadowIntensity; //vec3(shadowIntensity, 0, 0);
}


