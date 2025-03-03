#version 450 core

layout(binding = 0) uniform texture2D tex;
layout(binding = 128) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

float screenPxRange() {
    return 12.0;
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec3 msdf = texture(sampler2D(tex, texSampler), texCoord).rgb;
    float distance = median(msdf.r, msdf.g, msdf.b);

    float pxDist = screenPxRange() * (distance - 0.5f);
    float alpha = clamp(pxDist + 0.5f, 0.0f, 1.0f);

    outColor = vec4(vec3(1), alpha);
}