#version 460 core
in vec3 textureDir;
out vec4 FragColor;

uniform samplerCube cubeMap;
void main(){
    FragColor = texture(cubeMap,textureDir);
}