#include "game.h"
#include "game_engine.h"
#include "player_ship.h"
#include "burning_eye.h"
#include "test_weapon.h"
#include "laser_cannon.h"
#include "test_bullet.h"
#include "particle_system2.h"
#include "particle_system2_specifiers.h"
#include "particle_system2_factories.h"
#include "ribbon2.h"
#include "industrial_laser.h"
#include "minigun.h"

Game::Game() :
	mPointerInput(),
	mPlayerShipInput(),
	mPlayerShip(nullptr),
	mMainCollisionContext(AxisAlignedRectangled::fromCenter(Vector2d(), Vector2d(1024, 1024)), 10) {

	GE.input().add(&mPointerInput);
	GE.input().add(&mPlayerShipInput);
	GE.collision().add(&mMainCollisionContext);

	mMainCollisionContext.setPartnerTestActivation(std::pair<ubyte, ubyte>(
		Game::CollisionFilters::player_ship, 
		Game::CollisionFilters::enemy_ship
		), true);

	mMainCollisionContext.setPartnerTestActivation(std::pair<ubyte, ubyte>(
		Game::CollisionFilters::player_bullet,
		Game::CollisionFilters::enemy_ship
		), true);
}

Game::~Game() {
	if (mPlayerShip != nullptr) delete mPlayerShip;
}

void Game::init() {
	mPlayerShip = new PlayerShip();
	mPlayerShip->getWeaponsSystem().setPrimary(new Minigun);
	mPlayerShip->getDepthTransform().setLocalDepth(0.2);

	for (uint i = 0; i < 5; i++) {
		BurningEye* testEnemy = new BurningEye();
		testEnemy->getTransform().setLocalPosition(Vector2d(4 + i * 2.0, 4));
		testEnemy->getDepthTransform().setLocalDepth(-0.1);
	}
	/*
	
	Texture* dustTex = GE.assets().get<Texture>("SparkSpriteTexture");
	AmbientDustSpecifier* dustSpecifier = new AmbientDustSpecifier;
	ParticleSystem2* dustSystem = new ParticleSystem2(dustTex, dustSpecifier);
	GE.perFrameUpdate().add(dustSystem);
	GE.render().add(dustSystem);

	Texture* smallBulletTex = GE.assets().get<Texture>("SmallBulletTexture");
	Sprite* smallBullet = new Sprite(AxisAlignedRectangled::fromCenter(Vector2d(0, 0), Vector2d(1.0, 1.0)), smallBulletTex);
	GE.render().add(smallBullet);

	Ribbon2* ribbon = new Ribbon2(GE.assets().get<Texture>("LaserSpriteTexture"));
	Ribbon2::graph_t& ribbonGraph = ribbon->getGraph();

	Ribbon2::node_t& node1 = ribbonGraph.addNode();
	node1.mData.mPosition = Vector2d(0, 0);
	node1.mData.mWidth = 2.0;
	node1.mData.mColor = ColorRGBAf(0.0, 0.0, 1.0, 1.0);
	node1.mData.mUV1 = Vector2f(0.5, 0.0);
	node1.mData.mUV2 = Vector2f(0.5, 1.0);

	Ribbon2::node_t& node2 = ribbonGraph.addNode();
	node2.mData.mPosition = Vector2d(3, 0);
	node2.mData.mWidth = 2.0;
	node2.mData.mColor = ColorRGBAf(1.0, 0.0, 1.0, 1.0);
	node2.mData.mUV1 = Vector2f(0.5, 0.0);
	node2.mData.mUV2 = Vector2f(0.5, 1.0);

	Ribbon2::node_t& node3 = ribbonGraph.addNode();
	node3.mData.mPosition = Vector2d(7, 5);
	node3.mData.mWidth = 2.0;
	node3.mData.mColor = ColorRGBAf(1.0, 0.0, 0.0, 1.0);
	node3.mData.mUV1 = Vector2f(0.5, 0.0);
	node3.mData.mUV2 = Vector2f(0.5, 1.0);

	Ribbon2::EdgeData edgeData;
	node1.connect1(node2, edgeData);
	node2.connect1(node3, edgeData);

	ribbon->updateMesh();
	GE.render().add(ribbon);
	*/
	//IndustrialLaserBeam* beam = new IndustrialLaserBeam;
}

void Game::preupdate(double in_dt) {
	mMainCollisionContext.broadcast();
}

void Game::postupdate(double in_dt) {}

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

DamageManager& Game::getDamageManager() {
	return mDamageManager;
}

Game::AITargetContainer& Game::getAITargets() {
	return mAITargets;
}

