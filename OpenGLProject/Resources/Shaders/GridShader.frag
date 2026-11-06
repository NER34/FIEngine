#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Norm;
in vec3 FragPos;
in mat3 NormMat;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    vec2 TexCoordScaled = TexCoord * 10.0;
    int x = int(TexCoordScaled.x);
    int y = int(TexCoordScaled.y);
    if (mod(floor(x) + floor(y), 2.0) == 0)
    {
        FragColor = vec4(0.45, 0.45, 0.45, 1.0);
    }
    else
    {
        FragColor = vec4(0.6, 0.6, 0.6, 1.0);      
    }
}
