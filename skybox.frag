#version 410 core

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main(void)
{
    FragColor = texture(skybox, texCoords);
}
