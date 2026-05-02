#version 460 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material{
    sampler2D diffuseTex;
    sampler2D specularTex;

    // vec3 specularColor;
    float shininess;
    // sampler2D emissionTex;
};

struct Light{
    // vec3 position;
    // vec4 vector;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;

    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

void main(){

    vec3 ambient = (light.color * light.ambientStrength) * vec3(texture(material.diffuseTex,TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir;
    // if(light.vector.w == 1){
    //     lightDir = normalize(light.vector.xyz - FragPos);
    // }
    // else{
    //     lightDir = normalize(-light.vector.xyz);
    // }
    lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   
    if(theta > light.outerCutOff){
        float diff = max(dot(norm,lightDir),0);
        vec3 diffuse = (light.color * light.diffuseStrength) * diff * vec3(texture(material.diffuseTex,TexCoords));

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir,norm);

        // 只有当表面面向光源时才计算高光
        float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess) /** step(0.0, dot(norm,lightDir))*/;
        vec3 specular = (light.color * light.specularStrength) * spec * vec3(texture(material.specularTex,TexCoords));

        // vec3 emission = vec3(texture(material.emissionTex,TexCoords));

        // if(light.vector.w == 1){
        //     float distance = length(light.vector);
        //     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        //     diffuse *= attenuation;
        //     ambient *= attenuation;
        //     specular *= attenuation;
        // }

        diffuse  *= intensity;
        specular *= intensity;  

        FragColor = vec4((diffuse + ambient + specular /*+ emission*/),1.0f);
    }
    else{
        FragColor = vec4((ambient /*+ emission*/),1.0f);
    }


}