#version 400

uniform sampler2DArray colorTextures;

in vec3 uv_f;

out vec4 color_o;

void main() {
/*
	if(uv_f.x < 0.0 || uv_f.y < 0.0 || uv_f.x > 1.0 || uv_f.y > 1.0)
		color_o = vec4(0.5);
	else
		color_o = texture(colorTextures, uv_f);
	
	*/
	color_o = vec4(1, 1, 1, 1);
}