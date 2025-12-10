#version 330 core

out vec4 fragColor;
in vec4 color;
in vec2 uv;
in float thikness;

uniform sampler2D u_sdf;
uniform float u_radius = 0.48;
uniform float u_range = 0.15;

void main() {
    float s = texture(u_sdf, uv).r; // 0..1
    float d = (0.5 - s) * 2.0 * u_range; // d < 0 => in, d = 0 => out
    float aa = max(fwidth(d), 0.0005);
    float thickness = clamp(thikness, 0.0, 1.0);
    float inner_r = u_radius * (1.0 - thickness);
    float inner_cutoff_d = inner_r - u_radius;
    float outerMask = 1.0 - smoothstep(-aa, aa, d);
    float innerMask = smoothstep(inner_cutoff_d - aa, inner_cutoff_d + aa, d);
    float alpha = outerMask * innerMask;
    fragColor = vec4(color.rgb, color.a * alpha);

}
