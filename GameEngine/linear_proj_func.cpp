#include "linear_proj_func.h"

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
			thisEntity->Mask()-> + in_avoidanceRadius);

		Vector3d projectedEntityPosition = thisEntity->transform.position + thisEntity->velocity * projectedCollisionTime;

		if (projectedCollisionTime > 0.0 && projectedCollisionTime < in_maxProjectionTime) {
			avoidanceVector += (in_objectPosition - projectedEntityPosition).Normalized() * (in_maxProjectionTime - projectedCollisionTime);
		}
	}

	avoidanceVector.Normalize();

	return avoidanceVector;
}