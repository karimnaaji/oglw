#pragma begin:vertex
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 mv;

out vec3 incident;
out vec3 n;

void main() {
    vec4 eyep = vec4(position, 1.0);
    incident = normalize(vec3(mv * eyep));
    n = normal;

    gl_Position = mvp * vec4(position, 1.0);
}

#pragma end:vertex
#pragma begin:fragment
#version 330

uniform sampler2D tex;
uniform mat3 normalmat;

out vec4 outColour;

in vec3 incident;
in vec3 n;

void main(void) {
    vec3 r = reflect(incident, normalize(normalmat * n));

    float m = 2.0 * sqrt(
        pow(r.x, 2.0) +
        pow(r.y, 2.0)  +
        pow(r.z + 1.0, 2.0)
    );
    vec2 uv = r.xy / m + 0.5;

    outColour = vec4(texture(tex, uv).rgb, 1.0);
}

#pragma end:fragment
#pragma begin:geom
#version 330

layout(points) in;
layout(points, max_vertices = 1) out;

void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}

#pragma end:geom
