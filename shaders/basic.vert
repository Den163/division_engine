#version 450

layout (location=0) in vec3 VertexPosition;
layout (location=1) in mat4 ModelViewProjectionMatrix;

void main()
{
    gl_Position = ModelViewProjectionMatrix * vec4(VertexPosition, 1.0);
}