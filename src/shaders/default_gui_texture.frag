#version 450

layout(location=0) out vec4 out_Color;
layout(location=2) in vec2 uv;

layout(binding=0) uniform sampler2D tex;

void main() {
    out_Color = texture(tex, uv);
}
