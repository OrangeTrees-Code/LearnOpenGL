#version 460 core
#define NR_POINT_LIGHTS 1
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;
struct Material{
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 intensity;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 intensity;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
    
    vec3 intensity;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct Surface {
    vec3 normal;
    vec3 viewDir;
    vec3 diffuseTex;
    vec3 specularTex;
};

struct LightColor {
    vec3 intensity;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightTerms {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform bool switchSpotLight;

uniform samplerCube skybox;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_specular_0;

Surface GetSurface(vec3 normal, vec3 viewDir)
{
    return Surface(
        normal,
        viewDir,
        vec3(texture(texture_diffuse_0, TexCoords)),
        vec3(texture(texture_specular_0, TexCoords))
    );
}

LightColor GetLightColor(DirLight light)
{
    return LightColor(light.intensity, light.ambient, light.diffuse, light.specular);
}

LightColor GetLightColor(PointLight light)
{
    return LightColor(light.intensity, light.ambient, light.diffuse, light.specular);
}

LightColor GetLightColor(SpotLight light)
{
    return LightColor(light.intensity, light.ambient, light.diffuse, light.specular);
}

float CalcAttenuation(float constant, float linear, float quadratic, float distance)
{
    return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

float CalcSpotIntensity(SpotLight light, vec3 lightDir)
{
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    return clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
}

LightTerms CalcLightTerms(LightColor light, Surface surface, vec3 lightDir)
{
    float diff = max(dot(lightDir, surface.normal), 0.0);
    vec3 reflectDir = reflect(-lightDir, surface.normal);
    float spec = diff > 0.0 ? pow(max(dot(surface.viewDir, reflectDir), 0.0), material.shininess) : 0.0;

    return LightTerms(
        light.intensity * light.ambient * surface.diffuseTex,
        light.intensity * light.diffuse * diff * surface.diffuseTex,
        light.intensity * light.specular * spec * surface.specularTex
    );
}

vec3 ApplyLight(LightTerms terms, float ambientScale, float directScale)
{
    return terms.ambient * ambientScale + (terms.diffuse + terms.specular) * directScale;
}

vec3 CalcDirLight(DirLight light, Surface surface)
{
    vec3 lightDir = normalize(-light.direction);
    LightTerms terms = CalcLightTerms(GetLightColor(light), surface, lightDir);
    return ApplyLight(terms, 1.0, 1.0);
}

vec3 CalcPointLight(PointLight light, Surface surface, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = CalcAttenuation(light.constant, light.linear, light.quadratic, distance);
    LightTerms terms = CalcLightTerms(GetLightColor(light), surface, lightDir);
    return ApplyLight(terms, attenuation, attenuation);
}

vec3 CalcSpotLight(SpotLight light, Surface surface, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = CalcAttenuation(light.constant, light.linear, light.quadratic, distance);
    float spotIntensity = CalcSpotIntensity(light, lightDir);
    LightTerms terms = CalcLightTerms(GetLightColor(light), surface, lightDir);
    return ApplyLight(terms, attenuation, attenuation * spotIntensity);
}

void main(){
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    Surface surface = GetSurface(norm, viewDir);
    
    vec3 result = CalcDirLight(dirLight, surface);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], surface, FragPos);   
    if(switchSpotLight == true){
        result += CalcSpotLight(spotLight, surface, FragPos);    
    }

    // vec3 Refelct = reflect(-viewDir,norm);

    FragColor = vec4(result, 1.0);
    // FragColor = texture(texture_diffuse_0,TexCoords);
}
