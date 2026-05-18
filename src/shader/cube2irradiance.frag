#version 460 core
in vec3 localPos;
out vec4 FragColor;

uniform samplerCube environmentMap;

const float PI = 3.14159265358979;

void main(){
    vec3 normal = normalize(localPos);

    vec3 irradiance = vec3(0.0);

    vec3 up = abs(normal.y) > 0.999 ? vec3(1.0,0.0,0.0) : vec3(0.0,1.0,0.0);
    vec3 right = normalize(cross(up,normal));
    up = normalize(cross(normal,right));

    float sampleDelta = 0.025;
    int numSamples = 0;
    for(float phi = 0.0;phi < 2 * PI;phi += sampleDelta){
        for(float theta = 0.0;theta < PI / 2.0;theta += sampleDelta){
            vec3 tangentSample = vec3(sin(theta) * cos(phi),sin(theta) * sin(phi),cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            irradiance += texture(environmentMap,sampleVec).rgb * cos(theta) * sin(theta);
            numSamples++;
        }
    }

    irradiance = irradiance * PI / float(numSamples);



    FragColor = vec4(irradiance,1.0);

}
