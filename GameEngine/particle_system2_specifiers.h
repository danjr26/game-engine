#ifndef PARTICLE_SYSTEM2_SPECIFIERS_H
#define PARTICLE_SYSTEM2_SPECIFIERS_H

#include "particle_system2.h"
#include "stepper.h"
#include "cubic_transitioner.h"

class TestParticleSpecifier : public ParticleSystem2::Specifier {
	using Accessor = ParticleSystem2::Accessor;
	CubicTransitioner<ColorRGBAf> mColorTransition;
	Stepper mGenerateStepper;
	uint mNGenerated;

public:
	TestParticleSpecifier();

	void init(ParticleSystem2& in_system, Accessor& in_accessor) override;
	void update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
};

class AmbientDustSpecifier : public ParticleSystem2::Specifier {
	using Accessor = ParticleSystem2::Accessor;
	CubicTransitioner<ColorRGBAf> mColorTransition;
	uint mMaxParticles;

public:
	AmbientDustSpecifier();

	void init(ParticleSystem2& in_system, Accessor& in_accessor) override;
	void update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
};

class TestBulletImpactSparksSpecifier : public ParticleSystem2::Specifier {
	using Accessor = ParticleSystem2::Accessor;
	CubicTransitioner<ColorRGBAf> mColorTransition;
	uint mMaxParticles;

public:
	TestBulletImpactSparksSpecifier();

	void init(ParticleSystem2& in_system, Accessor& in_accessor) override;
	void update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	void generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
};

#endif