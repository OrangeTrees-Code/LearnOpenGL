#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    mat4 modelView = view * model;
    gl_Position = projection * modelView * vec4(aPos,1.0f);
    vs_out.FragPos = vec3(modelView * vec4(aPos,1.0f));
    vs_out.Normal = mat3(transpose(inverse(modelView))) * aNormal;  
    vs_out.TexCoords = aTexCoords;
}
