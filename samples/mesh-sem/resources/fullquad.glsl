#pragma begin:vertex
#version 330

in vec2 position;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
}

#pragma end:vertex

#pragma begin:fragment
#version 330

uniform sampler2D tex;
uniform vec2 resolution;
uniform float far;
uniform float near;

out vec4 outColour;

void main(void) {
    vec2 uv = gl_FragCoord.xy / resolution;
    outColour = vec4(texture(tex, uv).rgb, 1.0);

    //float depth = texture(tex, uv).x;
    //float linearDepth = 2.0 * near * far / (far + near - 2.0 * depth - 1.0 * (far - near));
    //outColour = vec4(vec3(linearDepth), 1.0);
}

#pragma end:fragment
