#ifndef SWARM_DROID_H
#define SWARM_DROID_H

#include "weapons.h"
#include "component.h"
#include "particle_system.h"
#include "ship_systems.h"
#include "ai.h"

class SwarmDroid;

struct SwarmDroidSettings {
	ShipPropulsionSettings	propulsion;
	ShipPulseGunSettings	pulsegun;
	ShipReactorSettings		reactor;
};

class SwarmDroidPropulsion : public ShipSystem<SwarmDroid> {
public:
	ShipPropulsionSettings settings;

			SwarmDroidPropulsion(SwarmDroid* parent, ShipPropulsionSettings& settings);
	void	Update				(double t);
};

class SwarmDroidPulseGun : public ShipSystem<SwarmDroid> {
public:
	ShipPulseGunSettings settings;
	PulseGun gun;

	SwarmDroidPulseGun(SwarmDroid* parent, ShipPulseGunSettings& settings);
	void	Update(double t);
	void	Fire();
};

class SwarmDroidReactor : public ShipSystem<SwarmDroid> {
public:
	ShipReactorSettings settings;

			SwarmDroidReactor	(SwarmDroid* parent, ShipReactorSettings& settings);
	void	Update				(double t);
};

class SwarmDroid : public NPC {
public:
	static SwarmDroidSettings defsettings;
	SwarmDroidSettings		settings;
	SwarmDroidReactor		reactor;
	SwarmDroidPropulsion	propulsion;
	SwarmDroidPulseGun		pulsegun;

	SwarmDroidAI			ai;

	MeshInstance*			mesh;

	Stepper					spawn;
	bool					spawned;

	float					fbvel;
	float					defvel;

					SwarmDroid		(Transform3d transform, SwarmDroidSettings& settings);
	virtual			~SwarmDroid		();
	void			Take_Damage		(float damage);
	virtual void	Update			(double t);
	virtual bool	Collide			(CollidableComponent3* that, Collision* collision);
	virtual void			(double t);
};

void DefaultExplosion(Vector3d position, float size);

#endif
