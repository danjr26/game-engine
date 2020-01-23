#include "collider.h"
#include "collision_evaluation.h"

// interface
template<class T, uint n>
Collider<T, n>::Collider(bool i_asIs) :
	mAsIs(i_asIs),
	mQueue(nullptr) {}

template<class T, uint n>
bool Collider<T, n>::getAsIs() const {
	return mAsIs;
}

template<class T, uint n>
void Collider<T, n>::setAsIs(bool i_value) {
	mAsIs = i_value;
}

template<class T, uint n>
void Collider<T, n>::setQueue(CollisionQueue<T, n>* i_queue) {
	mQueue = i_queue;
}

template<class T, uint n>
uint64_t Collider<T, n>::getFilters() const {
	return mFilters;
}

template<class T, uint n>
void Collider<T, n>::setFilters(uint64_t i_filters) {
	mFilters = i_filters;
}

template<class T, uint n>
CollisionQueue<T, n>* Collider<T, n>::getQueue() {
	return mQueue;
}

template<class T, uint n, class Basis>
_Collider<T, n, Basis>::_Collider(const basis_t& i_basis, bool i_asIs) :
	mBasis(i_basis),
	mQueue(nullptr),
	mAsIs(i_asIs)
{}

// implementation
template<class T, uint n, class Basis>
_Collider<T, n, Basis>::~_Collider() 
{}

template<class T, uint n, class Basis>
Basis& _Collider<T, n, Basis>::getBasis() {
	return mBasis;
}

template<class T, uint n, class Basis>
const Basis& _Collider<T, n, Basis>::getBasis() const {
	return mBasis;
}

template<class T, uint n, class Basis>
_Collider<T, n, Basis>::neuterable_ptr_t _Collider<T, n, Basis>::getTransformedBasis() {
	if (mAsIs) {
		neuterable_ptr_t out = neuterable_ptr_t(&mBasis);
		out.get_deleter().mNeuter = true;
		return out;
	}
	else {
		basis_t* ptr = new basis_t(mBasis);
		neuterable_ptr_t out = neuterable_ptr_t(ptr);
		ptr->applyTransform(getTransform());
		return out;
	}
}

template<class T, uint n, class Basis>
_Collider<T, n, Basis>::neuterable_cptr_t _Collider<T, n, Basis>::getTransformedBasis() const {
	if (mAsIs) {
		neuterable_cptr_t out = neuterable_ptr_t(&mBasis);
		out.get_deleter().mNeuter = true;
		return out;
	}
	else {
		basis_t* ptr = new basis_t(mBasis);
		neuterable_cptr_t out = neuterable_ptr_t(ptr);
		ptr->applyTransform(getTransform());
		return out;
	}
}

template<class T, uint n, class Basis>
void _Collider<T, n, Basis>::applyTransform() {
	mBasis.applyTransform(getTransform());
}

template<class T, uint n, class Basis>
Collider<T, n>* _Collider<T, n, Basis>::clone() const {
	return new _Collider<Basis, T, n>(*this);
}

template<class T, uint n, class Basis>
void _Collider<T, n, Basis>::getClosest(ClosestRequest<T, n>& in_request) const {}

template<class T, uint n, class Basis>
void _Collider<T, n, Basis>::_accept1(Collider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) {
	i_other._accept2(*this, o_collision, i_flags);
}

template<class T, uint n, class Basis>
void _Collider<T, n, Basis>::_accept2(AAHalfSpaceCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) {
	
}


