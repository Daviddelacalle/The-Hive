#version 400

layout(location = 0) in vec2 position;

void main() {
    gl_Position = vec4(position,-1, 1.0);
};
