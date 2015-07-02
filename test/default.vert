#version 150

in vec3 position;
in vec3 color;
in vec3 normal;

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
