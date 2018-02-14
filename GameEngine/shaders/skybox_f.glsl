#version 400

uniform samplerCube tex;

in vec3 pass_pos;

out vec4 out_col;

void main() {
	out_col = texture(tex, normalize(pass_pos));
	//float lum = (out_col.x + out_col.y + out_col.z) / 3;
	//out_col = mix(out_col, vec4(lum, lum, lum, 1.0), 0.2);
	//out_col = vec4((pass_pos + 1.0) * 0.5, 1.0);
	
}