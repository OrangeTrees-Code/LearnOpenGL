#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D texture_ssao;
uniform int debugMode;

vec3 EncodePosition(vec3 position) {
    return clamp(position * 0.05 + vec3(0.5), 0.0, 1.0);
}

void main() {
    if (debugMode == 1) {
        float ssao = texture(texture_ssao, TexCoords).r;
        FragColor = vec4(vec3(ssao), 1.0);
        return;
    }

    vec2 quadUV = fract(TexCoords * 2.0);
    vec3 color;

    if (TexCoords.x < 0.5 && TexCoords.y > 0.5) {
        // top-left: view-space position
        color = EncodePosition(texture(gPositionDepth, quadUV).rgb);
    } else if (TexCoords.x > 0.5 && TexCoords.y > 0.5) {
        // top-right: normal
        color = normalize(texture(gNormal, quadUV).rgb) * 0.5 + 0.5;
    } else if (TexCoords.x < 0.5 && TexCoords.y < 0.5) {
        // bottom-left: diffuse color
        color = texture(gColorSpec, quadUV).rgb;
    } else {
        // bottom-right: specular strength in alpha channel
        float spec = texture(gColorSpec, quadUV).a;
        color = vec3(spec);
    }

    FragColor = vec4(color, 1.0);

    
}
