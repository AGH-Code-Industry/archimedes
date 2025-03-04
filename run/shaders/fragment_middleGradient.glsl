#version 450

layout(binding = 0) uniform texture2D tex;
layout(binding = 128) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

float alpha(vec2 v) {
    float edgeMin = 1.0 / 4.0;
    float edgeMax = 1.0 - edgeMin;
    
    if (v.x < edgeMin || v.x > edgeMax /* || v.y < edgeMin || v.y > edgeMax */) {
        return 0.0; // Completely black outside the fade region
    }
    
    // Compute linear fade effect symmetrically inside the defined region
    float fadeX = 1.0 - abs((v.x - edgeMin) / (edgeMax - edgeMin) - 0.5) * 2.0;
    //float fadeY = 1.0 - abs((v.y - edgeMin) / (edgeMax - edgeMin) - 0.5) * 2.0;
    
    return fadeX;// * fadeY;
}

void main() {
    outColor = vec4(0.0, 0.0, 0.0, 1.0 - alpha(texCoord));
}