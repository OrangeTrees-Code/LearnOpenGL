#version 460 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 viewPos;

struct Material{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

struct Light{
    vec3 position;

    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;

    vec3 color;
};

uniform Material material;
uniform Light light;

void main(){

    vec3 ambient = light.color * (light.ambientStrength * material.ambientColor);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = light.color * (light.diffuseStrength * diff * material.diffuseColor);


    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);

    // 只有当表面面向光源时才计算高光
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess) /** step(0.0, dot(norm,lightDir))*/;
    vec3 specular = light.color * (light.specularStrength * spec * material.specularColor);

    FragColor = vec4((diffuse + ambient + specular),1.0f);
}