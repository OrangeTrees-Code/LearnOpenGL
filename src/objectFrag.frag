#version 460 core
in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
}fs_in;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform sampler2D diffuseTexture;
uniform samplerCube shadowMap;
uniform bool useTexture;

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int Shininess;
uniform float farPlane;

float ShadowCalculation(vec3 fragPos, vec3 normal, vec3 lightDir){
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float bias = max(0.15 * (1.0 - dot(normal, lightDir)), 0.03);

    float shadow = 0.0;
    float diskRadius = 0.04;
    vec3 sampleOffsetDirections[20] = vec3[](
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    for(int i = 0; i < 20; ++i){
        float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlane;
        shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }

    return shadow / 20.0;
}   

void main(){
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);

    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff * lightColor;


    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);

    float spec = 0;
    if(diff > 0){
        vec3 halfWay = normalize(viewDir + lightDir);
        spec = pow(max(dot(norm,halfWay),0.0),Shininess);
    }

    float distance = length(fs_in.FragPos - lightPos);
    float attenuation = 1.0 / pow(distance + 1,2);

    vec3 specular = specularStrength * spec * lightColor;
    vec3 baseColor = useTexture ? texture(diffuseTexture, fs_in.TexCoords).rgb : objectColor;

    float shadow = ShadowCalculation(fs_in.FragPos, norm, lightDir);
    FragColor = vec4((ambient + (1 - shadow) * attenuation * (diffuse + specular)) * baseColor,1.0f);
}
