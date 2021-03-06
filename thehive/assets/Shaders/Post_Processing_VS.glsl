#version 450 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 vertexUV;

out vec2 UV;

void main() {
    UV = vertexUV;
    gl_Position = vec4(vertexPosition, -0.993, 1.0);
}
