#include "game.h"
#include "game_engine.h"
#include "player_ship.h"
#include "test_enemy.h"
#include "test_weapon.h"
#include "test_bullet.h"
#include "particle_system2.h"
#include "particle_system2_specifiers.h"

Game::Game() :
	mPointerInput(),
	mPlayerShipInput(),
	mPlayerShip(nullptr),
	mMainCollisionContext(AxisAlignedRectangled::fromCenter(Vector2d(), Vector2d(1024, 1024)), 8) {

	GE.input().add(&mPointerInput);
	GE.input().add(&mPlayerShipInput);
	GE.collision().add(&mMainCollisionContext);

	mMainCollisionContext.setPartnerTestActivation(std::pair<ubyte, ubyte>(
		Game::MainCollisionContextFilters::player_ship, 
		Game::MainCollisionContextFilters::enemy
		), true);

	mMainCollisionContext.setPartnerTestActivation(std::pair<ubyte, ubyte>(
		Game::MainCollisionContextFilters::player_bullet,
		Game::MainCollisionContextFilters::enemy
		), true);
}

void Game::Init() {
	mPlayerShip = new PlayerShip();
	mPlayerShip->getWeaponsSystem().setPrimary(new TestWeapon);

	TestEnemy* testEnemy = new TestEnemy();
	testEnemy->getTransform().setLocalPosition(Vector2d(4, 4));

	Texture* dustTex = GE.assets().get<Texture>("SparkSpriteTexture");
	AmbientDustSpecifier* dustSpecifier = new AmbientDustSpecifier;
	ParticleSystem2* dustSystem = new ParticleSystem2(dustTex, dustSpecifier);
	GE.perFrameUpdate().add(dustSystem);
	GE.render().add(dustSystem);
}

PointerInputContext& Game::getPointerInput() {
	return mPointerInput;
}

PlayerShipInputContext& Game::getPlayerShipInput() {
	return mPlayerShipInput;
}

PlayerShip* Game::getPlayerShip() {
	return mPlayerShip;
}

CollisionContext2d& Game::getMainCollisionContext() {
	return mMainCollisionContext;
}
