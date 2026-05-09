#version 460 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;

uniform float time;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    mat4 ProjectionM;
}gs_in[];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

vec3 GetNormal(){
    vec3 a = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 b = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
    return normalize(cross(a,b));
}

const float distance = 0.2;
void explode(vec3 N){
    for(int i = 0;i<3;i++){
        Normal = gs_in[i].Normal;
        FragPos = gs_in[i].FragPos;
        TexCoords = gs_in[i].TexCoords;

        vec3 direction = N * ((sin(time) + 1.0) / 2.0) * distance;
        gl_Position  = gl_in[i].gl_Position + vec4(direction,0.0);
        gl_Position = gs_in[i].ProjectionM * gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}

void main(){
    vec3 N = GetNormal();

    explode(N);
}
