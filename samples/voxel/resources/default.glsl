#pragma begin:vertex

#version 330

in vec3 position;
in vec3 color;
out vec3 gColor;

void main() {
    gl_Position = vec4(position, 1.0);
    gColor = color;
}

#pragma end:vertex

#pragma begin:geom

#version 330

uniform mat4 mvp;

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in vec3 gColor[];
out vec3 fColor;
out vec3 fNormal;

void main() {
    mat4 transform = mvp;
    const vec3 vx = vec3(0.5, 0, 0);
    const vec3 vy = vec3(0, 0.5, 0);
    const vec3 vz = vec3(0, 0, 0.5);

    fColor = gColor[0];
    vec3 p = gl_in[0].gl_Position.xyz;

    fNormal = vec3(0.0, 0.0, -1.0);
    gl_Position = transform * vec4(p - vx - vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p - vx + vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx - vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx + vy - vz, 1);
    EmitVertex();
    EndPrimitive(); // face 1

    fNormal = vec3(0.0, 1.0, 0.0);
    gl_Position = transform * vec4(p - vx + vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p - vx + vy + vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx + vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx + vy + vz, 1);
    EmitVertex();
    EndPrimitive(); // face 2

    fNormal = vec3(1.0, 0.0, 0.0);
    gl_Position = transform * vec4(p - vx - vy + vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p - vx + vy + vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx - vy + vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx + vy + vz, 1);
    EmitVertex();
    EndPrimitive(); // face 3

    fNormal = vec3(0.0, -1.0, 0.0);
    gl_Position = transform * vec4(p - vx - vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p - vx - vy + vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx - vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx - vy + vz, 1);
    EmitVertex();
    EndPrimitive(); // face 4

    fNormal = vec3(-1.0, 0.0, 0.0);
    gl_Position = transform * vec4(p + vx - vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx + vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx - vy + vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p + vx + vy + vz, 1);
    EmitVertex();
    EndPrimitive(); // face

    fNormal = vec3(1.0, 0.0, 0.0);
    gl_Position = transform * vec4(p - vx - vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p - vx - vy + vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p - vx + vy - vz, 1);
    EmitVertex();
    gl_Position = transform * vec4(p - vx + vy + vz, 1);
    EmitVertex();
    EndPrimitive(); // face 6
}

#pragma end:geom

#pragma begin:fragment

#version 330

in vec3 fColor;
in vec3 fNormal;
out vec4 outColor;

void main(void) {
    outColor = vec4(fColor, 1.0);
}

#pragma end:fragment

