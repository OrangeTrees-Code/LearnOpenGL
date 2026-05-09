#version 460 core
layout(location = 0) in vec3 aPos;

out vec3 textureDir;

layout(std140,binding = 0) uniform Matrices{

    mat4 view;

    mat4 projection;

};


void main(){
    textureDir = aPos;
    vec4 clipPos = projection * mat4(mat3(view)) * vec4(aPos,1.0f);
    gl_Position = clipPos.xyww;

}