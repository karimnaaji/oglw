#version 150

uniform vec2 resolution;

out vec4 outColour;

bool grid(vec2 p, float res){
    vec2 grid = fract(p * res);
    return grid.x < res || grid.y < res;
}

vec3 back(vec2 p){
    vec3 background = vec3(0.01, 0.05, 0.06); 
    if(grid(p, 0.01)) { background += vec3(0.05); }
    if(grid(p, 0.05)) { background += vec3(0.02); }
    return background;
}

void main(void) {
    vec2 uv = (gl_FragCoord.xy / resolution) * 2.0 - vec2(1.0);
    float vignette = smoothstep(0.9, 0.5, length(uv));
    outColour = vec4(back(gl_FragCoord.xy) * vignette, 1.0);
}
