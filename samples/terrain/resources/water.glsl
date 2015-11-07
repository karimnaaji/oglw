#pragma begin:vertex
#version 330

in vec2 position;
in vec2 uv;

uniform mat4 mvp;
uniform float time;
uniform float yWaterPlane;

out vec2 fUV;
out vec3 fPos;

float pi = 3.14;
float waveAmplitude = 0.05;
float waveFreq = 100.0 * 2 * pi;
float waveSpeed = 0.005;

float distanceFrom(float x, float y) {
    return sqrt(x * x + y * y);
}

float addWave(float xSource, float ySource, float speed, float amplitude, float frequency, float waveLength) {
    return amplitude * cos((time * speed - waveLength * distanceFrom(xSource, ySource)) * frequency);
}

void main() {
    float a0 = addWave(uv.x * 0.5, uv.x * 0.5, waveSpeed * 10.0, waveAmplitude * 2.0, 0.1 * waveFreq, 2.0);
    float a1 = addWave(uv.x, uv.y, waveSpeed, waveAmplitude, waveFreq, 3.0);
    float a2 = addWave(0.0, uv.y, waveSpeed, waveAmplitude, 0.5 * waveFreq, 1.0);
    float a3 = addWave(uv.x, 0.0, waveSpeed, waveAmplitude, 0.5 * waveFreq, 2.0);
    float a4 = addWave(uv.x * 0.5, uv.y * 0.5, waveSpeed, waveAmplitude, 0.5 * waveFreq, 2.0);
    float a5 = addWave(0.0, 0.0, waveSpeed, waveAmplitude, .5 * waveFreq, 2.0);

    vec3 pos = vec3(position, yWaterPlane + 0.3 * (a0 + a1 + a2 + a3 + a4 + a5));
    gl_Position = mvp * vec4(pos, 1.0);

    fUV = uv;
    fPos = pos;
}

#pragma end:vertex

#pragma begin:fragment
#version 330

out vec4 outColour;

in vec2 fUV;
in vec3 fPos;

uniform mat3 normalMatrix;
uniform mat4 modelView;

void main(void) {
    vec3 p0 = dFdx(fPos);
    vec3 p1 = dFdy(fPos);
    vec3 n = normalMatrix * normalize(cross(p0, p1));

    vec3 surfaceColor = vec3(0.6, 0.8, 0.96);
    float ambientIntensity = 1.0;

    vec3 surfaceToLight = normalize(modelView * vec4(0.0, -15.0, 0.0, 0.0)).xyz;
    vec3 surfaceToCamera = normalize(-modelView * vec4(fPos, 0.0)).xyz;

    vec3 ambient = ambientIntensity * surfaceColor;
    float diffuseCoefficient = max(0.0, dot(n, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor;

    vec3 lightFactor = ambient + diffuse;

    outColour = vec4(lightFactor, 0.3 + 0.2 * diffuseCoefficient);
}

#pragma end:fragment
