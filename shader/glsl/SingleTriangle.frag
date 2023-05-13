#version 330 core

in vec3 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform float green;

uniform sampler2D wood;
uniform sampler2D face;

void main() {
    FragColor = mix(texture(wood, TexCoord), texture(face, TexCoord), 0.2) * vec4(Color, 1.0);
}