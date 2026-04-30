#version 460 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int Shininess;


void main(){
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff * lightColor;


    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);

    // 只有当表面面向光源时才计算高光
    float spec = pow(max(dot(viewDir,reflectDir),0.0),Shininess) /** step(0.0, dot(norm,lightDir))*/;
    vec3 specular = specularStrength * spec * lightColor;

    FragColor = vec4((diffuse + ambient + specular) * objectColor,1.0f);
}