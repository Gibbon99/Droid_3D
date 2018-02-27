#version 330

#define MAX_LIGHTS 40

#define LIGHT_SPOTLIGHT    0
#define LIGHT_POINT        1

uniform mat4 u_modelMat;
uniform mat4 projMatrix;
uniform mat4 u_viewProjectionMat;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec2 fragLightmapCoord;
in vec3 fragVert;

out vec4 finalColor;

uniform sampler2D inTexture0;
uniform sampler2D inTexture1;

uniform vec3        cameraPosition;

uniform float       globalAmbient;
uniform float       globalGammaFactor;

uniform float       materialShininess;
uniform vec3        materialSpecularColor;

uniform int         numLights;

uniform struct Light
{
    int      type;
    vec4     position;
    vec3     intensities; //a.k.a the color of the light
    float    attenuation;
    float    ambientCoefficient;
    float    coneAngle;
    vec3     coneDirection;
} allLights[MAX_LIGHTS];

//
// See if the pixel has a normal, and if it pointing away from the light, then don't draw it
// Stop the light bleeding through walls


//-----------------------------------------------------------------------------
//
// Calcuate the color the pixel based on parameters in light structure
//
vec3 ApplyLight(int index, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera)
//-----------------------------------------------------------------------------
{
    vec3        surfaceToLight;
    vec4        lightPosition;
    vec3        materialSpecularColor;

    materialSpecularColor = vec3(1,1,1);

    lightPosition = allLights[index].position;
    vec3 transformed_normal = normal;

    //point light
    surfaceToLight = normalize(lightPosition.xyz - surfacePos);
    float distanceToLight = distance(vec3(allLights[index].position).xyz, surfacePos.xyz);

    float lightAdd = 0.0f;
    float b = 0.01;

    float att = 1.0 / (1.0 + allLights[index].attenuation * distanceToLight + b * distanceToLight*distanceToLight);

    vec3 specular = vec3(0);

    if (materialShininess > 0.0)
    {
        //specular
        float specularCoefficient = 0.0;
    //    if (diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);

        specular = specularCoefficient * materialSpecularColor * allLights[index].intensities;
    }

    if (LIGHT_SPOTLIGHT == allLights[index].type)
    {
        //cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(allLights[index].coneDirection))));
        if(lightToSurfaceAngle > allLights[index].coneAngle)
        {
            //discard;
            att = 0.0;
            return vec3(0);
        }
    }

    vec3 surf2light = normalize(lightPosition.xyz - surfacePos);

    vec3 norm = normalize(normal);

    float dcont = max(0.0, dot(norm, surf2light));

    lightAdd += att * (dcont + allLights[index].ambientCoefficient);

    vec3 lightOutput = lightAdd * allLights[index].intensities;

    return ((globalAmbient + lightOutput) * surfaceColor) + specular;


/*

    float b;
float radius = 9;
float minLight = 0.01;

    b = 1.0 / (radius*radius * minLight);

    attenuation = 1.0 / (1.0 + (allLights[index].attenuation * distanceToLight) + (b * distanceToLight * distanceToLight));// pow(distanceToLight, 2));
//    1.0 / (1.0 + a*dist + b*dist*dist))



    //ambient
    vec3 ambient = allLights[index].ambientCoefficient * surfaceColor.rgb * allLights[index].intensities * attenuation;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * allLights[index].intensities * attenuation;

    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);

    vec3 specular = specularCoefficient * materialSpecularColor * allLights[index].intensities;

    //linear color (color before gamma correction)
    return (ambient * diffuse) * attenuation;

//    		results.specular += surface.specular * light.specular *pow(specular, surface.shininess) * attenuation;

*/
}

void main()
{
    vec3 normal =       normalize(fragNormal);
    vec3 color   =      texture(inTexture0, fragTexCoord).rgb;
    vec3 position =     fragVert;
    vec3 surfaceToCamera = normalize(cameraPosition - position);

    finalColor.rgb = vec3(0);

    for (int index = 0; index < numLights; index++)
    {
        finalColor.rgb += ApplyLight(index, color, normal, position, surfaceToCamera);
    }

    //
    // Lightmap textures
    finalColor.rgb += texture(inTexture1, fragLightmapCoord).rgb;
    //
    // Gamma correction
    vec3 gamma = vec3(1.0/2.2);
    finalColor.rgb = vec3(pow(finalColor.rgb, gamma)) * globalGammaFactor;
}

 // w-component: 1 for positions and 0 for vectors
