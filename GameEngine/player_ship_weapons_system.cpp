#include "player_ship_weapons_system.h"
#include "game_engine.h"
#include "game.h"

PlayerShipWeaponsSystem::PlayerShipWeaponsSystem(PlayerShip& in_parent) :
	mParent(in_parent),
	mActivePrimary(nullptr),
	mActiveSecondary(nullptr),
	mShipInput(&GE.game().getPlayerShipInput())
{}

void PlayerShipWeaponsSystem::update(double in_dt) {
	InputEvent _event;
	while (mShipInput.getNumberEvents() > 0) {
		_event = mShipInput.popEvent();
		if (_event.mType == InputEvent::Type::state_change) {
			switch (_event.mMessage) {
			case PlayerShipInputContext::States::fire_1:
				if (mActivePrimary == nullptr) break;
				if (_event.mState.newValue) mActivePrimary->startFire();
				else mActivePrimary->endFire();
				break;
			}
		}
	}
	mShipInput.clearEvents();

	Weapon::Feedback feedback;

	if (mActivePrimary != nullptr) mActivePrimary->update(in_dt, &feedback);

	mParent.getRigidBody().applyLocalImpulse({ mParent.getTransform().getLocalPosition(), feedback.mImpulse });

	if (mActiveSecondary != nullptr) mActiveSecondary->update(in_dt, &feedback);
}

Weapon* PlayerShipWeaponsSystem::getPrimary() {
	return mActivePrimary;
}

Weapon* PlayerShipWeaponsSystem::getSecondary() {
	return mActiveSecondary;
}

void PlayerShipWeaponsSystem::setPrimary(Weapon* in_weapon) {
	if (in_weapon == nullptr && mActivePrimary != nullptr) {
		mActivePrimary->removeIgnoreMask(&mParent.getCollisionMask());
	}
	mActivePrimary = in_weapon;
	if (mActivePrimary != nullptr) {
		mActivePrimary->getTransform().setParent(&mParent.getTransform());
		mActivePrimary->addIgnoreMask(&mParent.getCollisionMask());
	}
}

void PlayerShipWeaponsSystem::setSecondary(Weapon* in_weapon) {
	if (in_weapon == nullptr && mActiveSecondary != nullptr) {
		mActiveSecondary->removeIgnoreMask(&mParent.getCollisionMask());
	}
	mActiveSecondary = in_weapon;
	if (mActiveSecondary != nullptr) {
		mActiveSecondary->getTransform().setParent(&mParent.getTransform());
		mActiveSecondary->addIgnoreMask(&mParent.getCollisionMask());
	}
}
