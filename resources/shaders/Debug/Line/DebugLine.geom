#version 330 core
layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 v_color[];
in float v_thikness[];

layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

out vec4 g_color;
out vec2 g_p0;
out vec2 g_p1;
out float g_half_thickness;

void EmitCorner(vec2 screenPos, float clipW, float clipZ) {
    vec2 ndc = (screenPos / Resolution) * 2.0 - 1.0;
    vec4 clip = vec4(ndc * clipW, clipZ, clipW);
    gl_Position = clip;
    EmitVertex();
}

void main() {
    vec4 p0_clip = gl_in[0].gl_Position;
    vec4 p1_clip = gl_in[1].gl_Position;

    vec2 p0_ndc = p0_clip.xy / p0_clip.w;
    vec2 p1_ndc = p1_clip.xy / p1_clip.w;

    vec2 p0_screen = (p0_ndc * 0.5 + 0.5) * Resolution;
    vec2 p1_screen = (p1_ndc * 0.5 + 0.5) * Resolution;

    float thickness = max(v_thikness[0], v_thikness[1]);
    float half_thickness = thickness * 0.5;


    vec2 dir = p1_screen - p0_screen;
    float len = length(dir);
    vec2 dir_norm = (len > 1e-6) ? normalize(dir) : vec2(1.0, 0.0);
    vec2 perp = vec2(-dir_norm.y, dir_norm.x);

    vec2 p0_ext = p0_screen - dir_norm * half_thickness;
    vec2 p1_ext = p1_screen + dir_norm * half_thickness;

    vec2 offset = perp * half_thickness;

    g_color = v_color[0];
    g_p0 = p0_screen;
    g_p1 = p1_screen;
    g_half_thickness = half_thickness;

    EmitCorner(p0_ext + offset, p0_clip.w, p0_clip.z);
    EmitCorner(p0_ext - offset, p0_clip.w, p0_clip.z);
    EmitCorner(p1_ext + offset, p1_clip.w, p1_clip.z);
    EmitCorner(p1_ext - offset, p1_clip.w, p1_clip.z);

    EndPrimitive();
}
