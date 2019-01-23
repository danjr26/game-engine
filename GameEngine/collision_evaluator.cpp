#include "collision_evaluator.h"
#include "collision_mask.h"

template<class T>
inline void CollisionEvaluator<T, 2>::Return_Point(bool in_val) {
	returnPoint = in_val;
}

template<class T>
inline CollisionEvaluator<T, 2>::CollisionEvaluator() :
	returnPoint(true),
	returnSeparator(false) {}

template<class T>
inline void CollisionEvaluator<T, 2>::Return_Separator(bool in_val) {
	returnSeparator = in_val;
}

template<class T>
Collision<T, 2> CollisionEvaluator<T, 2>::Evaluate(CollisionMask<T, 2>& in_mask1, CollisionMask<T, 2>& in_mask2) {
	return in_mask1.Accept_Evaluator(*this, in_mask2);
}

template<class T>
Collision<T, 3> CollisionEvaluator<T, 3>::Evaluate(CollisionMask<T, 3>& in_mask1, CollisionMask<T, 3>& in_mask2) {
	return Collision<T, 3>();
	//return in_mask1.Accept_Evaluator(*this, in_mask2);
}

template class CollisionEvaluator<float, 2>;
template class CollisionEvaluator<double, 2>;