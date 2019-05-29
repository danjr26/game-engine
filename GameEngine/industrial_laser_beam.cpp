#include "industrial_laser_beam.h"
#include "game_engine.h"
#include "game.h"
#include "raycast.h"

IndustrialLaserBeam::IndustrialLaserBeam() :
mSprite(),
mRaycastFilter(
	[] (CollisionMask2d* in_mask) {
		return true;
	}
),
mRadius(0.8),
mCollisionMask(LineSegment2d::fromPoints(
	Vector2d(0.0, 0.0), Vector2d(10.0, 0.0)
)) {

	Texture* tex = GE.assets().get<Texture>("LaserSpriteTexture");
	mSprite.setTexture(tex);
	mSprite.setUVs(Vector2f(0.5f, 0.0f), Vector2f(0.5f, 1.0f));

	mSprite.setRectangle(AxisAlignedRectangled::fromExtrema(Vector2d(0.0, -mRadius), Vector2d(10.0, mRadius)));

	mSprite.getTransform().setParent(&getTransform());
	mSprite.getDepthTransform().setParent(&getDepthTransform());

	mSprite.getTextureInstance().getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);

	mSprite.setColor(ColorRGBAf(0.7, 0.0, 0.9, 1.0));

	GE.render().add(&mSprite);
	GE.perFrameUpdate().add(this);

	mCollisionMask.addFilter(Game::CollisionFilters::player_bullet);
	GE.game().getMainCollisionContext().add(&getCollisionMask());
}

IndustrialLaserBeam::~IndustrialLaserBeam() {}

void IndustrialLaserBeam::update(double in_dt) {
	Ray2d ray = Ray2d::fromPointDirection(
		getTransform().getWorldPosition(),
		getTransform().getWorldRotation().applyTo(Vector2d(1, 0))
	);
	typename CollisionContext2d::CollisionPartner partner;
	double length = raycast(ray, *GE.collision().getActive2(), partner, mRaycastFilter) ?
		(partner.mCollision.mPoint - getTransform().getWorldPosition()).magnitude() : 50.0;
	
	mCollisionMask.getBasis() = 
	LineSegment2d::fromPoints(
		Vector2d(0.0, 0.0), Vector2d(length, 0.0)
	);

	mSprite.setRectangle(AxisAlignedRectangled::fromExtrema(
		Vector2d(0.0, -mRadius), Vector2d(length, mRadius)
	));
	
}

CollisionMask2d& IndustrialLaserBeam::getCollisionMask() {
	return mCollisionMask;
}

void IndustrialLaserBeam::setFilter(std::function<bool(CollisionMask2d*)> in_filter) {
	mRaycastFilter = in_filter;
}

void IndustrialLaserBeam::setIgnoreMasks(std::unordered_set<CollisionMask2d*>& in_masks) {
	mIgnoreMasks = in_masks;
}

std::unordered_set<CollisionMask2d*>& IndustrialLaserBeam::getIgnoreMasks() {
	return mIgnoreMasks;
}
