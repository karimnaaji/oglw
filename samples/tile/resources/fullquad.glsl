#pragma begin:vertex
#version 330

layout (location = 0) in vec2 position;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
}

#pragma end:vertex

#pragma begin:fragment
#version 330

uniform sampler2D tex;
uniform vec2 resolution;

out vec4 outColour;

void main(void) {
    vec2 uv = gl_FragCoord.xy / resolution;

    outColour = vec4(vec3(texture(tex, uv).x), 1.0);
}

#pragma end:fragment
