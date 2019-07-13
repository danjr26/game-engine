#ifndef AMBIENT_DUST_SPECIFIER_H
#define AMBIENT_DUST_SPECIFIER_H

#include "particle_system2.h"
#include "stepper.h"
#include "cubic_transitioner.h"
#include "linear_transitioner.h"

class AmbientDustSpecifier : public ParticleSystem2::Specifier {
	using Accessor = ParticleSystem2::Accessor;
	CubicTransitioner<ColorRGBAf, float> mColorTransition;
	uint mMaxParticles;

public:
	AmbientDustSpecifier();

	void init(ParticleSystem2& in_system, Accessor& in_accessor) override;
	void update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
};

#endif