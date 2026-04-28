#version 460 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float visibility;

void main(){
    // FragColor = texture(texture_1,TexCoord) * vec4(vertexColor,1.0f);
    vec2 flipCoord = vec2(1-TexCoord.x,TexCoord.y);
    FragColor = mix(texture(texture_1,TexCoord),texture(texture_2,flipCoord),clamp(visibility,0.0,1.0));
}