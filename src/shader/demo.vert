#version 460 core
layout(location = 0) in vec3 aPos;
out vec4[6] Axis;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos,1.0);
    vec4 axis[6] = vec4[](
        worldPos + vec4(-0.1,0,0,0),
        worldPos + vec4(0.1,0,0,0),
        worldPos + vec4(0,0.1,0,0),
        worldPos + vec4(0,-0.1,0,0),
        worldPos + vec4(0,0,0.1,0),
        worldPos + vec4(0,0,-0.1,0)
    );

    gl_Position = projection * view * worldPos;  

    for(int i = 0;i<6;i++){
        axis[i] = projection * view * axis[i];
    };

    Axis = axis;  
}