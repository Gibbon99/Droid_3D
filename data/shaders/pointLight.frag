#version 330

uniform mat4 u_modelMat;
uniform vec3 cameraPosition;

uniform sampler2D   inTexture0;     // material texture
uniform sampler2D   inTexture1;     // shadowmap texture

uniform float materialShininess;
uniform vec3 materialSpecularColor;

uniform struct Light
{
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
} light;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

// texture coordinates for access to the shadow map
in vec4     o_shadowCoord;

out vec4 finalColor;

void main()
{
    float shadowIntensity = 0.5;

    vec3 normal = normalize(transpose(inverse(mat3(u_modelMat))) * fragNormal);
    vec3 surfacePos = vec3(u_modelMat * vec4(fragVert, 1));
    vec4 surfaceColor = texture(inTexture0, fragTexCoord);
    vec3 surfaceToLight = normalize(light.position - surfacePos);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    //ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;

    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

    //attenuation
    float distanceToLight = length(light.position - surfacePos);
    float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + attenuation*(diffuse + specular);

    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);

    vec4 scPostW = o_shadowCoord / o_shadowCoord.w;

    if (o_shadowCoord.w <= 0.0f || (scPostW.x < 0 || scPostW.y < 0) || (scPostW.x >= 1 || scPostW.y >= 1))
        {
            // Behind or outside frustrum: no shadow
            shadowIntensity = 1.0;
        }
    else
        {
            // Standard shadow mapping, done manually
            float shadow = texture2D(inTexture1, scPostW.xy).x;
            float epsilon = 0.0;
            if (shadow + epsilon < scPostW.z)
                {
                    shadowIntensity = 0.5;
                }
            else
                {
                    shadowIntensity = 1.0;
                }
        }
    finalColor = vec4(pow(linearColor, gamma), surfaceColor.a) * shadowIntensity;
}
