#version 460 core

in GS_OUT{
    vec4 FragPos;
}fs_in;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{             
    float distance = length(fs_in.FragPos.xyz - lightPos);
    gl_FragDepth = distance / farPlane;
}
