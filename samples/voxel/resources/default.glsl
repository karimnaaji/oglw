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
uniform mat3 normalMatrix;
uniform vec3 viewPos;
uniform mat4 view;

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in vec3 gColor[];
out vec3 fColor;
out vec3 fNormal;
out vec4 fPos;

void main() {
    const vec3 vx = vec3(0.5, 0, 0);
    const vec3 vy = vec3(0, 0.5, 0);
    const vec3 vz = vec3(0, 0, 0.5);

    fColor = gColor[0];
    vec3 p = gl_in[0].gl_Position.xyz;

    fNormal = normalize(normalMatrix * vec3(0.0, 0.0, -1.0));
    fPos = vec4(p - vx - vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p - vx + vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx - vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx + vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    EndPrimitive(); // face 1

    fNormal = normalize(normalMatrix * vec3(0.0, 1.0, 0.0));
    fPos = vec4(p - vx + vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p - vx + vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx + vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx + vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    EndPrimitive(); // face 2

    fNormal = normalize(normalMatrix * vec3(1.0, 0.0, 0.0));
    fPos = vec4(p - vx - vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p - vx + vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx - vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx + vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    EndPrimitive(); // face 3

    fNormal = normalize(normalMatrix * vec3(0.0, -1.0, 0.0));
    fPos = vec4(p - vx - vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p - vx - vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx - vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx - vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    EndPrimitive(); // face 4

    fNormal = normalize(normalMatrix * vec3(-1.0, 0.0, 0.0));
    fPos = vec4(p + vx - vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx + vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx - vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p + vx + vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    EndPrimitive(); // face

    fNormal = normalize(normalMatrix * vec3(1.0, 0.0, 0.0));
    fPos = vec4(p - vx - vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p - vx - vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p - vx + vy - vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    fPos = vec4(p - vx + vy + vz, 1);
    gl_Position = mvp * fPos;
    EmitVertex();
    EndPrimitive(); // face 6
}

#pragma end:geom

#pragma begin:fragment

#version 330

#define MAX_LIGHTS 1
#define saturate(a) clamp(a, 0.0, 1.0)

struct Light {
    vec3 position;
    vec3 color;
    vec3 ambiant;
    float diffuseIntensity;
    float specularIntensity;
};

uniform vec3 viewPos;
uniform mat4 view;
uniform Light light;

in vec3 fColor;
in vec3 fNormal;
in vec4 fPos;
out vec4 outColor;

vec3 toLinear(vec3 a) {
    return pow(a, vec3(0.4545));
}

vec3 calcLight(Light light, vec3 vertexToLight, vec3 normal, vec3 vertexToEye, float materialSpecularIntensity) {
    vec3 ambiantColor = light.color * light.ambiant;
    float cosineDiffuseTerm = dot(normal, vertexToLight);
    vec3 diffuseColor = vec3(0);
    vec3 specularColor = vec3(0);

    if (cosineDiffuseTerm < 0) {
        diffuseColor = light.color * light.diffuseIntensity * cosineDiffuseTerm;
        vec3 lightReflect = normalize(reflect(vertexToLight, normal));
        float specularFactor = dot(vertexToEye, lightReflect);

        if (specularFactor < 0) {
            specularFactor = pow(specularFactor, light.specularIntensity);
            specularColor = light.color * materialSpecularIntensity * specularFactor;
        }
    }

    return ambiantColor + specularColor + diffuseColor;
}

void main(void) {
    vec4 viewVertexPos = view * fPos;
    vec3 vertexToEye = normalize(viewPos - viewVertexPos.xyz);
    vec4 viewLightPosition = view * vec4(light.position, 1.0);
    vec3 vertexToLight = normalize(viewLightPosition.xyz - viewVertexPos.xyz);

    outColor = vec4(calcLight(light, vertexToLight, fNormal, vertexToEye, 2.5), 1.0);
    outColor.rgb = toLinear(outColor.rgb) * fColor;
}

#pragma end:fragment

