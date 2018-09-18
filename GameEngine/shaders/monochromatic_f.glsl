#version 400

uniform vec4 color;

layout (location = 0) out vec4 color_o;

void main() {
	color_o = color;
}