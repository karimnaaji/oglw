#pragma begin:vertex
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;

out vec3 vNormal;

void main() {
    vNormal = normal;
    gl_Position = mvp * vec4(position, 1.0);
}

#pragma end:vertex
#pragma begin:geom
#version 330

layout(line_strip) in;
layout(line_strip, max_vertices = 2) out;

in vec3 vNormal[];

out vec3 fNormal;

void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
    fNormal = vNormal[0];
}

#pragma end:geom
#pragma begin:fragment
#version 330

uniform mat3 normalmat;

out vec4 outColour;

in vec3 fNormal;

void main(void) {
    outColour = vec4(normalmat * fNormal, 1.0);
}

#pragma end:fragment
