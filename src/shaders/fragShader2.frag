#version 460 core
in vec3 vertexColor;
out vec4 FragColor;

vec3 hsv2rgb(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0),
                              6.0) - 3.0) - 1.0,
                     0.0, 1.0);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0/3.0, 2.0/3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz),
                 vec4(c.gb, K.xy),
                 step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r),
                 vec4(c.r, p.yzx),
                 step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                d / (q.x + e),
                q.x);
}

uniform float time;

void main(){
    float hue = mod(time * 0.2,1.0);
    vec3 hsv = rgb2hsv(vertexColor);
    hsv.x = mod(hsv.x + hue,1.0);
    FragColor = vec4(hsv2rgb(hsv),1.0f);

}
