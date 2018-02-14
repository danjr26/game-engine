#ifndef SHOCK_DROID_H
#define SHOCK_DROID_H

#include "component.h"
#include "ship_systems.h"

class ShockDroid;

struct ShockDroidSettings {
	ShipPropulsionSettings	propulsion;
	ShipPulseGunSettings	pulsegun;
	ShipReactorSettings		reactor;
};

class ShockDroidPropulsion : public ShipSystem<ShockDroid> {
public:
	ShipPropulsionSettings settings;

			ShockDroidPropulsion(ShockDroid* parent, ShipPropulsionSettings& settings);
	void	Update				(double t);
};

class ShockDroidPulseGun : public ShipSystem<ShockDroid> {
public:
	ShipPulseGunSettings settings;
	PulseGun gun;

	ShockDroidPulseGun	(ShockDroid* parent, ShipPulseGunSettings& settings);
	void	Update		(double t);
	void	Fire		();
};

class ShockDroidReactor : public ShipSystem<ShockDroid> {
public:
	ShipReactorSettings settings;

			ShockDroidReactor	(ShockDroid* parent, ShipReactorSettings& settings);
	void	Update				(double t);
};

class ShockDroid : public NPC {
public:
	static ShockDroidSettings defsettings;
	ShockDroidSettings		settings;
	ShockDroidReactor		reactor;
	ShockDroidPropulsion	propulsion;
	ShockDroidPulseGun		pulsegun;

	MeshInstance*			mesh;

	ShockDroidAI			ai;

	Stepper					spawn;
	bool					spawned;


					ShockDroid		(Transform3d transform, ShockDroidSettings& settings);
	virtual			~ShockDroid		();
	void			Take_Damage		(float damage);
	virtual void	Update			(double t);
	virtual bool	Collide			(CollidableComponent* that, Collision* collision);
	virtual void			(double t);
};


#endif