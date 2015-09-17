#pragma begin:vertex
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 depthMVP;

out vec4 shadowCoord;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    shadowCoord = depthMVP * vec4(position, 1);
}

#pragma end:vertex

#pragma begin:fragment
#version 330

uniform mat3 normalmat;
uniform sampler2D depthMap;

out vec4 outColour;
in vec4 shadowCoord;

float visibility(vec4 lightSpace) {
    vec3 shadowPosition = lightSpace.xyz / lightSpace.w;
    vec2 uvs;
    uvs.x = 0.5 * shadowPosition.x + 0.5;
    uvs.y = 0.5 * shadowPosition.y + 0.5;
    float z = 0.5 * shadowPosition.z + 0.5;
    float depth = texture(depthMap, uvs).x;

    if (depth < (z - 0.05)) {
        return 0.5;
    } else {
        return 1.0;
    }
}

void main(void) {
    outColour = vec4(vec3(visibility(shadowCoord)), 1.0);
    //outColour = vec4(shadowCoord.xy, 0.0, 1.0);
    //outColour = vec4(vec3(shadowCoord.z), 1.0);
}

#pragma end:fragment
