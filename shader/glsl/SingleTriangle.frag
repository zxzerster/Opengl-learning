#version 330 core

out vec4 FragColor;

uniform float green;

void main() {
    FragColor = vec4(0.0f, green, 0.0f, 1.0f);
}