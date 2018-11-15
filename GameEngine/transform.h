#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vector.h"
#include "matrix.h"
#include "unclamped_rotation.h"
#include "rotation.h"

template<class T, uint n>
class Transform {
	friend class Transform;
	// applies components in this order:
	// scale, rotation, translation
private:
	Transform<T, n>* parent;
	Vector<T, n> translation;
	Vector<T, n> scale;
	Rotation<T, n> rotation;

public:
	Transform();
	template<class T2, std::enable_if_t<!std::is_same_v<T, T2>>>
	Transform(const Transform<T2, n>& in_other) : 
		parent(nullptr),
		translation(in_other.translation),
		scale(in_other.scale),
		rotation(in_other.rotation)
	{}

	Transform<T, n>* Get_Parent();
	Transform<T, n> const* Get_Const_Parent() const;
	void Set_Parent(Transform<T, n>* in_parent);

	bool Is_Local_Identity() const;
	bool Is_World_Identity() const;

	Rotation<T, n> Get_Local_Rotation() const;
	Rotation<T, n> Get_World_Rotation() const;
	void Set_Local_Rotation(const Rotation<T, n>& in_rotation);
	void Set_World_Rotation(const Rotation<T, n>& in_rotation);
	void Rotate_Local(const Rotation<T, n>& in_rotation);
	void Rotate_World(const Rotation<T, n>& in_rotation);
	void Rotate_Local_Around_Local_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point);
	void Rotate_World_Around_Local_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point);
	void Rotate_Local_Around_World_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point);
	void Rotate_World_Around_World_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point);

	Vector<T, n> Get_Local_Scale() const;
	void Set_Local_Scale(const Vector<T, n>& in_factor);
	void Scale_Local(const Vector<T, n>& in_factor);

	Vector<T, n> Get_Local_Position() const;
	Vector<T, n> Get_World_Position() const;
	void Set_Local_Position(const Vector<T, n>& in_position);
	void Translate_Local(const Vector<T, n>& in_translation);
	void Translate_World(const Vector<T, n>& in_translation);

	Vector<T, n> Apply_To_Local_Point(const Vector<T, n>& in_point) const;
	Vector<T, n> Apply_To_World_Point(const Vector<T, n>& in_point) const;
	Vector<T, n> Apply_To_Local_Direction(const Vector<T, n>& in_direction) const;
	Vector<T, n> Apply_To_World_Direction(const Vector<T, n>& in_direction) const;
	Vector<T, n> Apply_To_Local_Vector(const Vector<T, n>& in_vector) const;
	Vector<T, n> Apply_To_World_Vector(const Vector<T, n>& in_vector) const;

	Matrix<T, 4, 4> Get_Local_Matrix() const;
	Matrix<T, 4, 4> Get_World_Matrix() const;
	Matrix<T, 4, 4> Get_Local_Inverse_Matrix() const;
	Matrix<T, 4, 4> Get_World_Inverse_Matrix() const;

	Vector<T, n> World_To_Local_Point(const Vector<T, n>& in_point) const;
	Vector<T, n> Local_To_World_Point(const Vector<T, n>& in_point) const;
	Vector<T, n> World_To_Local_Direction(const Vector<T, n>& in_direction) const;
	Vector<T, n> Local_To_World_Direction(const Vector<T, n>& in_direction) const;
	Vector<T, n> World_To_Local_Vector(const Vector<T, n>& in_vector) const;
	Vector<T, n> Local_To_World_Vector(const Vector<T, n>& in_vector) const;
	Rotation<T, n> World_To_Local_Rotation(const Rotation<T, n>& in_rotation) const;
	Rotation<T, n> Local_To_World_Rotation(const Rotation<T, n>& in_rotation) const;
};

using Transform2f = Transform<float, 2>;
using Transform2d = Transform<double, 2>;
using Transform3f = Transform<float, 3>;
using Transform3d = Transform<double, 3>;

#endif

