#ifndef AI_H
#define AI_H
#include "component.h"
#include "trq.h"

class SwarmDroid;
class ShockDroid;

Vector3d Calculate_Rotation_Vector(Vector3d in_from, Vector3d in_to);

double When_Linear_Projectiles_Closest(Vector3d in_object1Position, Vector3d in_object1Velocity, 
	Vector3d in_object2Position, Vector3d in_object2Velocity);

Vector3d Distance_Between_Linear_Projectiles(Vector3d in_object1Position, Vector3d in_object1Velocity, 
	Vector3d in_object2Position, Vector3d in_object2Velocity, double in_timePassed);

double When_Linear_Projectiles_Distance_Equals(Vector3d in_object1Position, Vector3d in_object1Velocity, 
	Vector3d in_object2Position, Vector3d in_object2Velocity, double in_distance);

double When_Linear_Projectiles_Distance_Less_Than(Vector3d in_object1Position, Vector3d in_object1Velocity,
	Vector3d in_object2Position, Vector3d in_object2Velocity, double in_distance);

double When_Linear_Projectiles_Distance_Greater_Than(Vector3d in_object1Position, Vector3d in_object1Velocity,
	Vector3d in_object2Position, Vector3d in_object2Velocity, double in_distance);

Vector3d Calculate_Linear_Projectiles_Avoidance_Vector(Vector3d in_objectPosition, Vector3d in_objectVelocity, 
	Entity** in_entitiesToAvoid, uint in_nEntitiesToAvoid, double in_avoidanceRadius, double in_maxProjectionTime);

class EntityManager : public MutexProtected {
private:
	ResponsibleIndexedArray<Entity> entities;

public:
			EntityManager	();
			~EntityManager	();
	void	Add				(Entity* in_entity);
	void	Remove			(Entity* in_entity);

	IndexedArray<Entity> const& List() const;
};

template class IndexedArray<AIComponent>;
class AIManager : public MutexProtected {
private:
	IndexedArray<AIComponent> components;
	EntityManager entityManager;

public:
			AIManager	();
			~AIManager	();
	void	Add			(AIComponent* in_component);
	void	Remove		(AIComponent* in_component);

	EntityManager& Entities();
};

class SwarmDroidAI {
	friend class AITR;
	friend class SwarmDroid;
private:
	enum State {
		inactive,
		alert,
		pursueEnemy,
		firingRun,
		followAlly,
		goToLocation
	} state;

	SwarmDroid* parent;
	Entity*		aggressor;
	Entity*		targetEntity;
	Vector3d	targetLocation;		// doubles as offset in reference to targetEntity
	Vector3d	chosenDirection;
	double		chosenSpeed;
	double		stateStartTime;
	double		firingTolerance;	// plus/minus degrees
	double		firingRange;

			SwarmDroidAI	(SwarmDroid* in_parent);
	void	Refresh			();
	bool	Choose_Enemy	();
	bool	Confirm_Target	();
	bool	(*enemyCriteria)(Entity* in_entity);
};
/*
class ShockDroidAI {
public:
	ShockDroid* parent;
	Entity*		aggressor;
	Entity*		target;
	Vector3d	tgtdir;
	float		tgtspeed;
	Vector3d	tgtloc;
	double		prevmove;

			ShockDroidAI	(ShockDroid* parent);
	void	Refresh		();
	void	Choose_Target	();
	bool	Confirm_Target	();
};

#endif


#ifndef AI_H
#define AI_H

#include "clock.h"
#include "component.h"


class AIStateMachine;

class AIState {
public:
	float	priority;
	int		index;

						AIState		(float priority);
	virtual Vector3d	Refresh	();
	virtual bool		Is_Active	();
};

class AIStateMachine {
public:
	AIState**	states;
	int			nstates;
	int			maxstates;

				AIStateMachine	(int maxstates);
				~AIStateMachine	();
	Vector3d	Refresh		();
	void		Add_State		(AIState* state);
	void		Remove_State	(AIState* state);
};

class EntityManager {
public:
	static EntityManager*	active;
	Entity**				entities;
	int						nentities;
	const int				maxentities = 512;

			EntityManager	();
			~EntityManager	();
	void	Add_Entity		(Entity* e);
	void	Remove_Entity	(Entity* e);
};

class AIManager {
public:
	static AIManager*	active;
	AIComponent**		components;
	int					ncomponents;
	const int			maxcomponents = 512;

	EntityManager		emanager;

			AIManager		();
			~AIManager		();
	void	Refresh		(double t);
	void	Add	(AIComponent* component);
	void	Remove(AIComponent* component);
};

//AI Instances

class BasicShipAI;

class BasicShipAIRest : public AIState {
public:
	BasicShipAI* parent;

						BasicShipAIRest	(BasicShipAI* parent);
	virtual Vector3d	Refresh		();
	virtual bool		Is_Active		();
};

class BasicShipAIRegroup : public AIState {
public:
	BasicShipAI* parent;

						BasicShipAIRegroup	(BasicShipAI* parent);
	virtual Vector3d	Refresh			();
	virtual bool		Is_Active			();
};

class BasicShipAISearch : public AIState {
public:
	BasicShipAI*	parent;

	Vector3d		center;
	float			radius;

	double			startt;
	double			lifet;

	Vector3d		v;
	double			vstartt;
	double			vlifet;

	bool			continuous;

						BasicShipAISearch	(BasicShipAI* parent);
	virtual Vector3d	Refresh			();
	virtual bool		Is_Active			();
};

class BasicShipAIPursue : public AIState {
public:
	BasicShipAI*	parent;
	Entity*			target;
	TransitionTracker tracker;

						BasicShipAIPursue	(BasicShipAI* parent);
	virtual Vector3d	Refresh			();
	virtual bool		Is_Active			();
};

class BasicShipAI {
public:
	NPC*			parent;
	AIStateMachine	statemachine;
	Vector3d		home;
	float			homerad;

				BasicShipAI		(NPC* parent, Vector3d home, float homerad, int maxmissions);
				~BasicShipAI	();
	Vector3d	Refresh		();
};

class KamikazeAI {
public:
	NPC*			parent;
	Entity*			target;

				KamikazeAI	(NPC* parent, Entity* target);
				~KamikazeAI	();
	Vector3d	Refresh	();
};

float		Max_Approach_Speed	(Vector3d from, Vector3d to, Vector3d forward, float turnrate);
Vector3d	Avoid_Entities		(NPC* npc, Entity** ignore, int nignore);
*/
#endif
