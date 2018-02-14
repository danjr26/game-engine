#version 400

uniform mat4 promatrix;

layout(lines) in;
in vec4 pass_col[];
in vec2 pass_txc[];
in float pass_wid[];

layout(triangle_strip, max_vertices = 8) out;
out vec4 pass_col_2;
out vec2 pass_txc_2;

void main() {
	vec3 v1 = gl_in[0].gl_Position.xyz;
	vec3 v2 = gl_in[1].gl_Position.xyz;

	vec3 dir = normalize(v2 - v1);
	
	vec3 tocam1 = normalize(-v1); 
	vec3 up1 = normalize(cross(tocam1, dir));
	vec3 rig1 = cross(tocam1, up1);
	
	vec3 tocam2 = normalize(-v2); 
	vec3 up2 = normalize(cross(tocam2, dir));
	vec3 rig2 = -cross(tocam2, up2);
	
	float wid = pass_wid[0] * 0.5;
	
	up1 *= wid;
	up2 *= wid;
	rig1 *= wid;
	rig2 *= wid;
	
	pass_col_2 = pass_col[0];
	
	pass_txc_2 = pass_txc[0];
	gl_Position = promatrix * vec4(v1 + up1 + rig1, 1.0);
	EmitVertex();
	
	pass_txc_2 = vec2(pass_txc[0].x, pass_txc[1].y);
	gl_Position = promatrix * vec4(v1 - up1 + rig1, 1.0);
	EmitVertex();
	
	pass_txc_2 = vec2((pass_txc[0].x + pass_txc[1].x) * 0.5, pass_txc[0].y);
	gl_Position = promatrix * vec4(v1 + up1, 1.0);
	EmitVertex();
	
	pass_txc_2 = vec2((pass_txc[0].x + pass_txc[1].x) * 0.5, pass_txc[1].y);
	gl_Position = promatrix * vec4(v1 - up1, 1.0);
	EmitVertex();
	
	pass_col_2 = pass_col[1];
	
	pass_txc_2 = vec2((pass_txc[0].x + pass_txc[1].x) * 0.5, pass_txc[0].y);
	gl_Position = promatrix * vec4(v2 + up2, 1.0);
	EmitVertex();
	
	pass_txc_2 = vec2((pass_txc[0].x + pass_txc[1].x) * 0.5, pass_txc[1].y);
	gl_Position = promatrix * vec4(v2 - up2, 1.0);
	EmitVertex();
	
	pass_txc_2 = vec2(pass_txc[1].x, pass_txc[0].y);
	gl_Position = promatrix * vec4(v2 + up2 + rig2, 1.0);
	EmitVertex();
	
	pass_txc_2 = pass_txc[1];
	gl_Position = promatrix * vec4(v2 - up2 + rig2, 1.0);
	EmitVertex();
}