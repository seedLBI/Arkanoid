#version 330 core
layout(location = 0) in vec2 a_pos;

layout(location = 1) in float instance_thikness;	// instance
layout(location = 2) in vec4  instance_color;		// instance
layout(location = 3) in mat4  instance_model;		// instance

layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

out float thikness;
out vec2 uv;
out vec4 color;

void main() {
    gl_Position = projectionView * instance_model * vec4(vec3(a_pos, 0.0), 1.0);
	uv = a_pos * 0.5 + 0.5;
    color = instance_color;
	thikness = instance_thikness;
}
