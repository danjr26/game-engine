#ifndef PARTICLE_SYSTEM2_SPECIFIERS_H
#define PARTICLE_SYSTEM2_SPECIFIERS_H

#include "particle_system2.h"
#include "stepper.h"
#include "cubic_transitioner.h"

class TestParticleSpecifier : public ParticleSystem2::Specifier {
	using Accessor = ParticleSystem2::Accessor;
	CubicTransitioner<ColorRGBAf> colorTransition;
	Stepper generateStepper;
	uint nGenerated;

public:
	TestParticleSpecifier();

	void Update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void Destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void Generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
};

#endif