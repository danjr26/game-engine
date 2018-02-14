#version 400

struct Light {
	vec4 amb;
	vec4 dif;
	vec4 spe;
	vec4 pos;
	int  on;
};

struct Material {
	vec4 amb;
	vec4 dif;
	vec4 spe;
	float shi;
	int lig;
};

uniform sampler2D 	tex;
uniform int			texbnd;
uniform Light 		ligs[8];
uniform int			nligs;
uniform Material 	mat;

in vec3 pass_pos;
in vec3 pass_nor;
in vec2 pass_txc;

out vec4 out_col;

void main() {
	out_col = vec4(0.0, 0.0, 0.0, 1.0);
	
	if(mat.lig == 1) {
		vec3 nor = normalize(pass_nor);
		for(int i=0; i<nligs; i++) {
			if(ligs[i].on != 1)
				continue;
			float d;
			float s;
			float atten;
			vec3 tolightn;
			if(ligs[i].pos.w == 0.0) {
				tolightn = -ligs[i].pos.xyz;
				atten = 1.0;
			}
			if(ligs[i].pos.w == 1.0) {
				vec3 tolight = ligs[i].pos.xyz - pass_pos;
				tolightn = normalize(tolight);
				float tolightd2 = dot(tolight, tolight);
				atten = 1.0 / (1.0 + 0.001 * tolightd2);
			}
			
			
			vec3 tocam = normalize(-pass_pos);
			vec3 reflection = reflect(-tolightn, nor);
			
			d = dot(nor, tolightn);
			s = dot(nor, reflection);
			if(s > 0.0)
				s = pow(s, 2.0);
			else 
				s = 0.0;
			
			vec4 amb = clamp(ligs[i].amb * mat.amb, 0.0, 1.0);
			vec4 dif = clamp(ligs[i].dif * mat.dif * d, 0.0, 1.0);
			vec4 spe = clamp(ligs[i].spe * mat.spe * s, 0.0, 1.0);
			
			out_col += (amb + dif + spe) * atten;
		}
	}
	else {
		out_col = mat.dif;
	}
	
	if(texbnd != 0)
		out_col *= texture(tex, pass_txc);
				
	float dist = 1.0 / (1.0 - gl_FragCoord.z);
	
	out_col *= mix(vec4(1.0), vec4(0.5, 0.5, 1.0, 1.0), clamp(dist * dist * (1.0 / (250.0 * 250.0)), 0.0, 1.0));
	
	float lum = (out_col.x + out_col.y + out_col.z) / 3;
	
	out_col = mix(out_col, vec4(lum, lum, lum, 1.0), clamp(dist * dist * (1.0 / (250.0 * 250.0)), 0.0, 1.0));
}