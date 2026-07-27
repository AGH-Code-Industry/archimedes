#version 450

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 outColor;

void main() {
    vec2 center = vec2(0.5, 0.5);
    vec2 pos = texCoord - center;

    float dist = length(pos);

    float radius = 0.5;

    if (dist <= radius) {
        outColor = vec4(0.0, 0.0, 1.0, 1.0);
    } else {
        outColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}