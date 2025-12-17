#version 330 core

out vec4 fragColor;

layout (std140) uniform MainBlock {
    mat4 projectionView;
    vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};



const float PI = 3.14159265359;

float sdCross( in vec2 p, in vec2 b, float r ) 
{
    p = abs(p); p = (p.y>p.x) ? p.yx : p.xy;
    vec2  q = p - b;
    float k = max(q.y,q.x);
    vec2  w = (k>0.0) ? q : vec2(b.y-p.x,-k);
    return sign(k)*length(max(w,0.0)) + r;
}

vec2 rotate2D(vec2 _st, float _angle){
    _st =  mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle)) * _st;
    return _st;
}

vec3 lerp(vec3 a, vec3 b, float t){
    return a + t*(b - a);
}

vec2 lerp(vec2 a, vec2 b, float t){
    return a + t*(b - a);
}


vec3 hueShift(vec3 color, float hue) {
    const vec3 k = vec3(0.57735, 0.57735, 0.57735);
    float cosAngle = cos(hue);
    return vec3(color * cosAngle + cross(k, color) * sin(hue) + k * dot(k, color) * (1.0 - cosAngle));
}



void main() {

    vec2 uv = (2.0*gl_FragCoord.xy - Resolution.xy)/Resolution.y;
    
    float time = timeProgramm;
    
    float s = sin((time*2.0 - PI)*0.5);    
    float s1 = sin((time*2.0 + PI)*0.5);
    
    float step_s  = (smoothstep(0.0,1.0,s)-0.5) * PI;    
    float step_s1 = (smoothstep(0.0,1.0,s1)-0.5) * PI;
    
	float countRows = 8.0;
    float countColumns = countRows*2.0;
    
    uv *= countRows/2.0;
    
    float row    = floor(uv.y + countRows);
    float column = floor(uv.x + countColumns);
    
    float row_zebra = mod(row,2.0);    
    float column_zebra = mod(column,2.0);
    
    uv += vec2(
        row_zebra   *smoothstep(0.0,1.0,s),
        column_zebra*smoothstep(0.0,1.0,s1));
    
    row = floor(uv.y + countRows);
    column = floor(uv.x + countColumns); 
    
    uv = (fract(uv)-0.5)*2.0;
    
	float shaxmat     =  mod(column + row,2.0);
    float shaxmat_inv =  mod(column + row + 1.0,2.0);
    
    vec2 p = uv;
    uv = rotate2D(uv, step_s *shaxmat);
    uv = rotate2D(uv, step_s1*shaxmat_inv);
    
    vec2 size_from = vec2(0.890,0.780);
    vec2 size_to   = vec2(1.000,0.300);
    
    float time_change_size = (sin(time + sin(time*2.0))*0.5 + 0.5);
    
    vec2 size_current = lerp(size_from, size_to, time_change_size);
    
    float d = sdCross(uv,size_current,0.2);
    
    vec3 col1 = vec3(0.0);
    vec3 col2 = hueShift(vec3(0.632,0.792,0.980)*0.5, time);
    
    vec3 col = lerp(col1,col2,step(d,0.0));
    col = mix( col, col2, 1.0-smoothstep(0.0,0.03,abs(d)) );



    fragColor = vec4(col,1.f);
}
