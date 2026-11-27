#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Norm;
in vec3 FragPos;
in mat3 NormMat;

uniform sampler2D SolidColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    FragColor = texture(SolidColor, TexCoord);
}
