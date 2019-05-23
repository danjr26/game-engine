#version 400

in vec4 color_f;

layout (location = 0) out vec4 color_o;

void main() {
	color_o = color_f;
}