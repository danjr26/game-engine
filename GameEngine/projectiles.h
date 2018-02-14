#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "component.h"
#include "ai.h"
#include "fx.h"

struct LaserSettings {
	float damage;	//units
	float speed;	//meters per second
	float timeout;	//seconds
};

struct MissileSettings {
	float	damage;		//units
	float	accel;		//meters per second per second
	float	maxspeed;	//meters per second
	float	turnrate;	//degrees per second
	float	timeout;	//seconds
};

struct PulseSettings {
	float damage;	//units
	float speed;	//units per second
	float timeout;	//seconds
};

struct PulseOrbSettings {
	float damage;	//units
	float speed;	//units per second
	float timeout;	//seconds
};

class Laser : public Entity {
public:
	static LaserSettings defsettings;
	Entity*			parent;
	LaserSettings	settings;
	double			timeout;
	GlowLaser		visual;

			Laser		(Entity* parent, LaserSettings& settings, Transform3d& transform);
			~Laser		();
	void	Update		(double t) override final;
	bool	Collide		(CollidableComponent* that, Collision* collision) override final;
	bool	Should_Check_Collision(CollidableComponent* that) override final;
};

class Missile : public NPC {
public:
	static MissileSettings defsettings;
	Entity*			parent;
	MissileSettings	settings;
	Stepper			timeout;
	MeshInstance*	mesh;

					Missile		(Entity* parent, Entity* target, MissileSettings& settings, Transform3d& transform);
					~Missile	();
	virtual void	Update		(double t);
	virtual bool	Collide		(CollidableComponent* that, Collision* collision);
	virtual void		(double t);
};

class Pulse : public Entity {
public:
	static PulseSettings defsettings;
	Entity*			parent;
	PulseSettings	settings;
	double			timeout;
	GlowLaser		visual;

			Pulse		(Entity* parent, PulseSettings& settings, Transform3d& transform);
			~Pulse		();
	void	Update		(double t);
	bool	Collide		(CollidableComponent* that, Collision* collision);
	bool	Should_Check_Collision(CollidableComponent* that) override final;
};

class PulseOrb : public Entity {
public:
	
};

class ExplosionBlast : public MechanicalComponent, public CollidableComponent {
public:
	Transform3d transform;
	Vector3d	vel;
	float		expansion;
	float		damagek;
	float		forcek;
	Stepper		life;

					ExplosionBlast	(Transform3d transform, float expansion, float lifet, 
						float damagek, float forcek);
					~ExplosionBlast	();
	virtual void	Update			(double t);
	virtual bool	Collide			(CollidableComponent* that, Collision* collision);
	float			Get_Damage		(float d);
	float			Get_Force		(float d);
};

void MissileExplosion(Vector3d position);
void LaserRecoil(Vector3d position, Vector3d forward);
void PulseRecoil(Vector3d position, Vector3d forward);

#endif
