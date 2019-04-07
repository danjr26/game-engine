#include "test_enemy.h"
#include "game_engine.h"
#include "game.h"
#include "particle_system2_specifiers.h"

TestEnemy::TestEnemy() :
	mRenderer(*this),
	mMover(*this),
	mHealth(*this) {
	
	GE.perFrameUpdate().add(this);

	subTransform(&mRigidBody.getTransform());

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 0.5));
	mRigidBody.setCollisionMask(mask);

	//mRigidBody.getTransform().setParent(&getTransform());
	getCollisionMask().addFilter(Game::MainCollisionContextFilters::enemy_ship);
	GE.game().getMainCollisionContext().add(&getCollisionMask());

	initMembers();
}

TestEnemy::~TestEnemy() {
	GE.perFrameUpdate().remove(this);
	GE.game().getMainCollisionContext().remove(&getCollisionMask());
}

void TestEnemy::update(double in_dt) {
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	CollisionContext2d& collisionContext = GE.game().getMainCollisionContext();
	collisionContext.getPartners(&getCollisionMask(), partners);

	CollisionResponder::Partner thisAsPartner;
	thisAsPartner.mMask = &getCollisionMask();

	for (auto it = partners.begin(); it < partners.end(); it++) {
		CollisionResponder* responder = (CollisionResponder*)(*it)->mMask->getParent();
		thisAsPartner.mCollision = (*it)->mCollision;
		if (responder) {
			CollisionPacket collisionPacket;
			responder->respond(thisAsPartner, collisionPacket);
			mHealth.damage(collisionPacket.mDamage);
		}
	}

	if (mHealth.isDead()) {
		TestEnemy* testEnemy = new TestEnemy();
		testEnemy->getTransform().setLocalPosition(Vector2d(4, 4));
		testEnemy->getDepthTransform().setLocalDepth(-0.1);

		Texture* waveTex = GE.assets().get<Texture>("SmokeTexture");
		TestEnemyExplosionSmokeSpecifier* waveSpecifier = new TestEnemyExplosionSmokeSpecifier;
		ParticleSystem2* waveSystem = new ParticleSystem2(waveTex, waveSpecifier);
		waveSystem->getTransform().setLocalPosition(getTransform().getLocalPosition());
		GE.perFrameUpdate().add(waveSystem);
		GE.render().add(waveSystem);
		GE.destruction().add(this);
	}

	mMover.update(in_dt);
}

TestEnemyRenderer& TestEnemy::getRenderer() {
	return mRenderer;
}

TestEnemyMover& TestEnemy::getMover() {
	return mMover;
}

TestEnemyHealth& TestEnemy::getHealth() {
	return mHealth;
}

RigidBody<2>& TestEnemy::getRigidBody() {
	return mRigidBody;
}

CollisionMask2d& TestEnemy::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}

void TestEnemy::initMembers() {
	mRenderer.init();
}
