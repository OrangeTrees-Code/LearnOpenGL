#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D texture_ssao;

struct Light{
    vec3 position;
    vec3 color;
};

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];

const float ambientStrength = 0.08;

const float shininess = 32;

const float intensity = 3.0;
void main(){
    vec3 FragPos = texture(gPositionDepth,TexCoords).xyz;
    vec3 Normal = normalize(texture(gNormal,TexCoords).xyz);
    vec3 Albedo = texture(gColorSpec,TexCoords).rgb;
    float specWeight = texture(gColorSpec,TexCoords).a;

    vec3 viewDir = normalize(-FragPos);

    vec3 lighting = intensity * ambientStrength * Albedo * texture(texture_ssao,TexCoords).r;
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // 漫反射
        vec3 lightDir = normalize(lights[i].position - FragPos);

        float diff = max(dot(Normal,lightDir),0);
        vec3 diffuse = diff * Albedo * lights[i].color;

        float spec = 0;
        if(diff > 0){
            vec3 halfWay = normalize(viewDir + lightDir);
            spec = pow(max(dot(halfWay,Normal),0),shininess);
        }
        vec3 specular = specWeight * spec * lights[i].color;

        float distance = length(FragPos - lights[i].position);
        float attenuation = 1.0 / pow(distance + 1,2);

        lighting += intensity * attenuation * (diffuse + specular);
    }
    

    FragColor = vec4(lighting, 1.0);
}
