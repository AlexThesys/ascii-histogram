// geometry shader
#version 440 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

const float width = 1.0f / 256.0f;

void main()
{

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position 
                  + vec4(width, 0.0f, 0.0f, 0.0f); 
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position 
                  + vec4(width, 0.0f, 0.0f, 0.0f); 
    EmitVertex();

    EndPrimitive();
}
