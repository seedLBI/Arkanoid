#version 330 core
layout(location = 0) in float a_t;			// 0.0 or 1.0
layout(location = 1) in vec2 a_start;		// per-instance
layout(location = 2) in vec2 a_end;			// per-instance
layout(location = 3) in vec4 a_color;		// per-instance
layout(location = 4) in float a_thikness;	// per-instance

layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

out vec4 v_color;
out float v_thikness;

void main() {
    vec2 pos = mix(a_start, a_end, a_t);
    gl_Position = projectionView * vec4(vec3(pos, 0.0), 1.0);
    v_color = a_color;
	v_thikness = a_thikness;
}
