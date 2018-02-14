#ifndef SHIP_SYSTEMS_H
#define SHIP_SYSTEMS_H

#include "weapons.h"

struct ShipSystemSettings {
	float armormult;	//unitless
};

template <class T>
class ShipSystem {
public:
	ShipSystemSettings*	syssettings;
	T*					parent;
	float				efficiency;

					ShipSystem	(T* parent, ShipSystemSettings* settings);
	virtual			~ShipSystem	() {}
	virtual void	Update		(double t) {}
	void			Take_Damage	(float damage);
};

struct ShipLaserGunSettings {
	ShipSystemSettings shipsys;
	LaserGunSettings gun;
};

struct ShipMissileLauncherSettings {
	ShipSystemSettings shipsys;
	MissileLauncherSettings launcher;
	float	jammult;	//(this) * (1.0 - missile launcher efficiency) ^ 2 = chance of misfire (unitless)
	int		nmissiles;	//missiles
};

struct ShipPropulsionSettings {
	ShipSystemSettings shipsys;
	float maxspeed;		//meters per second
	float maxturnrate;	//degrees per second
	float maxaccel;		//meters per second per second
};

struct ShipPulseGunSettings {
	ShipSystemSettings shipsys;
	PulseGunSettings gun;
};

struct ShipReactorSettings {
	ShipSystemSettings shipsys;
};

struct ShipRepairerSettings {
	ShipSystemSettings shipsys;
	float startup;
	float cooldown;
	float maxrepair;
	float lasergunrate;
	float missilelauncherrate;
	float propulsionrate;
	float pulsegunrate;
	float repairerrate;
	float seekerlauncherrate;
	float shieldrate;
	float stabilizersrate;
};

struct ShipSeekerLauncherSettings {
	ShipSystemSettings shipsys;
};

struct ShipShieldSettings {
	ShipSystemSettings shipsys;
	float startup;		//seconds
	float cooldown;		//seconds
	float damagemult;	//unitless
};

/*struct ShipStabilizersSettings {
	ShipSystemSettings shipsys;
	float maxlinaccel;	//meters per second per second
	float maxrotaccel;	//degrees per second per second
};*/

template <class T>
ShipSystem<T>::ShipSystem(T* parent, ShipSystemSettings* settings) :
syssettings	(settings),
parent		(parent),
efficiency	(1.0f)
{}

template<class T>
void ShipSystem<T>::Take_Damage(float damage) {
	efficiency -= damage * syssettings->armormult;
	if (efficiency < 0.0f)
		efficiency = 0.0f;
}

#endif
