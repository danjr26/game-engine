#include "particle_system2_specifiers.h"

TestParticleSpecifier::TestParticleSpecifier() :
	generateStepper(0.0005),
	nGenerated(0),
	colorTransition() {
	
	colorTransition.keys.push_back({ ColorRGBAf(0.1, 0.2, 1.0, 1.0), 0.5 });
	colorTransition.keys.push_back({ ColorRGBAf(0.7, 0.5, 0.1, 0.5), 0.5 });
	colorTransition.keys.push_back({ ColorRGBAf(0.7, 0.5, 0.1, 0.0), 1.0 });
}

void TestParticleSpecifier::Update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	//in_system.Reserve(6000);
	in_system.Access(0, in_accessor);
	for (uint i = 0; i < in_system.Count(); i++) {
		in_accessor.age[i] += (float)in_dt;
		in_accessor.position[i] += in_accessor.linearVelocity[i] * in_dt;
		in_accessor.position[i] -= Vector3f(300, 300, 0);
		in_accessor.position[i].Rotate(Vector3f(0, 0, 1), 0.5f * PI * in_dt);
		in_accessor.position[i] += Vector3f(300, 300, 0);
		in_accessor.color[i] = colorTransition.Evaluate(in_accessor.age[i]);
	}
}

void TestParticleSpecifier::Destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.Access(0, in_accessor);
	for (uint i = 0; i < in_system.Count();) {
		if (in_accessor.age[i] > 1.0f) {
			in_system.Remove(i);
		}
		else {
			i++;
		}
	}
}

void TestParticleSpecifier::Generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	if (in_system.Count() > 2000) return;
	uint nToGenerate = generateStepper.Step_Number(in_dt);
	uint index = in_system.Add(nToGenerate);
	in_system.Access(index, in_accessor);
	nGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		in_accessor.age[i] = 0.0f;
		in_accessor.angle[i] = 0.0f;
		in_accessor.angularVelocity[i] = 0.0f;
		in_accessor.color[i] = ColorRGBAf(0.8, 0.1, 0.1, 1.0);
		in_accessor.dimensions[i] = Vector2f(20, 20);
		in_accessor.linearVelocity[i] = Vector3f(200.0 * Vector2f(sqrt(Random<double>(1)), 0).Rotated(Random<double>(2 * PI)));
		in_accessor.position[i] = Vector3f(Vector2f(300, 300) + 50.0 * Vector2f(sqrt(Random<double>(1)), 0).Rotated(Random<double>(2 * PI)));
		in_accessor.uv1[i] = Vector2f(0, 0);
		in_accessor.uv2[i] = Vector2f(1, 1);
	}
}
