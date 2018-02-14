#include "ai.h"
#include "framework.h"

Vector3d Calculate_Rotation_Vector(Vector3d in_from, Vector3d in_to) {
	double theta = in_from.Theta(in_to);
	return in_from.Cross(in_to).Normalized() * theta;
}

double When_Linear_Projectiles_Closest(Vector3d in_object1Position, Vector3d in_object1Velocity, 
		Vector3d in_object2Position, Vector3d in_object2Velocity) {
	Vector3d positionDifference = in_object1Position - in_object2Position;
	Vector3d velocityDifference = in_object1Velocity - in_object2Velocity;
	double dotProduct1 = positionDifference.Dot(velocityDifference);
	double dotProduct2 = velocityDifference.Dot(velocityDifference);
	if (dotProduct2 == 0.0)
		return 0.0;
	return -dotProduct1 / dotProduct2;
}

Vector3d Distance_Between_Linear_Projectiles(Vector3d in_object1Position, Vector3d in_object1Velocity, 
		Vector3d in_object2Position, Vector3d in_object2Velocity, double timePassed) {
	Vector3d object1UpdatedPosition = in_object1Position + in_object1Velocity * timePassed;
	Vector3d object2UpdatedPosition = in_object2Position + in_object1Velocity * timePassed;
	return object1UpdatedPosition - object2UpdatedPosition;
}

double When_Linear_Projectiles_Distance_Equals(Vector3d in_object1Position, Vector3d in_object1Velocity, 
		Vector3d in_object2Position, Vector3d in_object2Velocity, double in_distance) {
	Vector3d positionDifference = in_object1Position - in_object2Position;
	Vector3d velocityDifference = in_object1Velocity - in_object2Velocity;

	double a = velocityDifference.Dot(velocityDifference);
	double b = velocityDifference.Dot(positionDifference) * 2;
	double c = positionDifference.Dot(positionDifference) - in_distance * in_distance;

	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0 || (b > 0 && discriminant < b * b))
		return -1.0;

	double sqrtOfDiscriminant = sqrt(discriminant);

	double result = (-b - sqrtOfDiscriminant) / (a + a);

	if (result > 0.0)
		return result;
	else
		return (-b + sqrtOfDiscriminant) / (a + a);
}

double When_Linear_Projectiles_Distance_Less_Than(Vector3d in_object1Position, Vector3d in_object1Velocity,
		Vector3d in_object2Position, Vector3d in_object2Velocity, double in_distance) {
	if ((in_object1Position - in_object2Position).Magnitude() <= in_distance) {
		return 0.0;
	}
	else {
		return When_Linear_Projectiles_Distance_Equals(in_object1Position, in_object1Velocity, in_object2Position, in_object2Velocity, in_distance);
	}
}

double When_Linear_Projectiles_Distance_Greater_Than(Vector3d in_object1Position, Vector3d in_object1Velocity,
		Vector3d in_object2Position, Vector3d in_object2Velocity, double in_distance) {
	if ((in_object1Position - in_object2Position).Magnitude() >= in_distance) {
		return 0.0;
	}
	else {
		return When_Linear_Projectiles_Distance_Equals(in_object1Position, in_object1Velocity, in_object2Position, in_object2Velocity, in_distance);
	}
}

Vector3d Calculate_Linear_Projectiles_Avoidance_Vector(Vector3d in_objectPosition, Vector3d in_objectVelocity,
	CustomArray<Entity*>& in_entitiesToAvoid, double in_avoidanceRadius, double in_maxProjectionTime) {
	Vector3d avoidanceVector = Vector3d(0, 0, 0);
	for (uint i = 0; i < in_entitiesToAvoid.Size(); i++) {
		Entity* thisEntity = in_entitiesToAvoid[i];

		Vector3d positionDifference = in_objectPosition - thisEntity->transform.position;
		Vector3d velocityDifference = in_objectVelocity - thisEntity->velocity;

		double projectedCollisionTime = When_Linear_Projectiles_Distance_Less_Than(
			in_objectPosition, in_objectVelocity,
			thisEntity->transform.position, thisEntity->velocity, 
			thisEntity->Mask()->tree->trunk->radius + in_avoidanceRadius);

		Vector3d projectedEntityPosition = thisEntity->transform.position + thisEntity->velocity * projectedCollisionTime;

		if (projectedCollisionTime > 0.0 && projectedCollisionTime < in_maxProjectionTime) {
			avoidanceVector += (in_objectPosition - projectedEntityPosition).Normalized() * (in_maxProjectionTime - projectedCollisionTime);
		}
	}

	avoidanceVector.Normalize();

	return avoidanceVector;
}

