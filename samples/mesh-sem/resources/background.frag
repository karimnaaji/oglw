#version 330

uniform sampler2D tex;
uniform vec2 resolution;

out vec4 outColour;

bool grid(vec2 p, float res){
    vec2 grid = fract(p * res);
    return grid.x < res || grid.y < res;
}

vec3 back(vec2 p){
    vec3 background = vec3(0.03, 0.03, 0.06);
    if(grid(p, 0.05)) { background += vec3(0.02); }
    return background;
}

void main(void) {
    vec2 uv = gl_FragCoord.xy / resolution;
    vec2 p = uv * 2.0 - 1.0;

    vec3 texColor = texture(tex, (uv + vec2(0.5)) * 0.4).rgb;
    outColour = vec4(mix(back(gl_FragCoord.xy), texColor, 0.4), 1.0);
}
