#version 400

uniform mat4 promatrix;

layout(points) in;
in vec3 pass_rig	[];
in vec3 pass_up		[];
in vec4 pass_col	[];
in vec2 pass_txc1	[];
in vec2 pass_txc2	[];

layout(triangle_strip, max_vertices = 4) out;
out vec4 pass_col_2;
out vec2 pass_txc;

void main() {
	pass_col_2 = pass_col[0];
	
	pass_txc = pass_txc1[0];
	gl_Position = promatrix * vec4(vec3(gl_in[0].gl_Position) + pass_rig[0] + pass_up[0], 1.0);
	EmitVertex();
	
	pass_txc = vec2(pass_txc1[0].x, pass_txc2[0].y);
	gl_Position = promatrix * vec4(vec3(gl_in[0].gl_Position) + pass_rig[0] - pass_up[0], 1.0);
	EmitVertex();
	
	pass_txc = vec2(pass_txc2[0].x, pass_txc1[0].y);
	gl_Position = promatrix * vec4(vec3(gl_in[0].gl_Position) - pass_rig[0] + pass_up[0], 1.0);
	EmitVertex();
	
	pass_txc = pass_txc2[0];
	gl_Position = promatrix * vec4(vec3(gl_in[0].gl_Position) - pass_rig[0] - pass_up[0], 1.0);
	EmitVertex();
	
	EndPrimitive();
}