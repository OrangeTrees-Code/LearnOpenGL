#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

vec3 EncodePosition(vec3 position) {
    return clamp(position * 0.05 + vec3(0.5), 0.0, 1.0);
}

void main() {
    vec2 quadUV = fract(TexCoords * 2.0);
    vec3 color;

    if (TexCoords.x < 0.5 && TexCoords.y > 0.5) {
        // top-left: world position
        color = EncodePosition(texture(gPosition, quadUV).rgb);
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
