#version 460 core
in vec3 AxisColor;

out vec4 FragColor;

float hash(vec2 p)
{
    vec3 p3 = fract(vec3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

float randomRange(vec2 p, float a, float b)
{
    return mix(a, b, hash(p));
}


void main(){
    // float possi = randomRange(gl_FragCoord.xy,0.0,100.0);
    // if(possi < 33.3){
    //     FragColor = vec4(1,0,0,1);
    // }
    // else if(possi < 66.6){
    //     FragColor = vec4(0,1,0,1);
    // }
    // else{
    //     FragColor = vec4(0,0,1,1);
    // }

    FragColor = vec4(AxisColor , 1.0);
}