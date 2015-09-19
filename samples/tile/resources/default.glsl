#pragma begin:vertex
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 depthMVP;
uniform mat3 normalMat;

out vec4 shadowCoord;
out vec3 n;
out vec4 pos;

void main() {
    pos = mvp * vec4(position, 1.0);
    gl_Position = pos;
    shadowCoord = depthMVP * vec4(position, 1);
    n = normalMat * normal;
}

#pragma end:vertex

#pragma begin:fragment
#version 330

uniform sampler2D depthMap;
uniform vec3 lightPos;

out vec4 outColour;
in vec4 shadowCoord;
in vec3 n;
in vec4 pos;

float hardShadow(vec4 shadowCoord, float bias) {
    vec3 shadowPosition = shadowCoord.xyz / shadowCoord.w;
    vec2 uvs = shadowPosition.xy * 0.5 + vec2(0.5);
    float z = 0.5 * shadowPosition.z + 0.5;
    float depth = texture(depthMap, uvs).x;

    if (shadowCoord.w <= 0.0 || uvs.x < 0.0 || uvs.y < 0.0 || uvs.x >= 1.0 || uvs.y >= 1.0) {
        // not in frustrum
        return 1.0;
    } else {
        return step(z - bias, depth);
    }
}

float PCF(vec2 texelSize, vec4 shadowCoord, float bias) {
    float result = 0.0;
    vec2 offset = texelSize * shadowCoord.w * 3.0;

    result += hardShadow(shadowCoord + vec4(vec2(-1.5, -1.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(-1.5, -0.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(-1.5,  0.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(-1.5,  1.5) * offset, 0.0, 0.0), bias);

    result += hardShadow(shadowCoord + vec4(vec2(-0.5, -1.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(-0.5, -0.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(-0.5,  0.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(-0.5,  1.5) * offset, 0.0, 0.0), bias);

    result += hardShadow(shadowCoord + vec4(vec2(0.5, -1.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(0.5, -0.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(0.5,  0.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(0.5,  1.5) * offset, 0.0, 0.0), bias);

    result += hardShadow(shadowCoord + vec4(vec2(1.5, -1.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(1.5, -0.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(1.5,  0.5) * offset, 0.0, 0.0), bias);
    result += hardShadow(shadowCoord + vec4(vec2(1.5,  1.5) * offset, 0.0, 0.0), bias);

    return result / 16.0;
}

void main(void) {
    vec3 surfaceToLight = normalize(lightPos - pos.xyz);

    float D = max(dot(n, surfaceToLight), 0.0);

    // FIXME : add normals to plane
    if (n == vec3(0.0)) {
        D = 1.0;
    }

    // TODO : adaptive bias
    float shadow = PCF(vec2(1.0 / 2048.0), shadowCoord, 0.0025);
    outColour = vec4(vec3(D) * shadow, 1.0);
}

#pragma end:fragment
