#version 330 core

layout(location = 0)  in vec2  localPos;
layout(location = 1)  in vec2  localUV;

layout(location = 2)  in vec4  instance_Pos_and_Size;
layout(location = 3)  in vec4  instance_Tangent_and_Normal;
layout(location = 4)  in vec4  instance_Color;                    


layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

out vec4  color;

void main() {
    //unpack
    vec2  pos       = instance_Pos_and_Size.xy;
    vec2  size      = instance_Pos_and_Size.zw;
    vec2  tangent   = instance_Tangent_and_Normal.xy;
    vec2  normal    = instance_Tangent_and_Normal.zw;


    mat2 rotation = mat2(tangent,normal);
    vec2 worldPos = pos +  localPos * size * rotation ;
    gl_Position = projectionView * vec4(worldPos, 0.0, 1.0);

    color = instance_Color;
}