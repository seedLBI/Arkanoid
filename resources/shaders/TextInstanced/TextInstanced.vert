#version 330 core

layout(location = 0)  in vec2  local_Pos;
layout(location = 1)  in vec2  local_UV;

layout(location = 2)  in vec4  instance_Pos_and_Size;
layout(location = 3)  in vec4  instance_Tangent_and_Normal;
layout(location = 4)  in vec4  instance_UV_min_and_size;
layout(location = 5)  in vec4  instance_Color_base;                    
layout(location = 6)  in vec4  instance_Color_outline;
layout(location = 7)  in vec2  instance_Thickness_base_and_outline;

layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

out vec4  color_base;
out vec4  color_outline;
out float thickness_base;
out float thickness_outline;
out vec2  fragUV;


void main() {
    //unpack
    vec2  pos      = instance_Pos_and_Size.xy;
    vec2  size     = instance_Pos_and_Size.zw;
    vec2  tangent  = instance_Tangent_and_Normal.xy;
    vec2  normal   = instance_Tangent_and_Normal.zw;
    vec2  uv_min   = instance_UV_min_and_size.xy;
    vec2  uv_size  = instance_UV_min_and_size.zw;

    mat2 rotation = mat2(normal,tangent);
    vec2 worldPos = pos + size * local_Pos  * rotation;
    gl_Position = projectionView * vec4(worldPos, 0.0, 1.0);
    
    fragUV = uv_min + local_UV * uv_size;

    color_base = instance_Color_base;
    color_outline = instance_Color_outline;

    thickness_base = instance_Thickness_base_and_outline.x;
    thickness_outline = instance_Thickness_base_and_outline.y;
}