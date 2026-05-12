#version 460 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 18) out;

out GS_OUT{
    vec4 FragPos;
}gs_out;

uniform mat4 shadowMatrices[6];

void main(){
    for(int face = 0;face < 6;++face){
        gl_Layer = face;
        for(int i = 0;i<3;i++){
            gs_out.FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}