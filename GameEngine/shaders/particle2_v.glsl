#version 400

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

layout(location = 0) in vec3 position_v;
layout(location = 2) in vec4 color_v;
layout(location = 3) in vec2 uv1_v;
layout(location = 4) in vec2 uv2_v;
layout(location = 5) in vec2 dimensions_v;
layout(location = 6) in float angle_v;

out vec4 color_g;
out vec2 uv1_g;
out vec2 uv2_g;
out vec2 dimensions_g;
out float angle_g;

void main() {
	mat4 mvMatrix = viewMatrix * modelMatrix;
	gl_Position = mvMatrix * vec4(position_v, 1.0);
	uv1_g = uv1_v;
	uv2_g = uv2_v;
	color_g =  color_v;
	dimensions_g = dimensions_v;
	angle_g = angle_v;
}
