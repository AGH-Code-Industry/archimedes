#version 450

layout(binding = 0) uniform texture2D tex;
layout(binding = 128) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main() {
    // funny circle
    //float d = min(sqrt((texCoord.x - 0.5) * (texCoord.x - 0.5) + (texCoord.y - 0.5) * (texCoord.y - 0.5)), 0.5f) * 2;

    outColor = vec4(texture(sampler2D(tex, texSampler), texCoord).rgb, sin(texCoord.y * 3.14159265f));
}