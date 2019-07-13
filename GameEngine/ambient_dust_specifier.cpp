#include "ambient_dust_specifier.h"
#include "game_engine.h"
#include "game.h"

AmbientDustSpecifier::AmbientDustSpecifier() :
	mColorTransition(),
	mMaxParticles(100) {

	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.6, 1.0), 0.0 });
}

void AmbientDustSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	Rectangled camRect = GE.cameras().get(CameraManager::ID::main)->getTransformedRectangle();

	in_system.access(0, in_accessor);
	uint index = in_system.add(mMaxParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < mMaxParticles; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mAngle[i] = 0.0f;
		in_accessor.mAngularVelocity[i] = 0.0f;
		in_accessor.mColor[i] = ColorRGBAf(0.5, 0.5, 0.6, 1.0);
		in_accessor.mDimensions[i] = Vector2f(0.3, 0.3);
		in_accessor.mLinearVelocity[i] = Vector3f(1.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0).rotated(GEUtil::random<double>(2 * PI)));
		in_accessor.mPosition[i] = Vector3f(Vector2f(camRect.randomPointInside()), 0);
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, mMaxParticles, in_system.getTransform(), in_system.getDepthTransform());
}

void AmbientDustSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	Rectangled camRect = GE.cameras().get(CameraManager::ID::main)->getTransformedRectangle();
	Vector2d axes[2];
	camRect.getAxes(axes);
	double dots[2] = {
		axes[0].dotSelf(),
		axes[1].dotSelf()
	};

	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * in_dt;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);

		Vector2d fromOrigin = Vector2f(in_accessor.mPosition[i]) - camRect.getOrigin();
		for (uint j = 0; j < 2; j++) {
			double dot = fromOrigin.dot(axes[j]);
			if (dot < 0) {
				in_accessor.mPosition[i] += Vector3d(axes[j], 0);
			}
			else if (dot > dots[j]) {
				in_accessor.mPosition[i] -= Vector3d(axes[j], 0);
			}
		}
	}
	ParticleSystem2::transformParticles(in_accessor, in_system.getCount(), in_system.getTransform(), in_system.getDepthTransform());
}

void AmbientDustSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}

void AmbientDustSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}
