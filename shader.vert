#define scaling 1.0

float height(float x, float y, float t) {
    return sin(x)*cos(y);
}

vec3 normal(float x, float y, float t) {
    float dh_dx = cos(x)*cos(y);
    float dh_dy = -sin(x)*sin(y);
    lowp vec3 t1 = normalize(vec3(1.0, 0.0, dh_dx));
    lowp vec3 t2 = normalize(vec3(0.0, 1.0, dh_dy));
    return cross(t1,t2);
}

attribute vec4 a_position;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 cameraPosition;
uniform float time;
// uniform float scaling;
varying vec3 norm;
uniform float lightFalloffDistance;
varying float light;
void main() {
    float x = scaling*(a_position.x - cameraPosition.y);
    float y = scaling*(a_position.y - cameraPosition.x);
    float z = height(x, y, time)/scaling - cameraPosition.z;
    vec4 position = a_position;
    position.z = z;
    gl_Position = modelViewProjectionMatrix*position;
    norm = normal(x,y,time);
    highp vec4 lightPosition = modelViewProjectionMatrix*position;
    highp float lightDistance = min(lightPosition.z, gl_Position.z);
    light = clamp((lightFalloffDistance * 0.85 - lightDistance) / (lightFalloffDistance * 0.7), 0.4, 1.0);
}