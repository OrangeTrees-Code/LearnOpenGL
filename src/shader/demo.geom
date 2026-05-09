#version 460 core

layout(points) in;
layout(line_strip,max_vertices = 6) out;

in vec4[][6] Axis;
out vec3 AxisColor;

void main(){

    vec3 colors[3] = vec3[](
    vec3(1,0,0),
    vec3(0,1,0),
    vec3(0,0,1)
    );

    int counter = 0;
    for(int i = 0;i<6;i++){
        gl_Position = Axis[0][i];
        AxisColor = colors[i/2];
        EmitVertex();
        if(++counter == 2){
            counter = 0;
            EndPrimitive();
        }
    }
}