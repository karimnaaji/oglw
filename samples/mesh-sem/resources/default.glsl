#pragma begin:vertex
#version 330

in vec3 position;
in vec3 color;
in vec3 normal;
in vec2 uv;

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

#pragma end:vertex

#pragma begin:fragment
#version 330

uniform sampler2D tex;
uniform mat3 normalmat;

out vec4 outColour;

in vec3 incident;
in vec3 n;
in float ratio;

void main(void) {
    vec3 r = reflect(incident, normalize(normalmat * n));

    float m = 2.0 * sqrt(
        pow(r.x, 2.0) +
        pow(r.y, 2.0)  +
        pow(r.z + 1.0, 2.0)
    );
    vec2 uv = r.xy / m + 0.5;

    outColour = vec4(mix(vec3(0.1, 0.1, 0.1), texture(tex, uv).rgb, ratio), 1.0);
}

#pragma end:fragment
