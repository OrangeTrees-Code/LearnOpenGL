#version 460 core
layout(location = 0) in vec3 aPos;

flat out int VertexID;

layout(std140) uniform Matrices{
    mat4 view;
    mat4 projection;
};

uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);    
    // gl_PointSize = gl_Position.z;   

    // vec4 viewPos = view * model * vec4(aPos, 1.0);
    // gl_Position = projection * viewPos;

    // float dist = -viewPos.z;
    // gl_PointSize = clamp(dist * 4.0, 2.0, 80.0);

    
    // gl_PointSize = clamp(80.0 / dist, 2.0, 80.0);

    VertexID = gl_VertexID; 
}