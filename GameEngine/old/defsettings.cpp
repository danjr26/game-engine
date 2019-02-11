#include "framework.h"

LaserSettings Laser::defsettings = {
	0.25f,	//damage
	150.0f,	//speed
	3.0f	//timeout
};

MissileSettings Missile::defsettings = {
	2.0f,	//damage
	75.0f,	//accel
	80.0f,	//maxspeed
	175.0f,	//turnrate
	10.0f,	//timeout
};

PulseSettings Pulse::defsettings = {
	0.01f,	//damage
	100.0f,	//speed
	3.0f	//timeout
};

LaserGunSettings LaserGun::defsettings = {
	Laser::defsettings,
	3.0f	//firerate
};

MissileLauncherSettings MissileLauncher::defsettings = {
	Missile::defsettings,
	1.0f	//firerate
};

PulseGunSettings PulseGun::defsettings = {
	Pulse::defsettings,
	5.0f	//firerate
};

PlayerShipSettings PlayerShip::defsettings = {
ShipLaserGunSettings{
	ShipSystemSettings{ 1.0f },
	LaserGun::defsettings
},
ShipMissileLauncherSettings{
	ShipSystemSettings{ 1.0f },
	MissileLauncher::defsettings,
	1.0f,	//jammult
	5		//nmissiles
},
ShipPropulsionSettings{
	ShipSystemSettings{ 1.0f },
	50.0f,	//maxspeed
	200.0f,	//maxturnrate
	25.0f	//maxaccel
},
ShipReactorSettings {
	ShipSystemSettings{ 1.0f }
},
ShipRepairerSettings{
	ShipSystemSettings{ 1.0f },
	2.0f,	//startup
	2.0f,	//cooldown
	0.75f,	//maxrepair
	0.10f,	//lasergunrate
	0.10f,	//missilelauncherrate
	0.10f,	//propulsionrate
	0.0f,	//pulsegunrate
	0.10f,	//repairerrate
	0.10f,	//seekerlauncherrate
	0.10f,	//shieldrate
	0.10f	//stabilizersrate
},
ShipShieldSettings{
	ShipSystemSettings{ 1.0f },
	2.0,	//startup
	2.0,	//cooldown
	0.5		//damagemult
},
Vector3d(0, 6, 20) 	//cameraloc
};

SwarmDroidSettings SwarmDroid::defsettings = {
ShipPropulsionSettings{
	ShipSystemSettings{ 1.0f },
	30.0f,	//maxspeed
	100.0f,	//maxturnrate
	25.0f	//maxaccel
},
ShipPulseGunSettings{
	ShipSystemSettings{ 1.0f },
	PulseGun::defsettings
},
ShipReactorSettings{
	ShipSystemSettings{ 1.0f },
}
};

ShockDroidSettings ShockDroid::defsettings = {
	ShipPropulsionSettings{
	ShipSystemSettings{ 1.0f },
	30.0f,	//maxspeed
	100.0f,	//maxturnrate
	25.0f	//maxaccel
},
ShipPulseGunSettings{
	ShipSystemSettings{ 1.0f },
	PulseGun::defsettings
},
ShipReactorSettings{
	ShipSystemSettings{ 1.0f },
}
};