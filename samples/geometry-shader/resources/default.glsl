#pragma begin:vertex

#version 330

layout in vec2 position;
layout in vec2 uv;

uniform mat4 mvp;
out vec2 gUv;

void main() {
    gl_Position = mvp * vec4(position, 0.0, 1.0);
    gUv = uv;
}

#pragma end:vertex

#pragma begin:geom

#version 330

layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in vec2 gUv[];
out vec2 fUv;

void main() {
    for(int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        fUv = gUv[i];
        EmitVertex();
    }

    EndPrimitive();
}

#pragma end:geom

#pragma begin:fragment

#version 330

out vec4 outColour;
in vec2 fUv;

void main(void) {
    outColour = vec4(fUv, 0.0, 1.0);
}

#pragma end:fragment
