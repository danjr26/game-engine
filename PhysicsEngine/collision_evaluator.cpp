#include "collision_evaluator.h"
#include "collision_mask.h"

template<class T>
inline void CollisionEvaluator<T, 2>::returnPoint(bool in_val) {
	mReturnPoint = in_val;
}

template<class T>
inline CollisionEvaluator<T, 2>::CollisionEvaluator() :
	mReturnPoint(true),
	mReturnSeparator(false) {}

template<class T>
inline void CollisionEvaluator<T, 2>::returnSeparator(bool in_val) {
	mReturnSeparator = in_val;
}

template<class T>
Collision<T, 2> CollisionEvaluator<T, 2>::evaluate(CollisionMask<T, 2>& in_mask1, CollisionMask<T, 2>& in_mask2) {
	return in_mask1.acceptEvaluator(*this, in_mask2);
}

template<class T>
Collision<T, 3> CollisionEvaluator<T, 3>::evaluate(CollisionMask<T, 3>& in_mask1, CollisionMask<T, 3>& in_mask2) {
	return Collision<T, 3>();
	//return in_mask1.acceptEvaluator(*this, in_mask2);
}

template class CollisionEvaluator<float, 2>;
template class CollisionEvaluator<double, 2>;