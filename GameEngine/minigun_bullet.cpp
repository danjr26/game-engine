#include "minigun_bullet.h"
#include "game_engine.h"
#include "game.h"

MinigunBullet::MinigunBullet() :
	mRigidBody(),
	mSprite(),
	mAge(0.0),
	mLifeSpan(2.0) {

	Texture* tex = GE.assets().get<Texture>("MinigunBulletTexture");
	mSprite.setTexture(tex);

	mSprite.setRectangle(AxisAlignedRectangled::fromCenter(Vector2d(), Vector2d(0.3, 0.3)));

	mSprite.getTransform().setParent(&getTransform());
	mSprite.getDepthTransform().setParent(&getDepthTransform());

	mSprite.getTextureInstance().getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);

	mSprite.setColor(ColorRGBAf(0.9, 0.7, 0.4, 1.0));

	GE.render().add(&mSprite);
	GE.perFrameUpdate().add(this);

	Point2CollisionMask<double> mask(Vector2d(0.4, 0.0));
	mRigidBody.setCollisionMask(mask);

	mRigidBody.setAngularMass(0);
	mRigidBody.setLinearMass(0);

	mRigidBody.getTransform().setParent(&getTransform());
	getCollisionMask().addFilter(Game::CollisionFilters::player_bullet);
	getCollisionMask().setQueue(&mCollisionQueue);
	GE.game().getMainCollisionContext().add(&getCollisionMask());
}

MinigunBullet::~MinigunBullet() {
	GE.render().remove(&mSprite);
	GE.perFrameUpdate().remove(this);
	GE.game().getMainCollisionContext().remove(&getCollisionMask());
}

void MinigunBullet::update(double in_dt) {
	mAge += in_dt;
	if (mAge >= mLifeSpan) {
		GE.destruction().add(this);
	}

	for (auto it = mCollisionQueue.begin(); !mCollisionQueue.empty(); it = mCollisionQueue.begin()) {
		if (it->mMask->hasFilter(Game::CollisionFilters::solid)) {
			DamageReceiver* receiver = GE.game().getDamageManager().get(it->mMask);
			if (receiver) {
				DamagePacket packet;
				packet.mAmount = 1.0;
				packet.mMethod = DamagePacket::Method::bulk;
				packet.mImpulse = { it->mCollision.mPoint, getTransform().applyToLocalDirection(Vector2d(1, 0)) };
				receiver->receiveDamage(packet, in_dt);
			}
			GE.destruction().add(this);
		}
		mCollisionQueue.pop_front();
	}

	Vector2d direction = getTransform().applyToLocalDirection(Vector2d(1, 0));
	getTransform().translateLocal(direction * 15.0 * in_dt);
}

CollisionMask2d& MinigunBullet::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}
