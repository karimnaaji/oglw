#version 330

uniform sampler2D tex;

out vec4 outColour;

in vec2 uvs;

void main(void) {
    outColour = texture(tex, uvs);
}
