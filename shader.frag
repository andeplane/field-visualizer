uniform vec3 cameraPosition;
varying vec3 norm;
varying float light;
void main() {
  vec3 lightPos = vec3(0.0, 0.0, cameraPosition.z);
  vec4 val = vec4(28.0/255.0,144.0/255.0,153.0/255.0,1.0);
  float lightValue = 0.2 + light*clamp(dot(normalize(lightPos), normalize(norm)), 0.0, 1.0);
  gl_FragColor = vec4(val.xyz*lightValue, 1.0);
}
