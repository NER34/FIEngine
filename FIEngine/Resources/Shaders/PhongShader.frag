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
    vec3 objectColor = texture(SolidColor, TexCoord).xyz;
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //vec3 norm = normalize(Norm);
    vec3 norm = normalize(NormMat * Norm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = objectColor * (ambient + diffuse + spec);
    FragColor = vec4(result, 1.0);
}
