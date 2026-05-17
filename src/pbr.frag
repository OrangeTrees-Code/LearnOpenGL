#version 460 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 camPos;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;


struct Light{
    vec3 Position;
    vec3 Color;
};

const int N_LIGHTS = 4;
const float PI = 3.14159265358979;
uniform Light lights[N_LIGHTS];

vec3 fresnelSchlick(float cosTheta, vec3 F0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 getNormalFromMap();

void main(){
    vec3 albedo = texture(albedoMap,TexCoords).rgb;
    float metallic = texture(metallicMap,TexCoords).r;
    float roughness = texture(roughnessMap,TexCoords).r;
    float ao = texture(aoMap,TexCoords).r;

    vec3 N = normalize(getNormalFromMap());
    vec3 V = normalize(camPos - FragPos);
    vec3 Lo = vec3(0.0);
    for(int i = 0;i < N_LIGHTS;i++){
        vec3 Li = normalize(lights[i].Position - FragPos);
        vec3 H = normalize(Li + V);

        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / pow(distance + 1,2);
        vec3 radiance = lights[i].Color * attenuation;

        vec3 F0 = vec3(0.04);
        F0 = mix(F0,albedo,metallic);
        vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
        float D = DistributionGGX(N,H,roughness);
        float G = GeometrySmith(N,V,Li,roughness);

        vec3 nominator    = D * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, Li), 0.0) + 0.001; 
        vec3 specular     = nominator / denominator;  

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;

        kD *= 1.0 - metallic; 

        float NdotLi = max(dot(N, Li), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotLi;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color   = ambient + Lo;  

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color,1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
