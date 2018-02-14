#version 400

#define ATTRIB_LOCATION_POSITION 	0
#define ATTRIB_LOCATION_NORMAL 		1
#define ATTRIB_LOCATION_UV 			3

uniform mat4 modmatrix;
uniform mat4 viematrix;
uniform mat4 promatrix;

layout(location = ATTRIB_LOCATION_POSITION) in vec3 in_position;
layout(location = ATTRIB_LOCATION_NORMAL) 	in vec3 in_normal;
layout(location = ATTRIB_LOCATION_UV) 		in vec2 in_uv;

out vec3 pass_position;
out vec3 pass_normal;
out vec2 pass_uv;

void main() {
	mat4 mvpmatrix = promatrix * viematrix * modmatrix;
	gl_Position = mvpmatrix * vec4(in_position, 1.0);
	
	pass_position = (modmatrix * vec4(in_position, 1.0)).xyz;
	pass_normal = (modmatrix * vec4(in_normal, 0.0)).xyz; 
	pass_uv = in_uv;
}