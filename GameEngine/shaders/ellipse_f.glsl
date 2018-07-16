#version 400

uniform vec2 pixelDimensions;
uniform vec4 fillColor;
uniform vec4 outlineColor;
uniform float outlineWidth;

in vec2 uv_f;

layout (location = 0) out vec4 color_o;

void main() {
	const float epsilon = 1.0;
	
	vec2 fromCenter = (uv_f - vec2(0.5, 0.5)) * pixelDimensions;
	float distanceFromCenter = length(fromCenter);
	float outerRadius = pixelDimensions.x / 2.0;
	if (distanceFromCenter > outerRadius) discard;
	float innerRadius = (pixelDimensions.x) / 2.0 - outlineWidth; 
	float outlineFactor = min(1.0, outlineWidth / epsilon);
	
	vec4 innerMixed = mix(fillColor, outlineColor, 
		vec4(smoothstep(innerRadius - epsilon, innerRadius + epsilon, distanceFromCenter)));
		
	vec4 outerMixed = vec4(outlineColor.xyz, outlineColor.a * outlineFactor * 
			(1.0 - smoothstep(outerRadius - epsilon, outerRadius, distanceFromCenter)));
	
	if (distanceFromCenter < innerRadius - epsilon) {
		color_o = fillColor;
	}
	else if (distanceFromCenter < innerRadius + epsilon) {
		if(distanceFromCenter > outerRadius - epsilon) {
			color_o = mix(innerMixed, outerMixed, 0.5);
		}
		else {
			color_o = innerMixed;
		}
	}
	else if (distanceFromCenter > outerRadius - epsilon) {
		color_o = outerMixed;
	} else {
		color_o = outlineColor;
	}
}