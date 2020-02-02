#version 430

layout (location = 1) in vec3 iPosition;

void main() {
	gl_Position = vec4(iPosition, 1.0);
}