#version 460 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform sampler2D diffuseTexture;
uniform bool useTexture;

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

    float spec = 0;
    if(diff > 0){
        vec3 halfWay = normalize(viewDir + lightDir);
        spec = pow(max(dot(norm,halfWay),0.0),Shininess);
    }

    float distance = length(FragPos - lightPos);
    float attenuation = 1.0 / pow(distance + 1,2);

    vec3 specular = specularStrength * spec * lightColor;
    vec3 baseColor = useTexture ? texture(diffuseTexture, TexCoords).rgb : objectColor;
    FragColor = vec4((ambient + attenuation * (diffuse + specular)) * baseColor,1.0f);
}