EntityManager::EntityManager() :
entities(1024, offsetof(Entity, entityManagerArrayIndex), false) 
{}

EntityManager::~EntityManager() {
	std::lock_guard<std::mutex> lock(mutex);
	while (entities.Size() > 0)
		delete entities[0];
}

void EntityManager::Add(Entity* in_entity) {
	std::lock_guard<std::mutex> lock(mutex);
	entities.Add(in_entity);
}

void EntityManager::Remove(Entity* in_entity) {
	std::lock_guard<std::mutex> lock(mutex);
	entities.CustomArray<Entity*>::Remove(in_entity);
}

IndexedArray<Entity> const& EntityManager::List() const {
	std::lock_guard<std::mutex> lock(mutex);
	return entities;
}

AIManager::AIManager() :
components(256, offsetof(AIComponent, aiManagerArrayIndex), false),
entityManager() 
{}

AIManager::~AIManager() {
	std::lock_guard<std::mutex> lock(mutex);
	while (components.Size() > 0)
		delete components[0];
}

void AIManager::Add(AIComponent* in_component) {
	std::lock_guard<std::mutex> lock(mutex);
	components.Add(in_component);
}

void AIManager::Remove(AIComponent* in_component) {
	std::lock_guard<std::mutex> lock(mutex);
	components.CustomArray<AIComponent*>::Remove(in_component);
}

EntityManager& AIManager::Entities() {
	std::lock_guard<std::mutex> lock(mutex);
	return entityManager;
}

SwarmDroidAI::SwarmDroidAI(SwarmDroid* parent) :
parent			(parent),
aggressor		(nullptr),
targetEntity	(nullptr),
targetLocation	(Vector3d(0, 0, 0)),
chosenDirection	(Vector3d(0, 0, -1)),
chosenSpeed		(30.0),
state			(alert),
firingTolerance	(3.0),
firingRange		(100.0),
stateStartTime	(GEngine::Get().Play_Clock().Now())
{}

void SwarmDroidAI::Refresh() {
	auto testFunction = 
		[this](Entity* in_entity) {
		return true;
	};

		
	CustomArray<Entity*> entitiesToAvoid (GEngine::Get().AI().Entities().List().Query(testFunction));

	Vector3d avoidanceVector = Calculate_Linear_Projectiles_Avoidance_Vector(
		parent->transform.position, parent->velocity, entitiesToAvoid, 
		parent->Mask()->tree->trunk->radius + 2.0, 2.0);

	if (!avoidanceVector.Is_Zero()) {
		chosenDirection = avoidanceVector;
		chosenSpeed = 40.0;
		firingTolerance = 1.0;
		firingRange = 20.0; 
		return;
	}

	bool changedState = false;
	while (changedState) {
		switch (state) {
		case inactive:
			aggressor = nullptr;
			targetEntity = nullptr;
			targetLocation = parent->transform.position;
			chosenDirection = parent->transform.orientation.forward;
			chosenSpeed = 0.0;
			firingTolerance = 0.0;
			firingRange = 0.0;
			break;
		case alert:
			if (Choose_Enemy()) {
				state = pursueEnemy;
				changedState = true;
				break;
			}
			targetLocation = parent->transform.position;
			chosenDirection = parent->transform.orientation.forward;
			chosenSpeed = 0.0;
			firingTolerance = 0.0;
			firingRange = 0.0;
			break;
		case pursueEnemy:
			if (!Confirm_Target()) {
				if (!Choose_Enemy()) {
					state = alert;
					changedState = true;
					break;
				}
			}
			//todo
			break;
		case firingRun:
			break;
		case followAlly:
			break;
		case goToLocation:
			break;
		default:
			throw MismatchException("SwarmDroid:Refresh:state not enum value");
		}
		changedState = false;
	}
}

