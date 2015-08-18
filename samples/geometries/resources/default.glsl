#pragma begin:vertex
#version 330

layout (location = 1) in vec2 position;
layout (location = 0) in vec2 uv;

uniform mat4 mvp;
uniform sampler2D tex;

out vec2 uvs;

float luminance(vec3 color) {
    return dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
}

void main() {
    gl_Position = mvp * vec4(position, luminance(texture(tex, uv).rgb) * 0.3, 1.0);
    uvs = uv;
}

#pragma end:vertex

#pragma begin:fragment
#version 330

uniform sampler2D tex;

out vec4 outColour;

in vec2 uvs;

void main(void) {
    outColour = texture(tex, uvs);
}
#pragma end:fragment
