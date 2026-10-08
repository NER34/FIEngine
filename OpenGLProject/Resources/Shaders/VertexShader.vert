#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projViewMat;
uniform mat4 modelMat;

out vec2 TexCoord;
out vec3 Norm;
out vec3 FragPos;
out mat3 NormMat;

void main()
{
    gl_Position = projViewMat * modelMat * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
    Norm = aNorm;
    FragPos = (modelMat * vec4(aPos, 1.0f)).xyz;
    NormMat = mat3(transpose(inverse(modelMat)));
}