#include "player_ship_weapons_system.h"
#include "game_engine.h"
#include "game.h"

player_ship::WeaponsSystem::WeaponsSystem(PlayerShip& i_parent) :
	mParent(i_parent),
	mActivePrimary(nullptr),
	mActiveSecondary(nullptr),
	mShipInput(&GE.game().getPlayerShipInput())
{}

void player_ship::WeaponsSystem::update(double i_dt) {
	InputEvent _event;
	while (mShipInput.getNumberEvents() > 0) {
		_event = mShipInput.popEvent();
		if (_event.mType == InputEvent::Type::state_change) {
			switch (_event.mMessage) {
			case InputContext::States::fire_1:
				if (mActivePrimary == nullptr) break;
				if (_event.mState.newValue) mActivePrimary->startFire();
				else mActivePrimary->endFire();
				break;
			}
		}
	}
	mShipInput.clearEvents();

	Weapon::Feedback feedback;

	if (mActivePrimary != nullptr) mActivePrimary->update(i_dt, &feedback);

	mParent.getRigidBody().applyLocalImpulse({ mParent.getTransform().getLocalPosition(), feedback.mImpulse });

	if (mActiveSecondary != nullptr) mActiveSecondary->update(i_dt, &feedback);
}

Weapon* player_ship::WeaponsSystem::getPrimary() {
	return mActivePrimary;
}

Weapon* player_ship::WeaponsSystem::getSecondary() {
	return mActiveSecondary;
}

void player_ship::WeaponsSystem::setPrimary(Weapon* i_weapon) {
	if (i_weapon == nullptr && mActivePrimary != nullptr) {
		mActivePrimary->removeIgnoreMask(&mParent.getCollider());
	}
	mActivePrimary = i_weapon;
	if (mActivePrimary != nullptr) {
		mActivePrimary->getTransform().setParent(&mParent.getTransform());
		mActivePrimary->addIgnoreMask(&mParent.getCollider());
	}
}

void player_ship::WeaponsSystem::setSecondary(Weapon* i_weapon) {
	if (i_weapon == nullptr && mActiveSecondary != nullptr) {
		mActiveSecondary->removeIgnoreMask(&mParent.getCollider());
	}
	mActiveSecondary = i_weapon;
	if (mActiveSecondary != nullptr) {
		mActiveSecondary->getTransform().setParent(&mParent.getTransform());
		mActiveSecondary->addIgnoreMask(&mParent.getCollider());
	}
}
