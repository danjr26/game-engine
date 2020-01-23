#ifndef PARTICLE_SYSTEM2_SPECIFIERS_H
#define PARTICLE_SYSTEM2_SPECIFIERS_H

#include "particle_system2.h"
#include "stepper.h"
#include "cubic_transitioner.h"
#include "linear_transitioner.h"


class TestBulletImpactSparksSpecifier : public ParticleSystem2::Specifier {
	using Accessor = ParticleSystem2::Accessor;
	CubicTransitioner<ColorRGBAf, float> mColorTransition;
	uint mMaxParticles;

public:
	TestBulletImpactSparksSpecifier();

	void init(ParticleSystem2& i_system, Accessor& i_accessor) override;
	void update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) override;
	void destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) override;
	void generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) override;
};


#endif