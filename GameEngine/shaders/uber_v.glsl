#version 400

#ifdef MVP_SEPARATE_MATRICES
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
#else
uniform mat4 mvpMatrix;
#endif

layout (location = 0) in vec3 position_v;

#ifdef NORMAL_ATTR
layout (location = 1) in vec3 normal_v;
#endif

#ifdef COLOR3_ATTR
layout (location = 2) in vec3 color_v;
#endif

#ifdef COLOR4_ATTR
layout (location = 2) in vec4 color_v;
#endif

#ifdef UV2_ATTR
layout (location = 3) in vec2 uv_v;
#endif

#ifdef UV3_ATTR
layout (location = 3) in vec3 uv_v;
#endif

#ifdef PASS_POSITION_TO_FRAG
out vec3 position_f;
#endif 

#ifdef PASS_NORMAL_TO_FRAG
out vec3 normal_f;
#endif 

#ifdef PASS_COLOR_TO_FRAG
	#ifdef COLOR3_ATTR
		out vec3 color_f;
	#endif
	#ifdef COLOR4_ATTR
		out vec4 color_f;
	#endif
#endif 

#ifdef PASS_UV_TO_FRAG
	#ifdef UV2_ATTR
		out vec2 uv_f;
	#endif
	#ifdef UV3_ATTR
		out vec3 uv_f;
	#endif
#endif

void main() {
	#ifdef MVP_SEPARATE_MATRICES
	mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
	#endif
	
	gl_Position = mvpMatrix * vec4(position_v, 1.0);
	
	#ifdef PASS_POSITION_TO_FRAG
	position_f = (modelMatrix * vec4(position_v, 1.0)).xyz;
	#endif
	
	#ifdef PASS_NORMAL_TO_FRAG
	normal_f = (modelMatrix * vec4(normal_v, 0.0)).xyz; 
	#endif 
	
	#ifdef PASS_COLOR_TO_FRAG
	color_f = color_v;
	#endif
	
	#ifdef PASS_UV_TO_FRAG
	uv_f = uv_v;
	#endif
}

