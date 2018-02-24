#version 330

in vec3     WorldPos0;

out vec4    fragColor;
out vec3    shadowMapPosition;

void main(void)
{
    //
    // Write position to texture
    shadowMapPosition = WorldPos0.xyz;

//    fragColor = vec4(1.0,0,0,0);
}
