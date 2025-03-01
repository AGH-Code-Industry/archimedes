#version 450 core

layout(binding = 0) uniform texture2D tex;
layout(binding = 128) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

float screenPxRange() {
    return 4.0;
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec4 msdf = texture(sampler2D(tex, texSampler), texCoord);
    float distance = median(msdf.r, msdf.g, msdf.b);

    float pxDist = screenPxRange() * (distance - 0.5);
    float alpha = clamp(pxDist + 0.5, 0.0, 1.0);

    outColor = vec4(msdf.rgb, alpha);
}