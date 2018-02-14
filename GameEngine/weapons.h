#ifndef WEAPONS_H
#define WEAPONS_H
//#include "clock.h"
#include "projectiles.h"

class Weapon {
public:
	Entity*			parent;
	Transform3d		transform;
	Stepper			stepper;
	bool			ready;
	bool			subjloc;

					Weapon		(Entity* parent, Transform3d transform);
					~Weapon		();
	void			Unready		();
	virtual void	Fire		();
	virtual void	Update		(double t);
};

struct LaserGunSettings {
	LaserSettings laser;
	float firerate;	//Hertz
};

class LaserGun : public Weapon {
public:
	static LaserGunSettings defsettings;
	LaserGunSettings	settings;

			LaserGun	(Entity* parent, Transform3d& transform, LaserGunSettings& settings);
			~LaserGun	();
	void	Fire		();
};

struct MissileLauncherSettings {
	MissileSettings missile;
	float firerate;	//reps per second
};

class MissileLauncher : public Weapon {
public:
	static MissileLauncherSettings defsettings;
	MissileLauncherSettings settings;

			MissileLauncher	(Entity* parent, Transform3d& transform, MissileLauncherSettings& settings);
			~MissileLauncher();
	Entity*	Get_Target		();
	void	Fire			();
};

struct PulseGunSettings {
	PulseSettings pulse;
	float firerate;	//reps per second
};

class PulseGun : public Weapon {
public:
	static PulseGunSettings defsettings;
	PulseGunSettings settings;

			PulseGun	(Entity* parent, Transform3d& transform, PulseGunSettings& settings);
			~PulseGun	();
	void	Fire		();
};

void LaserFireFlash(Vector3d position);
void PulseFireFlash(Vector3d position);

#endif
