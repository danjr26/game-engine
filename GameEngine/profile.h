#ifndef PROFILE_H
#define PROFILE_H

#include "player_ship.h"

struct LevelRecord {
	int level;
	int rwdcompletion;
	int rwdmission;
	int rwdbounty;
	int rwdallies;
	int dbtfuel;
	int dbtdamage;
	int dbtmissiles;
	int netcred;
};

struct ShipUpgrade {
	float	effect;
	float	effectmult;
	int		cost;
	int		max;
};

struct ShipUpgrades {
	struct {
		ShipUpgrade armor;
		ShipUpgrade maxpower;
	} reactor;
	struct {
		ShipUpgrade armor;
		ShipUpgrade damagemult;
	} shield;
	struct {
		ShipUpgrade armor;
		ShipUpgrade maxturnrate;
	} propulsion;
};

struct PlayerLevelProfile {
	PlayerShipSettings	shipsettings;
	LevelRecord			record;
	bool				completed;
};

class PlayerProfile {
public:
	PlayerProfile* active;
	long int credits;

			PlayerProfile	();
			~PlayerProfile	();
	void	Load_SAV		(string path, string filename);
	void	Save_SAV		(string path, string filename);
};

#endif
