#version 330 core

layout(location = 0)  in vec2  localPos;

layout(location = 2)  in vec2  instance_vertex_1;
layout(location = 3)  in vec2  instance_vertex_2;                
layout(location = 4)  in vec2  instance_vertex_3;                
layout(location = 5)  in vec4  instance_color;    



layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

out vec4  color;

void main() {
    vec2 vertexes[3] = {instance_vertex_1, instance_vertex_2, instance_vertex_3}; 
    gl_Position = projectionView * vec4(vertexes[gl_VertexID], 0.0, 1.0);

    color = instance_color;
}