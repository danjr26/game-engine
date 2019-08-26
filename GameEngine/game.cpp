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
#include "fractal_perlin.h"
#include "needler_iris_specifier.h"
#include "needler_pupil_specifier.h"

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
	mPlayerShip = new player_ship::PlayerShip();
	mPlayerShip->getWeaponsSystem().setPrimary(new Minigun);
	mPlayerShip->getDepthTransform().setLocalDepth(0.2);

	ParticleSystem2* newSystem = new ParticleSystem2(GE.assets().get<Texture>("WaveTexture"), new needler::IrisSpecifier);
	newSystem->setBlendSettings(BlendSettings::additive());
	newSystem->getDepthTransform().setLocalDepth(0.0);
	GE.perFrameUpdate().add(newSystem);
	GE.render().add(newSystem);

	newSystem = new ParticleSystem2(GE.assets().get<Texture>("GlowTexture"), new needler::PupilSpecifier);
	newSystem->setBlendSettings(BlendSettings::subtractive());
	newSystem->getDepthTransform().setLocalDepth(0.1);
	GE.perFrameUpdate().add(newSystem);
	GE.render().add(newSystem);


	FractalPerlin2f fractalPerlin(Vector2ui::filled(8), 8, Vector2f::filled(2), 2);
	
	/*
	Vector2ui dimensions(Vector2ui::filled(512));
	ColorRGBAc* buffer = new ColorRGBAc[dimensions.componentProduct()];
	for (uint i = 0; i < dimensions.componentProduct(); i++) {
		Vector2ui coords;
		coords.setAsCoords(dimensions, i);
		Vector2f normedCoords = Vector2f(coords).compwise(Vector2f(dimensions).componentInverted());
		float value = GEUtil::clamp<float>(fractalPerlin.evaluate(normedCoords), 0, 1);
		value = powf(value, 2.2);
		buffer[i] = ColorRGBAf(1, 1, 1, value);
	}

	Texture* perlinTexture = new Texture(Texture::Type::_2d, Vector3ui(dimensions, 1), (uchar*)buffer, 4, 8, Texture::Flags::mipmaps);
	Sprite* perlinSprite = new Sprite(AxisAlignedRectangled::fromCenter(Vector2d(), Vector2d::filled(20)), perlinTexture);
	GE.render().add(perlinSprite);

	*/
	for (uint i = 0; i < 5; i++) {
		burning_eye::BurningEye* testEnemy = new burning_eye::BurningEye();
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

	*/

	Ribbon2* ribbon = new Ribbon2(GE.assets().get<Texture>("SparkSpriteTexture"));
	Ribbon2::graph_t& ribbonGraph = ribbon->getGraph();

	HermiteTransitioner<float, 2> transitioner;

	transitioner.mKeys.push_back({ LocatedVector2f(Vector2f(0, 0), Vector2f(10, 5)), 1.0 });
	transitioner.mKeys.push_back({ LocatedVector2f(Vector2f(10, 0), Vector2f(0, 0)), 0.0 });

	Ribbon2::EdgeData edgeData;
	Ribbon2::node_t* prevNode = nullptr;
	uint nPoints = 1000;

	for (uint i = 0; i < nPoints; i++) {
		float progress = i / (float)nPoints;
		Ribbon2::node_t& newNode = ribbonGraph.addNode();
		newNode.mData.mPosition = transitioner.evaluate(progress).mPosition;
		newNode.mData.mPosition += transitioner.evaluate(progress).mVector.orthogonal() * 
			(fractalPerlin.evaluate(Vector2f(progress, 0.5)) - 0.5) * 0.2;
		newNode.mData.mWidth = GEUtil::cerp<float>(0.1, 0.02, progress) * 10.0;
		newNode.mData.mColor = ColorRGBAf(0.5, 0.5, 1.0, 1.0);
		newNode.mData.mUV1 = Vector2f(0.5, 0.0);
		newNode.mData.mUV2 = Vector2f(0.5, 1.0);

		if (prevNode) prevNode->connect1(newNode, edgeData);
		prevNode = &newNode;
	}

	ribbon->updateMesh();
	GE.render().add(ribbon);
}

void Game::preupdate(double in_dt) {
	mMainCollisionContext.broadcast();
}

void Game::postupdate(double in_dt) {}

PointerInputContext& Game::getPointerInput() {
	return mPointerInput;
}

player_ship::InputContext& Game::getPlayerShipInput() {
	return mPlayerShipInput;
}

player_ship::PlayerShip* Game::getPlayerShip() {
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

