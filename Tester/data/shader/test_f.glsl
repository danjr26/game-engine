#version 430

layout (std430, binding=0) readonly restrict buffer bData {
	vec4 color;
};

out vec4 oColor;

void main() {
	oColor = bData.color;
}