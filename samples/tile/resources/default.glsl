#pragma begin:vertex
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;

out vec3 fNormal;

void main() {
    fNormal = normal;
    gl_Position = mvp * vec4(position, 1.0);
}

#pragma end:vertex

#pragma begin:fragment
#version 330

uniform mat3 normalmat;

out vec4 outColour;

in vec3 fNormal;

void main(void) {
    outColour = vec4(normalmat * fNormal, 1.0);
}

#pragma end:fragment
