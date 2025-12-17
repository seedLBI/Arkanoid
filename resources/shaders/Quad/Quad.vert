#version 330 core

layout(location = 0)  in vec2  localPos;
layout(location = 1)  in vec2  localUV;

layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

out vec2 input_uv;

void main() {
    gl_Position = projectionView * vec4(localPos * Resolution.xy, 0.0, 1.0);
    input_uv = (localUV - 0.5)*2.0;
}