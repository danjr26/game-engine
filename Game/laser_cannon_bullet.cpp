#include "laser_cannon_bullet.h"
#include "game_engine.h"
#include "particle_system2_specifiers.h"
#include "game.h"

LaserCannonBullet::LaserCannonBullet() :
mRigidBody(),
mSprite(),
mAge(0.0),
mLifeSpan(5.0) {

	Texture* tex = GE.assets().get<Texture>("LaserSpriteTexture");
	mSprite.setTexture(tex);

	mSprite.setRectangle(AABox2d::fromCenter(Vector2d(), Vector2d(0.8, 0.8)));

	mSprite.getTransform().setParent(&getTransform());
	mSprite.getDepthTransform().setParent(&getDepthTransform());

	mSprite.getTextureInstance().getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);

	mSprite.setColor(ColorRGBAf(0.5, 0.5, 0.9, 1.0));

	GE.render().add(&mSprite);
	GE.perFrameUpdate().add(this);

	Point2Collider<double> mask(Vector2d(0.4, 0.0));
	mRigidBody.setCollider(mask);

	mRigidBody.setAngularMass(0);
	mRigidBody.setLinearMass(0);

	mRigidBody.getTransform().setParent(&getTransform());
	getCollider().addFilter(Game::CollisionFilters::player_bullet);
	getCollider().setQueue(&mCollisionQueue);
	GE.game().getMainCollisionContext().add(&getCollider());
}

LaserCannonBullet::~LaserCannonBullet() {
	GE.render().remove(&mSprite);
	GE.perFrameUpdate().remove(this);
	GE.game().getMainCollisionContext().remove(&getCollider());
}

void LaserCannonBullet::update(double i_dt) {
	mAge += i_dt;
	if (mAge >= mLifeSpan) {
		GE.destruction().add(this);
	}

	for (auto it = mCollisionQueue.begin(); !mCollisionQueue.empty(); it = mCollisionQueue.begin()) {
		if (it->mMask->hasFilter(Game::CollisionFilters::solid)) {
			Texture* tex = GE.assets().get<Texture>("SparkSpriteTexture");
			TestBulletImpactSparksSpecifier* specifier = new TestBulletImpactSparksSpecifier;
			ParticleSystem2* particles = new ParticleSystem2(tex, specifier);
			particles->getTransform().setLocalPosition(it->mCollision.mPoint);

			GE.perFrameUpdate().add(particles);
			GE.render().add(particles);

			DamageReceiver* receiver = GE.game().getDamageManager().get(it->mMask);
			if (receiver) {
				DamagePacket packet;
				packet.mAmount = 4.0;
				packet.mMethod = DamagePacket::Method::bulk;
				receiver->receiveDamage(packet, i_dt);
			}
			GE.destruction().add(this);
		}
		mCollisionQueue.pop_front();
	}

	Vector2d direction = getTransform().applyToLocalDirection(Vector2d(1, 0));
	getTransform().translateLocal(direction * 10.0 * i_dt);
}

Collider2d& LaserCannonBullet::getCollider() {
	Collider2d* mask = mRigidBody.getCollider();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}
