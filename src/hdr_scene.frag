#version 460 core

struct Light {
    vec3 Position;
    vec3 Color;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 objectColor;
uniform sampler2D diffuseTexture;
uniform bool useTexture;

uniform float ambientStrength;
uniform float specularStrength;
uniform int Shininess;
uniform vec3 viewPos;

uniform int lightCount;
uniform Light lights[16];

void main(){
    vec3 baseColor = useTexture ? texture(diffuseTexture, TexCoords).rgb : objectColor;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lighting = ambientStrength * baseColor;

    for(int i = 0; i < lightCount; ++i){
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);

        vec3 halfWay = normalize(viewDir + lightDir);
        float spec = 0.0;
        if(diff > 0.0){
            spec = pow(max(dot(norm, halfWay), 0.0), Shininess);
        }

        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / (1.0 + 0.18 * distance + 0.08 * distance * distance);

        vec3 diffuse = diff * baseColor * lights[i].Color;
        vec3 specular = specularStrength * spec * lights[i].Color;
        lighting += attenuation * (diffuse + specular);
    }

    FragColor = vec4(lighting, 1.0);
}
