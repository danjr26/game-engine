#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vector.h"
#include "matrix.h"
#include "rotation.h"

class Transform {
	// applies components in this order:
	// scale, rotation, translation
private:
	Transform* parent;
	Vector3d translation;
	Vector3d scale;
	Rotationd rotation;

public:
	Transform();

	Transform* Get_Parent();
	void Set_Parent(Transform* in_parent);

	Rotationd Get_Local_Rotation();
	Rotationd Get_World_Rotation();
	void Set_Local_Rotation(const Rotationd& in_rotation);
	void Set_World_Rotation(const Rotationd& in_rotation);
	void Rotate_Local(const Rotationd& in_rotation);
	void Rotate_World(const Rotationd& in_rotation);
	void Rotate_Local_Around_Local_Point(const Rotationd& in_rotation, const Vector3d& in_point);
	void Rotate_World_Around_Local_Point(const Rotationd& in_rotation, const Vector3d& in_point);
	void Rotate_Local_Around_World_Point(const Rotationd& in_rotation, const Vector3d& in_point);
	void Rotate_World_Around_World_Point(const Rotationd& in_rotation, const Vector3d& in_point);

	Vector3d Get_Local_Scale() const;
	void Set_Local_Scale(const Vector3d& in_factor);
	void Scale_Local(const Vector3d& in_factor);

	Vector3d Get_Local_Position() const;
	Vector3d Get_World_Position() const;
	void Set_Local_Position(const Vector3d& in_position);
	void Translate_Local(const Vector3d& in_translation);
	void Translate_World(const Vector3d& in_translation);

	Vector3d Apply_To_Local_Point(const Vector3d& in_point) const;
	Vector3d Apply_To_World_Point(const Vector3d& in_point) const;
	Vector3d Apply_To_Local_Direction(const Vector3d& in_direction) const;
	Vector3d Apply_To_World_Direction(const Vector3d& in_direction) const;
	Vector3d Apply_To_Local_Vector(const Vector3d& in_vector) const;
	Vector3d Apply_To_World_Vector(const Vector3d& in_vector) const;

	Matrix4d Get_Local_Matrix() const;
	Matrix4d Get_World_Matrix() const;
	Matrix4d Get_Local_Inverse_Matrix() const;
	Matrix4d Get_World_Inverse_Matrix() const;

private:
	Vector3d World_To_Local_Point(const Vector3d& in_point) const;
	Vector3d Local_To_World_Point(const Vector3d& in_point) const;
	Vector3d World_To_Local_Direction(const Vector3d& in_direction) const;
	Vector3d Local_To_World_Direction(const Vector3d& in_direction) const;
	Vector3d World_To_Local_Vector(const Vector3d& in_vector) const;
	Vector3d Local_To_World_Vector(const Vector3d& in_vector) const;
	Rotationd World_To_Local_Rotation(const Rotationd& in_rotation) const;
	Rotationd Local_To_World_Rotation(const Rotationd& in_rotation) const;
};



#endif

