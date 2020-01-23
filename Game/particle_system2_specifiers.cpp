#include "particle_system2_specifiers.h"
#include "game_engine.h"
#include "basic_collider.h"
#include "i_place_collision_evaluator.h"


TestBulletImpactSparksSpecifier::TestBulletImpactSparksSpecifier() :
	mColorTransition(),
	mMaxParticles(10) {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 1.0, 0.7), 0.3 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.5, 0.0), 0.0 });
}

void TestBulletImpactSparksSpecifier::init(ParticleSystem2& i_system, Accessor& i_accessor) {
	uint index = i_system.add(mMaxParticles);
	i_system.access(index, i_accessor);
	for (uint i = 0; i < mMaxParticles; i++) {
		i_accessor.mAge[i] = 0.0f;
		i_accessor.mAngle[i] = 0.0f;
		i_accessor.mAngularVelocity[i] = 0.0f;
		i_accessor.mColor[i] = ColorRGBAf();
		i_accessor.mDimensions[i] = Vector2f(1.0, 1.0);
		i_accessor.mLinearVelocity[i] = Vector3f(2.0 * Vector2f(sqrt(geutil::random<double>(1)), 0)
			.rotated(geutil::random<double>(2 * PI)), 0.0);
		i_accessor.mPosition[i] = Vector3f(0, 0, 0.5);
		i_accessor.mUV1[i] = Vector2f(0, 0);
		i_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(i_accessor, mMaxParticles, i_system.getTransform(), i_system.getDepthTransform());
}

void TestBulletImpactSparksSpecifier::update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount(); i++) {
		float v = geutil::random<float>(-0.5, 0.5);
		i_accessor.mAge[i] += (float)i_dt;
		i_accessor.mPosition[i] += i_accessor.mLinearVelocity[i] * i_dt;
		i_accessor.mColor[i] = mColorTransition.evaluate(i_accessor.mAge[i]) + ColorRGBAf(v, v, v, 0);
	}
}

void TestBulletImpactSparksSpecifier::destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount();) {
		if (i_accessor.mAge[i] > 0.3f) {
			i_system.remove(i);
		}
		else {
			i++;
		}
	}

	if (i_system.getCount() == 0) {
		GE.destruction().add(&i_system);
	}
}

void TestBulletImpactSparksSpecifier::generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {}

