#version 410 core

layout(location = 0) in vec3 positionIn;

out vec3 texCoords;

uniform mat4 pMatrix;
uniform mat4 vMatrix;

void main(void)
{   
    vec4 pos = pMatrix * vMatrix * vec4(positionIn, 1.0f);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    texCoords = vec3(positionIn.x, positionIn.y, -positionIn.z);
}
