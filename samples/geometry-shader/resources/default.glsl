#pragma begin:vertex
#version 330

layout (location = 1) in vec2 position;
layout (location = 0) in vec2 uv;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(position, 0.0, 1.0);
}

#pragma end:vertex

#pragma begin:fragment
#version 330

out vec4 outColour;

void main(void) {
    outColour = vec4(1.0);
}
#pragma end:fragment
