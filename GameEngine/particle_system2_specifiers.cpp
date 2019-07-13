#include "particle_system2_specifiers.h"
#include "game_engine.h"
#include "basic_collision_mask.h"
#include "in_place_collision_evaluator.h"


TestBulletImpactSparksSpecifier::TestBulletImpactSparksSpecifier() :
	mColorTransition(),
	mMaxParticles(10) {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 1.0, 0.7), 0.3 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.5, 0.0), 0.0 });
}

void TestBulletImpactSparksSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	uint index = in_system.add(mMaxParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < mMaxParticles; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mAngle[i] = 0.0f;
		in_accessor.mAngularVelocity[i] = 0.0f;
		in_accessor.mColor[i] = ColorRGBAf();
		in_accessor.mDimensions[i] = Vector2f(1.0, 1.0);
		in_accessor.mLinearVelocity[i] = Vector3f(2.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0)
			.rotated(GEUtil::random<double>(2 * PI)), 0.0);
		in_accessor.mPosition[i] = Vector3f(0, 0, 0.5);
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, mMaxParticles, in_system.getTransform(), in_system.getDepthTransform());
}

void TestBulletImpactSparksSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		float v = GEUtil::random<float>(-0.5, 0.5);
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * in_dt;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]) + ColorRGBAf(v, v, v, 0);
	}
}

void TestBulletImpactSparksSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 0.3f) {
			in_system.remove(i);
		}
		else {
			i++;
		}
	}

	if (in_system.getCount() == 0) {
		GE.destruction().add(&in_system);
	}
}

void TestBulletImpactSparksSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}

