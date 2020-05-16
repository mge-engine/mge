#version 330 core

layout(location = 0) in vec3 vertex_position
layout(location = 1) in vec2 vertex_uv

out vec2 uv;

uniform mat4 model_view_projection;

void main()
{
    gl_Position = model_view_projection * vec4(vertex_position, 1);
    uv = vertex_uv;
}