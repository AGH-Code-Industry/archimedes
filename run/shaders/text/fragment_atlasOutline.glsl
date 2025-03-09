#version 450 core

layout(binding = 0) uniform texture2D tex;
layout(binding = 128) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

// Variable for outline thickness (in SDF units, e.g. 0.05)
float outlineThickness = 0.3;

float screenPxRange() {
    return 32.0;
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec3 msdf = texture(sampler2D(tex, texSampler), texCoord).rgb;
    float distance = median(msdf.r, msdf.g, msdf.b);
    float smoothing = 1.0 / screenPxRange();

    float insideAlpha = smoothstep(
        0.5 - smoothing,
        0.5 + smoothing,
        distance
    );
    float outlineAlpha = smoothstep(
        0.5 - outlineThickness - smoothing,
        0.5 - outlineThickness + smoothing,
        distance
    );
    
    float alpha = max(insideAlpha, outlineAlpha);
    vec3 color = mix(vec3(1.0), vec3(0.0), insideAlpha);
    
    outColor = vec4(vec3(1) - color, alpha);
}