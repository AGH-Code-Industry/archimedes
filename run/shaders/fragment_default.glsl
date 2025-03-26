#version 450

layout(binding = 0) uniform texture2D tex;
layout(binding = 128) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 color = texture(sampler2D(tex, texSampler), texCoord);// + vec4(fragColor, 1.0);
    outColor = vec4(pow(color.rgb, vec3(2.2)), color.a);
}