#version 460 core
in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
}fs_in;

out vec4 FragColor;


uniform sampler2D diffuseTexture;
uniform samplerCube cubeShadowMap;
uniform float farPlane;


uniform bool useTexture;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int Shininess;

const vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);


float ShadowCalculation(vec3 fragPos){
    vec3 sampleDir = fragPos - lightPos;

    float currDistance = length(sampleDir);


    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(cubeShadowMap, sampleDir + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlane;
        if(currDistance - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
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

    float shadow = ShadowCalculation(fs_in.FragPos);
    FragColor = vec4((ambient + (1 - shadow) * attenuation * (diffuse + specular)) * baseColor,1.0f);
}
