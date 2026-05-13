#version 460 core

in VS_OUT{
    vec2 TexCoords;
    vec3 FragPos;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
}fs_in;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthMap;

uniform float height_scale;

uniform bool useTexture;

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 lightPos;
uniform int Shininess;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir){
    const float minLayerNum = 8.0;
    const float maxLayerNum = 24.0;
    float numLayers = mix(minLayerNum,maxLayerNum,abs(dot(vec3(0,0,1),viewDir)));

    float layerDepth = 1.0/numLayers;
    float curr_depth = 0.0;
    vec2 p = viewDir.xy * height_scale;
    vec2 deltaTexCoords = p / numLayers;

    vec2 curr_texCoords = texCoords;
    float curr_depthMax = texture(depthMap,curr_texCoords).r;

    while(curr_depth < curr_depthMax){
        curr_depth += layerDepth;
        curr_texCoords -= deltaTexCoords;
        curr_depthMax = texture(depthMap,curr_texCoords).r;
    }

        // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = curr_texCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = curr_depthMax - curr_depth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - curr_depth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + curr_texCoords * (1.0 - weight);

    return finalTexCoords;
}

void main(){

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords,viewDir);

    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0){
        discard;
    }
    
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = texture(normalTexture, texCoords).xyz;
    norm = normalize(norm * 2.0 - 1.0);
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);

    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff * lightColor;


    vec3 reflectDir = reflect(-lightDir,norm);

    float spec = 0;
    if(diff > 0){
        vec3 halfWay = normalize(viewDir + lightDir);
        spec = pow(max(dot(norm,halfWay),0.0),Shininess);
    }

    float distance = length(fs_in.FragPos - lightPos);
    float attenuation = 1.0 / pow(distance + 1,2);

    vec3 specular = specularStrength * spec * lightColor;
    vec3 baseColor = useTexture ? texture(diffuseTexture, texCoords).rgb : objectColor;
    FragColor = vec4((ambient + attenuation * (diffuse + specular)) * baseColor, 1.0);
}
