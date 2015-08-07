#version 150

in vec2 position;
in vec2 uv;

uniform mat4 mvp;
uniform sampler2D tex;

out vec2 uvs;

float luminance(vec3 color) {
    return dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
}

void main() {

    float d = luminance(texture(tex, uv).rgb);

    gl_Position = mvp * vec4(position, d * 0.3, 1.0);
    uvs = uv;
}
