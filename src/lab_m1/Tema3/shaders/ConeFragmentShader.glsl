#version 330

// Uniform
uniform vec3 color;

// Output
layout(location = 0) out vec4 out_color;

void main() {
    out_color = vec4(color, 0.5f);
}
