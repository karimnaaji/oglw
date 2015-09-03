#pragma begin:vertex
#version 330

layout (location = 1) in vec2 position;
layout (location = 0) in vec2 uv;

uniform mat4 mvp;
uniform sampler2D tex;

out vec2 uvs;

void main() {
    float offset = texture(tex, uv).r;
    gl_Position = mvp * vec4(position, offset, 1.0);
    uvs = uv;
}

#pragma end:vertex

#pragma begin:fragment
#version 330

out vec4 outColour;

in vec2 uvs;

void main(void) {
    outColour = vec4(uvs, 0.0, 1.0);
}
#pragma end:fragment
