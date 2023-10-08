#version 330 core
layout (location = 0) in vec2 aPos;

out vec2 TexCoords;

void main()
{
    TexCoords = vec2((aPos.x + 1) / 2 , (aPos.y + 1) / 2);
    gl_Position = vec4(aPos, 0.0, 1.0);
}