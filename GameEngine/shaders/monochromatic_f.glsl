#version 400

uniform vec4 colorTint;

layout (location = 0) out vec4 color_o;

void main() {
	color_o = colorTint;
}