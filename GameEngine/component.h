#ifndef COMPONENT_H
#define COMPONENT_H
#include "array.h"
#include "transform.h"
#include "misc.h"
#include <mutex>

class	CollisionMask;
struct	Collision;
class	MechTR;
class	AITR;
class	SphereTree;
class	Clock;

class MutexProtected {
public:
	mutable std::mutex mutex;

	MutexProtected();
	~MutexProtected();
};

class Stepper {
public:
	double step;
	double accumulation;

			Stepper		(double in_step);
			Stepper		(double in_step, double in_accumulation);
			~Stepper	();
	int		Step_Number	(double in_newAccumulation);
	double	Step_Total	(double in_newAccumulation);
};

class ProgressTracker {
public:
	double	accumulation;
	double	maxAccumulation;
	bool	capAtMax;

			ProgressTracker	(double in_maxAccumulation, bool in_capAtMax);
	void	Update			(double in_newAccumulation);
	double	Progress		() const;
	double	Inv_Progress	() const;
	bool	Is_Done			() const;

	template<class T>
	T Lin_Interp(T in_a, T in_b) const {
		double progress = Progress(); 
		double inverseProgress = 1.0 - progress;
		return (T)(in_a * inverseProgress + in_b * progress);
	} 
};

namespace TransitionStates {
	enum : Flags {
		off,
		offToOn,
		on,
		onToOff
	};
}

class TransitionTracker {
private:
	Flags			state;
	ProgressTracker	progressTracker;
	double			offToOnDuration;
	double			onToOffDuration;

public:
			TransitionTracker			(double in_offToOnDuration, double in_onToOffDuration);
	void	Update						(double in_newAccumulation);
	void	Turn_On						();
	void	Turn_Off					();
	void	Toggle						();
	bool	Is_Transitioning			() const;
	Flags	Get_State					() const;
	double	Get_Turn_On_Duration		() const;
	double	Get_Turn_Off_Duration		() const;
	double	Get_Progress_Accumulation	() const;
	void	Set_State					(Flags in_state);
	void	Set_Turn_On_Duration		(double in_offToOnDuration);
	void	Set_Turn_Off_Duration		(double in_onToOffDuration);
	void	Set_Progress_Accumulation	(double in_progressAccumulation);
	template<class T>
	T Lin_Interp(T in_a, T in_b) const {
		switch (state) {
		case TransitionStates::on:
			return in_b;
		case TransitionStates::off:
			return in_a;
		case TransitionStates::offToOn:
			return progressTracker.Lin_Interp(in_a, in_b);
		case TransitionStates::onToOff:
			return progressTracker.Lin_Interp(in_b, in_a);
		}
		return in_a;
	}
};

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

class CollidableComponent {
	friend class CollisionManager;
private:
	uint collisionManagerArrayIndex;
protected:
	CollisionMask*	mask;
	Identity		type;

public:
							CollidableComponent		(Identity in_type, CollisionMask* in_mask);
	virtual					~CollidableComponent	();
	virtual bool			Collide					(const CollidableComponent* in_that, const Collision* in_collision);
	virtual bool			Should_Check_Collision	(const CollidableComponent* in_that) const;
	CollisionMask const*	Mask					() const;
	Identity				Type					() const;
};

enum class Allegiance : Flags {
	human,
	alien,
	neutral
};

class Entity : public MechanicalComponent, public CollidableComponent {
public:
	Transform3d	transform;
	Vector3d	velocity;
	Vector3d	rotationalVelocity;
	Allegiance	allegiance;
	uint		entityManagerArrayIndex;

					Entity					(Transform3d in_transform, Identity in_type, Allegiance in_allegiance, 
						SphereTree* in_tree, double in_mechanicsStep);
	virtual			~Entity					();
	virtual void	Update					(double in_timePassed);
	virtual bool	Collide					(const CollidableComponent* in_that, const Collision* in_collision) override;
	virtual bool	Should_Check_Collision	(const CollidableComponent* in_that) const override;
};

class NPC : public Entity, public AIComponent {
public:
					NPC						(Transform3d in_transform, Identity in_type, Allegiance in_allegiance, 
						SphereTree* in_tree, double in_mechanicsStep, double in_aiSstep);
	virtual			~NPC					();
	virtual void	Update					(double in_timePassed) override;
	virtual bool	Collide					(const CollidableComponent* in_that, const Collision* in_collision) override;
	virtual bool	Should_Check_Collision	(const CollidableComponent* in_that) const override;
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