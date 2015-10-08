#pragma begin:vertex
#version 330

in vec3 position;

uniform mat4 depthMVP;

void main() {
    gl_Position = depthMVP * vec4(position, 1.0);
}

#pragma end:vertex

#pragma begin:fragment
#version 330

out float depth;

void main(void) {
    depth = gl_FragCoord.z;
}

#pragma end:fragment
