#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 texCoord;

layout(push_constant) uniform PushConstant {
	mat4 transform;
} pushConstant;

// layout(binding = 256) uniform UniformBufferObject {
//     mat4 mvp;
// } ubo;


void main() {
    gl_Position = pushConstant.transform * vec4(inPosition, 1.0);
    fragColor = vec3(inUV, 0.0);
    texCoord = inUV;
}