#pragma begin:vertex
#version 330

in vec2 position;
in vec2 uv;

uniform mat4 mvp;
uniform sampler2D tex;
uniform float extrude;
uniform float yOffset;
uniform float scale;

out float fOffset;
out vec3 fPos;

void main() {
    float offset = texture(tex, uv * scale).r;
    vec3 pos = vec3(position, offset * extrude - yOffset);
    gl_Position = mvp * vec4(pos, 1.0);
    fPos = pos;
    fOffset = offset;
}

#pragma end:vertex

#pragma begin:fragment
#version 330

out vec4 outColour;

in float fOffset;
in vec3 fPos;

uniform mat3 normalMatrix;
uniform mat4 modelView;
uniform vec3 color;

void main(void) {
    vec3 p0 = dFdx(fPos);
    vec3 p1 = dFdy(fPos);
    vec3 n = normalMatrix * normalize(cross(p0, p1));

    vec3 surfaceColor = color;
    float ambientIntensity = 0.7;

    vec3 surfaceToLight = normalize(modelView * vec4(0.0, 5.0, 0.0, 0.0)).xyz;
    vec3 surfaceToCamera = normalize(-modelView * vec4(fPos, 0.0)).xyz;

    vec3 ambient = ambientIntensity * surfaceColor;
    float diffuseCoefficient = max(0.0, dot(n, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor;

    vec3 lightFactor = ambient + diffuse;

    outColour = vec4(lightFactor * clamp(fOffset, 0.2, 1.0), 1.0);
    //outColour.rgb = pow(outColour.rgb, vec3(0.4545));
}

#pragma end:fragment
