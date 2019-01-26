#include "particle_system2_specifiers.h"

TestParticleSpecifier::TestParticleSpecifier() :
	generateStepper(0.001),
	nGenerated(0)
{}

void TestParticleSpecifier::Update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}

void TestParticleSpecifier::Destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}

void TestParticleSpecifier::Generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	if (nGenerated > 5000) return;
	uint nToGenerate = generateStepper.Step_Number(in_dt);
	uint index = in_system.Add(nToGenerate);
	in_system.Access(index, in_accessor);
	nGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		in_accessor.age[i] = 0.0f;
		in_accessor.angle[i] = 0.0f;
		in_accessor.angularVelocity[i] = 0.0f;
		in_accessor.color[i] = ColorRGBAf(1.0, 1.0, 1.0, 0.1);
		in_accessor.dimensions[i] = Vector2f(20, 20);
		in_accessor.linearVelocity[i] = Vector3f(0, 0, 0);
		in_accessor.position[i] = Vector3f(Vector2f(200, 200) + Vector2f(Random<double>(100), 0).Rotated(Random<double>(2 * PI)));
		in_accessor.uv1[i] = Vector2f(0, 0);
		in_accessor.uv2[i] = Vector2f(1, 1);
	}
}
