#define scaling 1.0
attribute vec4 a_position;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 cameraPosition;
uniform float time;
// uniform float scaling;
varying vec3 norm;
uniform float lightFalloffDistance;
varying float light;

float height(float x, float y, float t) {
    return sin(x)*cos(y)*cos(0.001*x*y*t);
//    float MNsq = (97.0+x)*(97.0+x);
//    float MXsq = (144.0+y)*(144.0+y);
//    float MYsq = 180.0*180.0;
//    float MZsq = 568.0*568.0;
//    return 4.945437808107262e8 + 4.159615180441358e-8*MNsq*MNsq*MNsq*MNsq + 7.967539670936551e-7*MXsq*MXsq*MXsq*MXsq + 494817.83241677977*MYsq + 199.4880856950785*MYsq*MYsq + 0.03587563434801489*MYsq*MYsq*MYsq + 2.4228531018395114e-6*MYsq*MYsq*MYsq*MYsq + MNsq*MNsq*MNsq*(-0.0011214998473257948 - 9.554088632321997e-8*MXsq - 8.39244010410777e-8*MYsq + 1.3080680146643253e-8*MZsq) + MXsq*MXsq*MXsq*(-0.014066467818595288 - 4.038311443284019e-6*MYsq + 2.137854569696517e-7*MZsq) - 28801.102853258384*MZsq - 22.089436120932646*MYsq*MZsq - 0.005945481597836115*MYsq*MYsq*MZsq - 5.349727297268404e-7*MYsq*MYsq*MYsq*MZsq + 0.6333464867338235*MZsq*MZsq + 0.0003289127214880023*MYsq*MZsq*MZsq + 4.43144403847036e-8*MYsq*MYsq*MZsq*MZsq - 6.219682945368267e-6*MZsq*MZsq*MZsq - 1.6333012551485228e-9*MYsq*MZsq*MZsq*MZsq + 2.2984304071727213e-11*MZsq*MZsq*MZsq*MZsq + MNsq*MNsq*(14.914925802222756 + 5.502651450317014e-7*MXsq*MXsq + 5.755832754853654e-7*MYsq*MYsq + MYsq*(0.004398777365090501 - 6.489982158966259e-8*MZsq) + MXsq*(-0.0006506360057573337 - 8.344935262578346e-7*MYsq + 2.058589516409181e-8*MZsq) - 0.0003835746794826118*MZsq + 2.535942992820496e-9*MZsq*MZsq) + MXsq*MXsq*(118.03012040755969 + 8.113847178902898e-6*MYsq*MYsq + MYsq*(0.06007199869155439 - 8.959996008448914e-7*MZsq) - 0.003458430490381368*MZsq + 2.5471723354865756e-8*MZsq*MZsq) + MXsq*(-341886.1927778221 - 7.237552158075443e-6*MYsq*MYsq*MYsq + MYsq*MYsq*(-0.08029576474230003 + 1.1978777085675496e-6*MZsq) + 15.61251731042926*MZsq - 0.0002362289932202581*MZsq*MZsq + 1.1864768102085224e-9*MZsq*MZsq*MZsq + MYsq*(-296.7585912558013 + 0.008853614549420123*MZsq - 6.60476532363592e-8*MZsq*MZsq)) + MNsq*(-124123.09397815229 + 6.375101179397463e-7*MXsq*MXsq*MXsq - 1.9188875195557583e-6*MYsq*MYsq*MYsq + MXsq*MXsq*(-0.01441389684267835 - 3.2167604271088467e-6*MYsq + 2.0369978322620455e-7*MZsq) + MYsq*MYsq*(-0.021385269534081264 + 3.184115998435637e-7*MZsq) + 5.210029029604001*MZsq - 0.00007402331201618714*MZsq*MZsq + 3.548872286530906e-10*MZsq*MZsq*MZsq + MYsq*(-80.12518551281774 + 0.0023794799403980557*MZsq - 1.7681323767602378e-8*MZsq*MZsq) + MXsq*(45.08418865792368 + 4.28708440785298e-6*MYsq*MYsq + MYsq*(0.03159334543643911 - 4.7166090897259715e-7*MZsq) - 0.0014642695057850929*MZsq + 1.1544776096002105e-8*MZsq*MZsq));
}

vec3 normal(float x, float y, float t) {
    float delta = 0.01;
    float oneOverTwoDelta = 50.0;
    float dh_dx = (height(x-delta,y,t) - height(x+delta, y, t))*oneOverTwoDelta;
    float dh_dy = (height(x,y-delta,t) - height(x, y+delta, t))*oneOverTwoDelta;
//    float dh_dx = (height(x-delta,y,t) - 2.0*height(x+delta, y, t))*oneOverTwoDelta;
//    float dh_dy = (height(x,y-delta,t) - 0.1*height(x, y+delta, t))*oneOverTwoDelta;
    lowp vec3 t1 = normalize(vec3(1.0, 0.0, dh_dx));
    lowp vec3 t2 = normalize(vec3(0.0, 1.0, dh_dy));
    return cross(t1,t2);
}


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
