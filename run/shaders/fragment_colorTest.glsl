#version 450

layout(binding = 0) uniform texture2D tex;
layout(binding = 128) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 customColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = /*texture(sampler2D(tex, texSampler), texCoord) **/ customColor;
}