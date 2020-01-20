// vertex shader
#version 440 core

layout (location=0) in uint posY;

uniform int size;

const float dim = 256.0f;

void main()
{
    const float ypos = float(posY) * 8.0f / float(size);
    const float xpos = (float(gl_VertexID) / dim) * 2.0f - 1.0f;
    gl_Position = vec4(xpos, ypos, 0.0f, 1.0f);
}
