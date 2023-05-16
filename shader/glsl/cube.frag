#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D wood;
uniform sampler2D face;

void main() 
{
    color = mix(texture(wood, TexCoord), texture(face, TexCoord), 0.2);
}