bool SwarmDroidAI::Choose_Enemy() {
	Entity* bestChoice = nullptr;
	double bestRating = 1e12;
	Entity* thisEntity;
	double thisRating;

	auto testFunction = 
		[] (Entity* in_entity) -> bool {
		return in_entity->allegiance == Allegiance::human;
	};
	CustomArray<Entity*> candidates (GEngine::Get().AI().Entities().List().Query(testFunction));

	for (uint i = 0; i < candidates.Size(); i++) {
		thisEntity = candidates[i];
		thisRating = (float)(thisEntity->transform.position - parent->transform.position).Magnitude();
		if (thisEntity == aggressor) {
			aggressor = nullptr;
			thisRating *= 0.5f;
		}
		if ((thisEntity->transform.position - parent->transform.position).Dot(parent->transform.orientation.forward) < 0) {
			thisRating *= 1.2f;
		}
		if (thisRating < bestRating) {
			bestChoice = thisEntity;
			bestRating = thisRating;
		}
	}
	targetEntity = bestChoice;
	
	return targetEntity != nullptr;
}

bool SwarmDroidAI::Confirm_Target() {
	if (targetEntity == nullptr)
		return false;
	for (uint i = 0; i < GEngine::Get().AI().Entities().List().Size(); i++) {
		if (GEngine::Get().AI().Entities().List()[i] == targetEntity)
			return true;
	}
	return false;
}
/*
ShockDroidAI::ShockDroidAI(ShockDroid* parent) :
parent		(parent),
aggressor	(nullptr),
target		(nullptr),
tgtdir		(Vector3d(0, 0, -1)),
tgtspeed	(0.0f),
tgtloc		(parent->transform.position),
prevmove	(Rand_Float(0, 10)) {
	Choose_Target();
	if(target != nullptr)
		tgtloc = target->transform.position + Vector3d::Random_Normalized() * 50.0;
}

void ShockDroidAI::Refresh() {
	if (!Confirm_Target())
		Choose_Target();

	if (target == nullptr) {
		tgtdir = Vector3d(0, 0, -1);
		tgtspeed = 0.0;
		return;
	}

	if (GEngine::Get().Clock().Now() - prevmove > 10) {
		tgtloc = target->transform.position + Vector3d::Random_Normalized() * 50.0;
		prevmove += 10.0;
	}

	tgtdir = tgtloc - parent->transform.position;
	tgtspeed = 30.0;

	if (tgtdir.Magnitude() < 10.0)
		tgtspeed = 0.0;

	Vector3d avoid = Avoid_Entities(parent, nullptr, 0);

	if (!avoid.Is_Zero() && !parent->velocity.Is_Zero()) {
		tgtdir = avoid;
		tgtspeed = 20.0f;
	}

	tgtdir.Normalize();
}

void ShockDroidAI::Choose_Target() {
	Entity* beste = nullptr;
	float bestn = 1000.0;
	Entity* thise;
	float thisn;
	for (uint i = 0; i < GEngine::Get().AI().Entities().List().Size(); i++) {
		thise = GEngine::Get().AI().Entities().List()[i];
		if (thise->allegiance != Allegiance::human)
			continue;
		thisn = (float)(thise->transform.position - parent->transform.position).Magnitude();
		if (thise == aggressor) {
			aggressor = nullptr;
			thisn *= 0.5;
		}

		if (thisn < bestn) {
			beste = thise;
			bestn = thisn;
		}
	}
	target = beste;
}

bool ShockDroidAI::Confirm_Target() {
	if (target == nullptr)
		return false;
	if (aggressor != nullptr)
		return false;
	if ((target->transform.position - parent->transform.position).Dot(target->transform.position - parent->transform.position) > 1000000.0f)
		return false;
	for (uint i = 0; i < GEngine::Get().AI().Entities().List().Size(); i++) {
		if (GEngine::Get().AI().Entities().List()[i] == target)
			return true;
	}
	return false;
}

*/
