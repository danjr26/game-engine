#ifndef PLAYER_SHIP_MOVER_H
#define PLAYER_SHIP_MOVER_H

class PlayerShip;

class PlayerShipMover {
private:
	PlayerShip& mParent;
public:
	PlayerShipMover(PlayerShip& in_parent);
};

#endif