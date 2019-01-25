#version 400
#define PI 3.1415926536

uniform mat4 projectionMatrix;

layout(points) in;
in vec4 color_g[];
in vec2 uv1_g[];
in vec2 uv2_g[];
in vec2 dimensions_g[];
in float angle_g[];

layout(triangle_strip, max_vertices = 4) out;
out vec4 color_f;
out vec2 uv_f;

void main() {
	float s = sin(angle_g[0]);
	float c = cos(angle_g[0]);

	mat2 rotationMatrix =
		mat2(
			c, -s,
			s, c
		);

	vec3 position3 = vec3(gl_in[0].gl_Position);

	color_f = color_g[0];

	uv_f = vec2(uv2_g[0].x, uv2_g[0].y);
	gl_Position = projectionMatrix * vec4(position3 + vec3(rotationMatrix * vec2(dimensions_g[0].x, dimensions_g[0].y), 0.0), 1.0);
	//gl_Position = vec4(-100.0, -100.0, 0.0, 1.0);
	EmitVertex();

	uv_f = vec2(uv2_g[0].x, uv1_g[0].y);
	gl_Position = projectionMatrix * vec4(position3 + vec3(rotationMatrix * vec2(dimensions_g[0].x, -dimensions_g[0].y), 0.0), 1.0);
	//gl_Position = vec4(-100.0, 100.0, 0.0, 1.0);
	EmitVertex();

	uv_f = vec2(uv1_g[0].x, uv2_g[0].y);
	gl_Position = projectionMatrix * vec4(position3 + vec3(rotationMatrix * vec2(-dimensions_g[0].x, dimensions_g[0].y), 0.0), 1.0);
	//gl_Position = vec4(100.0, -100.0, 0.0, 1.0);
	EmitVertex();

	uv_f = vec2(uv1_g[0].x, uv1_g[0].y);
	gl_Position = projectionMatrix * vec4(position3 + vec3(rotationMatrix * vec2(-dimensions_g[0].x, -dimensions_g[0].y), 0.0), 1.0);
	//gl_Position = vec4(100.0, 100.0, 0.0, 1.0);
	EmitVertex();

	EndPrimitive();
}
