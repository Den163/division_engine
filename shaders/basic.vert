#version 450

layout (location=0) in vec4 in_Color;
layout (location=0) out vec4 out_Color;

layout (location=1) in vec3 VertexPosition;
layout (location=2) uniform mat4 ModelViewProjectionMatrix;


void main()
{
    gl_Position = ModelViewProjectionMatrix * vec4(VertexPosition, 1.0);
    out_Color = in_Color;
}