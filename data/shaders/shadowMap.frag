#version 330

// the shadow map from first rendering pass
uniform sampler2D   inTexture0;     // diffuse from texture

uniform sampler2D   inTexture1;     //  shadowmap texture
in      vec2        texCoord0;

// texture coordinates for access to the shadow map
in vec4     o_shadowCoord;
// color to the framebuffer
out vec4    resultingColor;


//uniform mat4 mv_matrix;
in vec3 frag_position, frag_normal;
in vec2 frag_texcoord;
in float frag_shininess;
in vec4 frag_specular;

in vec3 lightPosition0;

in mat4 mv_matrix0;

const vec3 light_direction = vec3(0.01, -60.0, 0.01);
const vec4 light_diffuse = vec4(0.1, 0.1, 0.1, 0.0);
const vec4 light_ambient = vec4(0.2, 0.2, 0.3, 1.0);
const vec4 light_specular = vec4(0.0, 1.0, 1.0, 1.0);



void main(void)
{
    vec4 scPostW = o_shadowCoord / o_shadowCoord.w;

    float ambientLight = 1.0;
    float shadowIntensity = 0.5;

    vec4 outColor = texture2D(inTexture0, texCoord0);    // Get the color of the pixel from the texture

    if (o_shadowCoord.w <= 0.0f || (scPostW.x < 0 || scPostW.y < 0) || (scPostW.x >= 1 || scPostW.y >= 1))
        {
            // Behind or outside frustrum: no shadow
            resultingColor.rgb = outColor.rgb;
            resultingColor.a = 0.5;
        }
    else
        {
            // Standard shadow mapping, done manually
            float shadow = texture2D(inTexture1, scPostW.xy).x;
            float epsilon = 0.0000;
            if (shadow + epsilon < scPostW.z)
                {
                    resultingColor.rgb = outColor.rgb * shadowIntensity;
                    resultingColor.a = 0.5;
                }
            else
                {
                    resultingColor.rgb = outColor.rgb;
                    resultingColor.a = 0.5;
                }
        }

    resultingColor.rgb *= ambientLight;



    vec3 mv_light_direction = (mv_matrix0 * vec4(normalize(lightPosition0), 0.0)).xyz;
//    vec3 mv_light_direction = (mv_matrix0 * vec4(light_direction, 0.0)).xyz;
    vec3 normal = normalize(frag_normal);
    vec3 eye = normalize(frag_position);
    vec3 reflection = reflect(mv_light_direction, normal);

    vec4 frag_diffuse = resultingColor; //texture2D(inTexture0, texCoord0);
    vec4 diffuse_factor = max(-dot(normal, mv_light_direction), 0.0) * light_diffuse;
    vec4 ambient_diffuse_factor = diffuse_factor + light_ambient;
    vec4 specular_factor = max(pow(-dot(reflection, eye), frag_shininess), 0.0) * light_specular;

//    resultingColor.rgb = vec3(specular_factor * frag_specular + ambient_diffuse_factor * frag_diffuse * ambientLight);
//    resultingColor.rgb = vec3(specular_factor * frag_specular + ambient_diffuse_factor * frag_diffuse);

    resultingColor = specular_factor * frag_specular + ambient_diffuse_factor * frag_diffuse;

}
