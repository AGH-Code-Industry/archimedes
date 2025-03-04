#version 450

layout(binding = 0) uniform texture2D tex;
layout(binding = 128) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

vec4 unpackColor(float color) {
    uint packed = floatBitsToUint(color);
    
    float a = float((packed >> 0)  & 0xff) / 255.f;
    float b = float((packed >> 8)  & 0xff) / 255.f;
    float r = float((packed >> 16) & 0xff) / 255.f;
    float g = float((packed >> 24) & 0xff) / 255.f;
    
    return vec4(r, g, b, a);
}

vec2 unpackRG(float packed) {
    float g = floor(packed / 256.0) / 255.0;
    float r = mod(packed, 256.0) / 255.0;
    return vec2(r, g);
}

vec2 unpackBA(float packed) {
    float a = floor(packed / 256.0) / 255.0;
    float b = mod(packed, 256.0) / 255.0;
    return vec2(b, a);
}


void main() {
    vec2 rg = unpackRG(texCoord.x);
    vec2 ba = unpackBA(texCoord.y);
    outColor = vec4(rg.x, rg.y, ba.x, ba.y);
}
