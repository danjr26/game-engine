#ifndef TEST_ENEMY_HEALTH_H
#define TEST_ENEMY_HEALTH_H

#include "damage_receiver.h"

namespace burning_eye {
	class BurningEye;

	class Health : public DamageReceiver {
	private:
		BurningEye& mParent;
		float mHealth;

	public:
		Health(BurningEye& i_parent);
		void receiveDamage(const DamagePacket& i_damage, double i_dt) override;
		bool isDead() const;
	};
}

#endif