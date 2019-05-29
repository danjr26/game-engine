#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 position_v;
layout (location = 1) in vec3 normal_v;
layout (location = 2) in vec4 color_v;
layout (location = 3) in vec2 uv_v;

out vec3 position_f;
out vec3 normal_f;
out vec4 color_f;
out vec2 uv_f;

void main() {
	mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
	gl_Position = mvpMatrix * vec4(position_v, 1.0);
	
	position_f = (modelMatrix * vec4(position_v, 1.0)).xyz;
	normal_f = (modelMatrix * vec4(normal_v, 0.0)).xyz; 
	color_f = color_v;
	uv_f = uv_v;
}

