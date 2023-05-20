#version 330 core

in vec3 fragColor;
in vec2 TexCoord;
out vec4 color;

uniform sampler2D wood;
uniform sampler2D face;

void main() 
{
    color = vec4(fragColor, 1.0f) * mix(texture(wood, TexCoord), texture(face, TexCoord), 0.2);
}