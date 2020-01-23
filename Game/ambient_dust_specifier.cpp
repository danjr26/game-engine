#include "ambient_dust_specifier.h"
#include "game_engine.h"
#include "game.h"

AmbientDustSpecifier::AmbientDustSpecifier() :
	mColorTransition(),
	mMaxParticles(100) {

	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.6, 1.0), 0.0 });
}

void AmbientDustSpecifier::init(ParticleSystem2& i_system, Accessor& i_accessor) {
	Rectangled camRect = GE.cameras().get(CameraManager::ID::main)->getTransformedRectangle();

	i_system.access(0, i_accessor);
	uint index = i_system.add(mMaxParticles);
	i_system.access(index, i_accessor);
	for (uint i = 0; i < mMaxParticles; i++) {
		i_accessor.mAge[i] = 0.0f;
		i_accessor.mAngle[i] = 0.0f;
		i_accessor.mAngularVelocity[i] = 0.0f;
		i_accessor.mColor[i] = ColorRGBAf(0.5, 0.5, 0.6, 1.0);
		i_accessor.mDimensions[i] = Vector2f(0.3, 0.3);
		i_accessor.mLinearVelocity[i] = Vector3f(1.0 * Vector2f(sqrt(geutil::random<double>(1)), 0).rotated(geutil::random<double>(2 * PI)));
		i_accessor.mPosition[i] = Vector3f(Vector2f(camRect.randomPointInside()), 0);
		i_accessor.mUV1[i] = Vector2f(0, 0);
		i_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(i_accessor, mMaxParticles, i_system.getTransform(), i_system.getDepthTransform());
}

void AmbientDustSpecifier::update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	Rectangled camRect = GE.cameras().get(CameraManager::ID::main)->getTransformedRectangle();
	Vector2d axes[2];
	camRect.getAxes(axes);
	double dots[2] = {
		axes[0].dotSelf(),
		axes[1].dotSelf()
	};

	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount(); i++) {
		i_accessor.mAge[i] += (float)i_dt;
		i_accessor.mPosition[i] += i_accessor.mLinearVelocity[i] * i_dt;
		i_accessor.mColor[i] = mColorTransition.evaluate(i_accessor.mAge[i]);

		Vector2d fromOrigin = Vector2f(i_accessor.mPosition[i]) - camRect.getOrigin();
		for (uint j = 0; j < 2; j++) {
			double dot = fromOrigin.dot(axes[j]);
			if (dot < 0) {
				i_accessor.mPosition[i] += Vector3d(axes[j], 0);
			}
			else if (dot > dots[j]) {
				i_accessor.mPosition[i] -= Vector3d(axes[j], 0);
			}
		}
	}
	ParticleSystem2::transformParticles(i_accessor, i_system.getCount(), i_system.getTransform(), i_system.getDepthTransform());
}

void AmbientDustSpecifier::destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {}

void AmbientDustSpecifier::generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {}
