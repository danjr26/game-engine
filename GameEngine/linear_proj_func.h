#ifndef LINEAR_PROJ_FUNC_H
#define LINEAR_PROJ_FUNC_H

#include "vector.h"

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

#endif