#include "test_bullet.h"
#include "game_engine.h"
#include "particle_system2_specifiers.h"
#include "game.h"

TestBullet::TestBullet() :
	mCollisionResponder(
		[this] (const CollisionPartner2d& i_partner, CollisionPacket& o_packet) {
	o_packet.mDamage.mAmount = 2.0;
	o_packet.mDamage.mMethod = DamagePacket::Method::bulk;
	o_packet.mImpulse.mPosition = i_partner.mCollision.mPoint;
	o_packet.mImpulse.mVector = Vector2d();
}),
	mRigidBody(),
	mSprite(),
	mAge(0) {
	
	Texture* tex = GE.assets().get<Texture>("LaserSpriteTexture");
	mSprite.setTexture(tex);

	mSprite.setRectangle(AABox2d::fromCenter(Vector2d(), Vector2d(0.8, 1.0)));

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
	GE.game().getMainCollisionContext().add(&getCollider());
}

TestBullet::~TestBullet() {
	GE.render().remove(&mSprite);
	GE.perFrameUpdate().remove(this);
	GE.game().getMainCollisionContext().remove(&getCollider());
}

void TestBullet::update(double i_dt) {
	std::vector<CollisionPartner2d*> partners;
	CollisionContext2d& collisionContext = GE.game().getMainCollisionContext();
	collisionContext.getPartners(&getCollider(), partners);

	mAge += i_dt;
	if (mAge >= 5.0 || partners.size() > 0) {
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
	getTransform().translateLocal(direction * 15.0 * i_dt);
}

Collider2d& TestBullet::getCollider() {
	Collider2d* mask = mRigidBody.getCollider();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}
