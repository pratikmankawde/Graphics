uniform samplerCube EnvMap;

void main (void)
{
  // Get reflected environment map color
  vec3 envCoord = vec3 (gl_TexCoord[0]);
  vec3 envColor = vec3 (textureCube (EnvMap, envCoord));

  gl_FragColor = vec4 (envColor, 1.0);
}