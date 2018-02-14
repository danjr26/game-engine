#ifndef PLAYER_SHIP_H
#define PLAYER_SHIP_H
#include "component.h"
#include "mesh.h"
#include "ship_systems.h"
#include "particle_system.h"
#include "weapons.h"

class PlayerShip;

struct PlayerShipSettings {
	ShipLaserGunSettings		lasergun;
	ShipMissileLauncherSettings	missilelauncher;
	ShipPropulsionSettings		propulsion;
	ShipReactorSettings			reactor;
	ShipRepairerSettings		repairer;
	ShipShieldSettings			shield;
	Vector3d					cameraloc;	//meters
};

class PlayerShipLaserGuns : public ShipSystem<PlayerShip> {
public:
	ShipLaserGunSettings settings;
	LaserGun	lgun;
	LaserGun	rgun;
	Stepper		alternater;
	bool		left;
	bool		ready;
	bool		altready;

			PlayerShipLaserGuns	(PlayerShip* parent, ShipLaserGunSettings& settings);
	void	Update				(double t);
	void	Fire				();
	float	Fire_Rate			();
};

class PlayerShipMissileLauncher : public ShipSystem<PlayerShip> {
public:
	ShipMissileLauncherSettings settings;
	MissileLauncher launcher;
	int				nmissiles;
	bool			ready;
	bool			primed;

			PlayerShipMissileLauncher	(PlayerShip* parent, ShipMissileLauncherSettings& settings);
	void	Update						(double t);
	void	Prime						();
	void	Unprime						();
	void	Fire						();
	float	Jam_Chance					();
	void	Jam							();
};

class PlayerShipPropulsion : public ShipSystem<PlayerShip> {
public:
	ShipPropulsionSettings settings;
	float fbvel;
	float rlvel;
	float defvel;

			PlayerShipPropulsion(PlayerShip* parent, ShipPropulsionSettings& settings);
	void	Update				(double t);
};

class PlayerShipReactor : public ShipSystem<PlayerShip> {
public:
	ShipReactorSettings settings;

			PlayerShipReactor	(PlayerShip* parent, ShipReactorSettings& settings);
	void	Update				(double t);
};

class PlayerShipRepairer : public ShipSystem<PlayerShip> {
public:
	ShipRepairerSettings settings;
	TransitionTracker trans;

			PlayerShipRepairer	(PlayerShip* parent, ShipRepairerSettings& settings);
	void	Update				(double t);
	void	Repair				(float& sysefficiency, float sysrate, double t);
	float	Repair_Amount		(float sysefficiency, float sysrate);
};

class PlayerShipShield : public ShipSystem<PlayerShip> {
public:
	ShipShieldSettings settings;
	MeshBlanket*		visual;
	TransitionTracker	trans;

			PlayerShipShield	(PlayerShip* parent, ShipShieldSettings& settings);
			~PlayerShipShield	();
	void	Update				(double t);
	void	Get_Hit				(Vector3d position, float intensity);
	float	Mod_Damage			(float damage);
};

class PlayerShip : public Entity {
public:
	static PlayerShipSettings	defsettings;
	PlayerShipSettings			settings;

	MeshInstance*				mesh;

	Emitter*					trail;

	PlayerShipLaserGuns			laserguns;
	PlayerShipMissileLauncher	missilelauncher;
	PlayerShipPropulsion		propulsion;
	PlayerShipReactor			reactor;
	PlayerShipRepairer			repairer;
	PlayerShipShield			shield;

	bool						firstperson;

					PlayerShip	(Transform3d& transform, PlayerShipSettings& settings);
					~PlayerShip	();
	void			Take_Damage	(float damage);
	void			Set_Person	(bool firstperson);
	virtual void	Update		(double tstep);
	virtual bool	Collide		(CollidableComponent* that, Collision* collision);
};


#endif