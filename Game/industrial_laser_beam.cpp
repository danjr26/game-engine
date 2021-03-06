#include "industrial_laser_beam.h"
#include "game_engine.h"
#include "game.h"
#include "raycast.h"
#include "damage_manager.h"

IndustrialLaserBeam::IndustrialLaserBeam() :
mSprite(),
mRaycastFilter(
	[] (Collider2d* i_mask) {
		return true;
	}
),
mRadius(0.8),
mIsOn(true) {

	Texture* tex = GE.assets().get<Texture>("LaserSpriteTexture");
	mSprite.setTexture(tex);
	mSprite.setUVs(Vector2f(0.5f, 0.0f), Vector2f(0.5f, 1.0f));

	mSprite.setRectangle(AABox2d::fromExtrema(Vector2d(0.0, -mRadius), Vector2d(10.0, mRadius)));

	mSprite.getTransform().setParent(&getTransform());
	mSprite.getDepthTransform().setParent(&getDepthTransform());

	mSprite.getTextureInstance().getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);

	mSprite.setColor(ColorRGBAf(0.7, 0.0, 0.9, 1.0));

	GE.render().add(&mSprite);
	GE.perFrameUpdate().add(this);
}

IndustrialLaserBeam::~IndustrialLaserBeam() {}

void IndustrialLaserBeam::update(double i_dt) {
	if (!mIsOn) return;
	Ray2d ray = Ray2d::fromPointDirection(
		getTransform().getWorldPosition(),
		getTransform().getWorldRotation().applyTo(Vector2d(1, 0))
	);
	CollisionPartner2d partner;
	bool hit = raycast(ray, *GE.collision().getActive2(), partner, mRaycastFilter);

	double length = hit ? (partner.mCollision.mPoint - getTransform().getWorldPosition()).magnitude() : 50.0;

	mSprite.setRectangle(AABox2d::fromExtrema(
		Vector2d(0.0, -mRadius), Vector2d(length, mRadius)
	));
	
	if (hit) {
		DamageReceiver* receiver = GE.game().getDamageManager().get(partner.mMask);
		if (receiver) {
			DamagePacket packet;
			packet.mAmount = 20.0;
			packet.mMethod = DamagePacket::Method::per_second;
			receiver->receiveDamage(packet, i_dt);
		}
	}
}

void IndustrialLaserBeam::setFilter(std::function<bool(Collider2d*)> i_filter) {
	mRaycastFilter = i_filter;
}

void IndustrialLaserBeam::setIgnoreMasks(std::unordered_set<Collider2d*>& i_masks) {
	mIgnoreMasks = i_masks;
}

std::unordered_set<Collider2d*>& IndustrialLaserBeam::getIgnoreMasks() {
	return mIgnoreMasks;
}

void IndustrialLaserBeam::setToggle(bool i_value) {
	mIsOn = i_value;
	if (mIsOn) {
		mSprite.enable();
	}
	else {
		mSprite.disable();
	}
}
