#version 330 core

in vec3 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform float green;

uniform sampler2D wood;
uniform sampler2D face;

uniform float percentage;

void main() {
    FragColor = mix(texture(wood, TexCoord), texture(face, TexCoord), percentage) * vec4(Color, 1.0);
}