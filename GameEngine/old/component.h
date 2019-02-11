#ifndef COMPONENT_H
#define COMPONENT_H
#include "array.h"
#include "transform.h"
#include "misc.h"

class	SphereTreeCollisionMask;
struct	Collision;
class	MechTR;
class	AITR;
class	SphereTree;
class	Clock;

class MechanicalComponent {
	friend class MechanicsManager;
	friend class MechTR;
private:
	uint mechanicsManagerArrayIndex;
protected:
	MechTR*	timeRequest;

public:
					MechanicalComponent	(Clock* in_clock, double in_step);
	virtual			~MechanicalComponent();
	virtual void	Update				(double in_timePassed);
};

class RenderComponent {
	friend class RenderManager;
private:
	uint renderManagerArrayIndex;
protected:
	Flags renderFlags;

public:
					RenderComponent	(Flags in_renderFlags);
	virtual			~RenderComponent();
	virtual void	Render			(Flags in_callFlags);
};

class OverlayComponent {
	friend class OverlayComponent;
	friend class OverlayManager;
private:
	uint overlayManagerArrayIndex;
protected:
	uint				parentArrayIndex;
	OverlayComponent*	parent;

	IndexedArray<OverlayComponent> children;

public:
					OverlayComponent	(OverlayComponent* in_parent, uint in_maxChildren, Vector3d (*in_Get_Position)(), Vector3d (*in_Get_Dimensions)());
					~OverlayComponent	();
	virtual void	Render				();
	void			Add					(OverlayComponent* in_child);
	void			Remove				(OverlayComponent* in_child);
	Vector3d		(*Get_Position)		();
	Vector3d		(*Get_Dimensions)	();
};

class AIComponent {
	friend class AIManager;
private:
	uint aiManagerArrayIndex;
protected:
	AITR* timeRequest;

public:
					AIComponent	(double in_step);
	virtual			~AIComponent();
	virtual void	Refresh		();
};

enum class Identity : Flags {
	laser,
	pulse,
	swarmdroid,
	shockdroid,
	asteroid,
	playership,
	missile,
	explosionblast
};

class CollidableComponent3 {
	friend class CollisionManager;
private:
	uint collisionManagerArrayIndex;
protected:
	CollisionMask3*	mask;
	Identity		type;

public:
							CollidableComponent3		(Identity in_type, SphereTreeCollisionMask* in_mask);
	virtual					~CollidableComponent3	();
	virtual bool			Collide					(const CollidableComponent3* in_that, const Collision* in_collision);
	virtual bool			Should_Check_Collision	(const CollidableComponent3* in_that) const;
	SphereTreeCollisionMask const*	Mask					() const;
	Identity				Type					() const;
};

enum class Allegiance : Flags {
	human,
	alien,
	neutral
};

class Entity : public MechanicalComponent, public CollidableComponent3 {
public:
	Transform3d	transform;
	Vector3d	velocity;
	Vector3d	rotationalVelocity;
	Allegiance	allegiance;
	uint		entityManagerArrayIndex;

					Entity					(Transform3d in_transform, Identity in_type, Allegiance in_allegiance, SphereTreeCollisionMask in_mask, double in_mechanicsStep);
	virtual			~Entity					();
	virtual void	Update					(double in_timePassed);
	virtual bool	Collide					(const CollidableComponent3* in_that, const Collision* in_collision) override;
	virtual bool	Should_Check_Collision	(const CollidableComponent3* in_that) const override;
};

class NPC : public Entity, public AIComponent {
public:
					NPC						(Transform3d in_transform, Identity in_type, Allegiance in_allegiance, SphereTreeCollisionMask in_mask, double in_mechanicsStep, double in_aiSstep);
	virtual			~NPC					();
	virtual void	Update					(double in_timePassed) override;
	virtual bool	Collide					(const CollidableComponent3* in_that, const Collision* in_collision) override;
	virtual bool	Should_Check_Collision	(const CollidableComponent3* in_that) const override;
	virtual void	Refresh					() override;
};
/*
double		When_Closest(Vector3d loc1, Vector3d vel1, Vector3d loc2, Vector3d vel2);
Vector3d	How_Close	(Vector3d loc1, Vector3d vel1, Vector3d loc2, Vector3d vel2, double t);
double		When_Close	(Vector3d loc1, Vector3d vel1, Vector3d loc2, Vector3d vel2, double d) {
	Vector3d dloc = loc1 - loc2;
	Vector3d dvel = vel1 - vel2;

	double a = dvel.Dot(dvel);
	double b = dvel.Dot(dloc) * 2;
	double c = dloc.Dot(dloc) - d * d;

	double disc = b * b - 4 * a * c;

	if (disc < 0 || (b > 0 && disc < b * b))
		return -1.0;

	double sqrtdisc = sqrt(disc);

	double result = (-b - sqrtdisc) / (a + a);

	if (result > 0.0)
		return result;
	else
		return (-b + sqrtdisc) / (a + a);
}*/

#endif