#version 460 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gColorSprc;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
}fs_in;


uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_specular_0;

void main(){
    gPosition = fs_in.FragPos;
    gNormal = normalize(fs_in.Normal);
    gColorSprc.rgb = texture(texture_diffuse_0,fs_in.TexCoords).rgb;
    gColorSprc.a = texture(texture_specular_0,fs_in.TexCoords).r;
}
