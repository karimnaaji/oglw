#version 150

in vec3 position;
in vec3 color;
in vec3 normal;
in vec2 uv;

uniform mat4 mvp;
uniform mat4 mv;
uniform float f;

out vec3 incident;
out vec3 n;
out vec2 uvs;

out float ratio;

void main() {
    vec3 p = position + normal * -abs(f);
    vec4 eyep = vec4(p, 1.0);
    incident = normalize(vec3(mv * eyep));
    n = normal;
    uvs = uv;

    ratio = pow((1.0 - dot(-incident, normal)), 6.0 * f);

    gl_Position = mvp * vec4(p, 1.0);
}
