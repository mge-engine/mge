#version 330 core

in vec2 uv;
out vec3 color;

uniform sampler2D cube_texture;

void main()
{
    color = texture( cube_texture, uv ).rgb;
}