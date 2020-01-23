#ifndef NEEDLER_IRIS_SPECIFIER_H
#define NEEDLER_IRIS_SPECIFIER_H

#include "particle_system2.h"
#include "stepper.h"
#include "cubic_transitioner.h"
#include "linear_transitioner.h"

namespace needler {
	class IrisSpecifier : public ParticleSystem2::Specifier {
		using Accessor = ParticleSystem2::Accessor;
		CubicTransitioner<ColorRGBAf, float> mColorTransition;
		Stepper mGenerateStepper;
		uint mNGenerated;

	public:
		IrisSpecifier();

		void init(ParticleSystem2& i_system, Accessor& i_accessor) override;
		void update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) override;
		void destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) override;
		void generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) override;
	};
}

#endif