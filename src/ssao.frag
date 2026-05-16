#version 460 core
out float FragColor;
in vec2 TexCoords;

uniform vec3 samples[64];
uniform mat4 projection;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

const float radius = 1.0;

const int kernelSize = 64;

const vec2 noiseScale = vec2(1920.0/4.0, 1080.0/4.0);
void main(){
    vec3 fragPos = texture(gPositionDepth,TexCoords).xyz;
    vec3 normal = texture(gNormal,TexCoords).xyz;//view space
    vec3 randomVec = texture(texNoise,TexCoords * noiseScale).xyz;//tangent space

    vec3 tangent = normalize(randomVec - normal * dot(randomVec,normal));
    vec3 bitangent = normalize(cross(normal,tangent));
    mat3 TBN = mat3(tangent,bitangent,normal);

    float occlusion = 0.0;
    for(int i = 0;i < kernelSize;i++){
        vec3 sampleVec = TBN * samples[i];

        sampleVec = fragPos + radius * sampleVec;

        vec4 samplePos = projection * vec4(sampleVec,1.0);
        samplePos.xyz /= samplePos.w;
        samplePos.xy = samplePos.xy/2+0.5;

        float sampleDepth = texture(gPositionDepth,samplePos.xy).w; 

        float rangeCheck = smoothstep(0.0,1.0,radius / abs(-fragPos.z - sampleDepth));


        occlusion += (sampleDepth <= (-sampleVec.z) ? 1.0:0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}
