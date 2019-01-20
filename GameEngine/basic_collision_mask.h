#ifndef BASIC_COLLISION_MASK_H
#define BASIC_COLLISION_MASK_H

#include "collision_mask.h"
#include "misc.h"

template<class Basis, class T, uint n>
class BasicCollisionMask
{};

template<class Basis, class T>
class BasicCollisionMask<Basis, T, 2> : public CollisionMask<T, 2> {
public:
	using basis_t = Basis;
	using neuterable_ptr_t = neuterable_ptr<basis_t>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

private:
	basis_t basis;

public:
	BasicCollisionMask(const basis_t& in_basis, bool in_ignoreTransform = false);
	basis_t& Get_Basis();
	const basis_t& Get_Basis() const;
	neuterable_ptr_t Get_Transformed_Basis() const;
	void Apply_Transform() override;
	BasicCollisionMask<Basis, T, 2>* Clone() const override;
	virtual Vector<T, 2> Get_Closest_Point(const Vector<T, 2>& in_point) const override = 0;
	virtual Vector<T, 2> Get_Closest_Normal(const Vector<T, 2>& in_point, pnp_t in_policy = towards_point) const override = 0;
};

#endif

template<class Basis, class T>
BasicCollisionMask<Basis, T, 2>::BasicCollisionMask(const basis_t& in_basis, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	basis(in_basis) 
{}

template<class Basis, class T>
Basis& BasicCollisionMask<Basis, T, 2>::Get_Basis() {
	return basis;
}

template<class Basis, class T>
const Basis& BasicCollisionMask<Basis, T, 2>::Get_Basis() const {
	return basis;
}

template<class Basis, class T>
neuterable_ptr<Basis> BasicCollisionMask<Basis, T, 2>::Get_Transformed_Basis() const {
	if (ignoreTransform) {
		neuterable_ptr_t out(&basis);
		out.neuter = true;
		return out;
	}
	else {
		neuterable_ptr_t out(new basis_t(basis));
		out->Apply_Transform(transform);
		return out;
	}
}

template<class Basis, class T>
void BasicCollisionMask<Basis, T, 2>::Apply_Transform() {
	basis.Apply_Transform(transform);
}

template<class Basis, class T>
BasicCollisionMask<Basis, T, 2>* BasicCollisionMask<Basis, T, 2>::Clone() const {
	return new BasicCollisionMask<Basis, T, 2>(*this);
}

