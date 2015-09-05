#pragma begin:vertex
#version 330

layout (location = 1) in vec2 position;
layout (location = 0) in vec2 uv;

uniform mat4 mvp;
uniform sampler2D tex;
uniform float t0;
uniform float time;

out vec2 fUV;
out float fOffset;

void main() {
    float offset = texture(tex, uv * 5.0).r;
    float r = time * t0;
    gl_Position = mvp * vec4(position, 2.f * offset, 1.0);
    fOffset = offset;
}

#pragma end:vertex

#pragma begin:fragment
#version 330

out vec4 outColour;

in float fOffset;

void main(void) {
    outColour = vec4(vec3(1.0) * fOffset, 1.0);
}

#pragma end:fragment
