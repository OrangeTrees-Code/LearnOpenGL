#version 460 core
in vec3 Position;
in vec3 Normal;
in vec3 ObjectPosition;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform samplerCube skybox;
uniform mat4 model;
uniform mat4 inverseModel;

const float AIR_IOR = 1.0;
const float GLASS_IOR = 1.52;
const vec3 BOX_MIN = vec3(-0.5);
const vec3 BOX_MAX = vec3(0.5);

vec3 boxExitNormal(vec3 p, vec3 dir)
{
    vec3 safeDir = mix(dir, vec3(1e-6), lessThan(abs(dir), vec3(1e-6)));
    vec3 t0 = (BOX_MIN - p) / safeDir;
    vec3 t1 = (BOX_MAX - p) / safeDir;
    vec3 tFar = max(t0, t1);

    float tExit = min(min(tFar.x, tFar.y), tFar.z);
    vec3 exitPos = p + dir * tExit;

    vec3 dMin = abs(exitPos - BOX_MIN);
    vec3 dMax = abs(exitPos - BOX_MAX);
    float d = min(min(min(dMin.x, dMax.x), min(dMin.y, dMax.y)), min(dMin.z, dMax.z));

    if (d == dMin.x) return vec3(-1.0, 0.0, 0.0);
    if (d == dMax.x) return vec3( 1.0, 0.0, 0.0);
    if (d == dMin.y) return vec3(0.0, -1.0, 0.0);
    if (d == dMax.y) return vec3(0.0,  1.0, 0.0);
    if (d == dMin.z) return vec3(0.0, 0.0, -1.0);
    return vec3(0.0, 0.0, 1.0);
}

void main(){
    vec3 viewPosObject = vec3(inverseModel * vec4(viewPos, 1.0));
    vec3 incidentObject = normalize(ObjectPosition - viewPosObject);
    vec3 normalObject = normalize(mat3(transpose(model)) * normalize(Normal));

    vec3 insideRayObject = refract(incidentObject, normalObject, AIR_IOR / GLASS_IOR);
    if (length(insideRayObject) < 1e-5) {
        vec3 reflectedWorld = reflect(normalize(Position - viewPos), normalize(Normal));
        FragColor = vec4(texture(skybox, reflectedWorld).rgb, 1.0);
        return;
    }

    vec3 exitNormalObject = boxExitNormal(ObjectPosition + insideRayObject * 1e-4, insideRayObject);
    vec3 exitRayObject = refract(insideRayObject, -exitNormalObject, GLASS_IOR / AIR_IOR);
    if (length(exitRayObject) < 1e-5) {
        exitRayObject = reflect(insideRayObject, -exitNormalObject);
    }

    vec3 exitRayWorld = normalize(mat3(model) * exitRayObject);
    FragColor = vec4(texture(skybox, exitRayWorld).rgb, 1.0);
}
