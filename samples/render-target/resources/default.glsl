#pragma begin:vertex
#version 330

layout (location = 0) in vec3 position;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
}

#pragma end:vertex

#pragma begin:geom
#version 330

layout (triangles) in;
layout (line_strip, max_vertices = 5) out;

out vec3 normal;
uniform mat4 mvp;

void main() {
    vec3 normals[3];
    gl_Position = gl_in[0].gl_Position;
    normal = normalize(cross(
        gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz,
        gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz));
    normals[0] = normal;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    normal = normalize(cross(
        gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz,
        gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz));
    normals[1] = normal;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    normal = normalize(cross(
        gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz,
        gl_in[0].gl_Position.xyz - gl_in[2].gl_Position.xyz));
    normals[2] = normal;    
    EmitVertex();

    EndPrimitive();

    vec4 m = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
    gl_Position = m;
    EmitVertex();
    gl_Position = m - vec4(normals[0], 0.0) * 2.0;
    EmitVertex();
    EndPrimitive();
}

#pragma end:geom

#pragma begin:fragment
#version 330

out vec4 outColour;
in vec3 normal;

void main(void) {
    outColour = vec4(normal, 1.0);
}

#pragma end:fragment
