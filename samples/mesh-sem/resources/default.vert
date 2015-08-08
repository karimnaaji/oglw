#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
uniform mat4 mv;
uniform float f;

out vec3 incident;
out vec3 n;

out float ratio;

void main() {
    vec3 p = position + normal * -abs(f);
    vec4 eyep = vec4(p, 1.0);
    incident = normalize(vec3(mv * eyep));
    n = normal;

    ratio = pow((1.0 - dot(-incident, normal)), 6.0 * f);

    gl_Position = mvp * vec4(p, 1.0);
}
