#version 450

layout (location=0) out vec4 FragColor;
layout (location=1) in vec4 Color;

void main() {
    FragColor = Color;
}