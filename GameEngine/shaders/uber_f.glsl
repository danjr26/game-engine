#version 400



#ifdef TEXTURE2_AMBIENT
	uniform sampler2D ambientTexture_f;
#endif

#ifdef TEXTURE2_DIFFUSE
	uniform sampler2D diffuseTexture_f;
#endif

#ifdef TEXTURE2_SPECULAR
	uniform sampler2D specularTexture_f;
#endif

#ifdef TEXTURE2_EMISSIVE
	uniform sampler2D emissiveTexture_f;
#endif

#ifdef COLOR_AMBIENT
	uniform vec3 ambientColor_f;
#endif

#ifdef COLOR_DIFFUSE
	uniform vec4 diffuseColor_f;
#endif

#ifdef COLOR_SPECULAR
	uniform vec4 specularColor_f;
#endif

#ifdef PASS_POSITION_TO_FRAG
	in vec3 position_f;
#endif 

#ifdef PASS_NORMAL_TO_FRAG
	in vec3 normal_f;
#endif

#ifdef PASS_COLOR_TO_FRAG
	#ifdef COLOR3_ATTR
		in vec3 color_f;
	#endif
	#ifdef COLOR4_ATTR
		in vec4 color_f;
	#endif
#endif 

#ifdef PASS_UV_TO_FRAG
	#ifdef UV2_ATTR
		in vec2 uv_f;
	#endif
	#ifdef UV3_ATTR
		in vec3 uv_f;
	#endif
#endif

#ifdef OUTPUT_COMPOSITE
layout out vec4 color_o;
layout out vec4 normal_o;

void main() {
	color_o = texture(colorTexture, uv_f);
	normal_o = vec4(normal_f, 1.0);
}