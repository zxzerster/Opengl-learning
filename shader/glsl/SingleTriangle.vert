#version 330 core

layout (location = 0) in vec3 pos1;
layout (location = 1) in vec3 color2;
layout (location = 2) in vec2 texCoord3;

out vec3 Color;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(pos1.x, pos1.y, pos1.z, 1.0);
    TexCoord = vec2(texCoord3.x, 1.0 - texCoord3.y);
    Color = color2;
}