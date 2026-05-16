#version 460 core
layout(location = 0) out vec4 gPositionDepth;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gColorSprc;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
}fs_in;

const float NEAR = 0.1; // 投影矩阵的近平面
const float FAR = 100.0; // 投影矩阵的远平面
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // 回到NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));    
}

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_specular_0;

void main(){
    gPositionDepth.xyz = fs_in.FragPos;
    gPositionDepth.w = LinearizeDepth(gl_FragCoord.z);
    gNormal = normalize(fs_in.Normal);
    gColorSprc.rgb = texture(texture_diffuse_0,fs_in.TexCoords).rgb;
    gColorSprc.a = texture(texture_specular_0,fs_in.TexCoords).r;
}
