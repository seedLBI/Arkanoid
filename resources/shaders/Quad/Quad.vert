#version 330 core

layout(location = 0)  in vec2  localPos;
layout(location = 1)  in vec2  localUV;

layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

void main() {
    gl_Position =  vec4((localUV - 0.5)*2.0, 0.0, 1.0);
}