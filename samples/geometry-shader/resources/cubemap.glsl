#pragma begin:vertex
#version 330
uniform mat4 u_viewProj;

attribute vec3 a_position;

varying vec3 v_uv;

void main() {
    v_uv = a_position;
    vec4 pos = u_viewProj * vec4(a_position, 1.0);
    // force depth to 1.0
    gl_Position = pos.xyww;
}

#pragma end:vertex

#pragma begin:fragment
#version 330
uniform samplerCube u_tex;

varying vec3 v_uv;

void main() {
    gl_FragColor = textureCube(u_tex, v_uv);
}
#pragma end:fragment