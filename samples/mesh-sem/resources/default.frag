#version 150

uniform sampler2D tex;
uniform mat3 normalmat;

out vec4 outColour;

in vec3 incident;
in vec3 n;
in vec2 uvs;
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