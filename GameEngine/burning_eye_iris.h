#ifndef BURNING_EYE_IRIS_SPECIFIER_H
#define BURNING_EYE_IRIS_SPECIFIER_H

#include "particle_system2.h"
#include "stepper.h"
#include "cubic_transitioner.h"
#include "linear_transitioner.h"

namespace burning_eye {
	class IrisSpecifier : public ParticleSystem2::Specifier {
		using Accessor = ParticleSystem2::Accessor;
		LinearTransitioner<ColorRGBAf, float> mColorTransition;
		Stepper mGenerateStepper;
		uint mNGenerated;

	public:
		IrisSpecifier();

		void init(ParticleSystem2& in_system, Accessor& in_accessor) override;
		void update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
		void destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
		void generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) override;
	};
};

#endif