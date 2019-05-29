#include "laser_cannon_bullet.h"
#include "game_engine.h"
#include "particle_system2_specifiers.h"
#include "game.h"

LaserCannonBullet::LaserCannonBullet() :
	mCollisionResponder(
		[this](const CollisionPartner2d& in_partner, CollisionPacket& out_packet) {
	out_packet.mDamage.mAmount = 2.0;
	out_packet.mDamage.mMethod = DamagePacket::Method::bulk;
	out_packet.mImpulse.mPosition = in_partner.mCollision.mPoint;
	out_packet.mImpulse.mVector = Vector2d();
}),
mRigidBody(),
mSprite(),
mAge(0.0),
mLifeSpan(5.0) {

	Texture* tex = GE.assets().get<Texture>("LaserSpriteTexture");
	mSprite.setTexture(tex);

	mSprite.setRectangle(AxisAlignedRectangled::fromCenter(Vector2d(), Vector2d(0.8, 0.8)));

	mSprite.getTransform().setParent(&getTransform());
	mSprite.getDepthTransform().setParent(&getDepthTransform());

	mSprite.getTextureInstance().getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);

	mSprite.setColor(ColorRGBAf(0.5, 0.5, 0.9, 1.0));

	GE.render().add(&mSprite);
	GE.perFrameUpdate().add(this);

	Point2CollisionMask<double> mask(Vector2d(0.4, 0.0));
	mRigidBody.setCollisionMask(mask);

	mRigidBody.setAngularMass(0);
	mRigidBody.setLinearMass(0);

	mRigidBody.getTransform().setParent(&getTransform());
	getCollisionMask().addFilter(Game::CollisionFilters::player_bullet);
	GE.game().getMainCollisionContext().add(&getCollisionMask());
}

LaserCannonBullet::~LaserCannonBullet() {
	GE.render().remove(&mSprite);
	GE.perFrameUpdate().remove(this);
	GE.game().getMainCollisionContext().remove(&getCollisionMask());
}

void LaserCannonBullet::update(double in_dt) {
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	CollisionContext2d& collisionContext = GE.game().getMainCollisionContext();
	collisionContext.getPartners(&getCollisionMask(), partners);

	mAge += in_dt;
	if (mAge >= mLifeSpan || partners.size() > 0) {
		GE.destruction().add(this);
	}

	if (partners.size() > 0) {
		Texture* tex = GE.assets().get<Texture>("SparkSpriteTexture");
		TestBulletImpactSparksSpecifier* specifier = new TestBulletImpactSparksSpecifier;
		ParticleSystem2* particles = new ParticleSystem2(tex, specifier);
		particles->getTransform().setLocalPosition(partners[0]->mCollision.mPoint);

		GE.perFrameUpdate().add(particles);
		GE.render().add(particles);
	}

	Vector2d direction = getTransform().applyToLocalDirection(Vector2d(1, 0));
	getTransform().translateLocal(direction * 15.0 * in_dt);
}

CollisionMask2d& LaserCannonBullet::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}
