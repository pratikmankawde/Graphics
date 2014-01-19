#version 120

uniform samplerCube CubeMap;

varying vec3 Position;

void main()
{
    gl_FragColor = textureCube(CubeMap, Position);
}