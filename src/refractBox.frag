#version 460 core
in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main(){
    float ratio = 1.00/1.52;
    vec3 I = normalize(Position - viewPos);
    vec3 R = refract(I,normalize(Normal),ratio);
    FragColor = vec4(texture(skybox,R).rgb,1.0);
}