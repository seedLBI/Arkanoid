#version 330 core
in vec4 g_color;
in vec2 g_p0;
in vec2 g_p1;
in float g_half_thickness;

out vec4 fragColor;

uniform float smoothness = 2.f;

float sdSegment(vec2 p, vec2 a, vec2 b) {
    vec2 pa = p - a;
    vec2 ba = b - a;
    float ba_len2 = dot(ba, ba);
    if (ba_len2 < 1e-8) return length(pa);
    float h = clamp(dot(pa, ba) / ba_len2, 0.0, 1.0);
    vec2 proj = a + ba * h;
    return length(p - proj);
}

void main() {
    vec2 p = gl_FragCoord.xy;

    float d = sdSegment(p, g_p0, g_p1);

    d = min(d, length(p - g_p0));
    d = min(d, length(p - g_p1));

    float aa = fwidth(d);
    aa = max(aa, 0.5);

    float edge0 = g_half_thickness - smoothness - aa * 0.5;
    float edge1 = g_half_thickness + smoothness + aa * 0.5;

    float alpha = 1.0 - pow( smoothstep(edge0, edge1, d),0.4);
    alpha = clamp(alpha, 0.0, 1.0) * g_color.a;

    fragColor = vec4(g_color.rgb,alpha);
}
