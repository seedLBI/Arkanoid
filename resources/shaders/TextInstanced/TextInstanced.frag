#version 330 core

out vec4 fragColor;

in vec4  color_base;
in vec4  color_outline;
in float thickness_base;
in float thickness_outline;
in vec2  fragUV;

layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

uniform float pxRange = 21.f;
uniform sampler2D mtsdf_texture;

float screenPxRange() {
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(mtsdf_texture, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(fragUV);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(vec3 msdf) {
    return max(min(msdf.r, msdf.g), min(max(msdf.r, msdf.g), msdf.b));
}

void main() {
     float dist_sdf  = texture(mtsdf_texture, fragUV).a;
     float dist_msdf = median(texture(mtsdf_texture, fragUV).rgb);

     float a = sin(timeProgramm) * 0.5 + 0.5;

     float screenPxDistance = screenPxRange() * (dist_msdf - 0.5);


     //float base     = smoothstep(-0.2,1.0,screenPxDistance);
     //float outline  = smoothstep(0.01,0.0,screenPxDistance);



     float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);




     vec4 color = mix(vec4(0.0),color_base, opacity);
     fragColor = color;
     //fragColor = vec4(vec3(abs(dist_msdf)),1.0);
}
