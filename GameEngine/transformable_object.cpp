#include "transformable_object.h"

template<class T, uint n>
TransformableObject<T, n>::TransformableObject() :
	mTransform(nullptr) {

	mTransform.reset(new Transform<T, n>());
	mTransform.get_deleter().mNeuter = false;
}

template<class T, uint n>
TransformableObject<T, n>::TransformableObject(Transform<T, n>* in_transform) :
	mTransform(in_transform) {

	if (in_transform == nullptr) {
		mTransform.reset(new Transform<T, n>());
		mTransform.get_deleter().mNeuter = false;
	}
	else {
		mTransform.get_deleter().mNeuter = true;
	}
}

template<class T, uint n>
TransformableObject<T, n>::TransformableObject(const TransformableObject<T, n>& in_other) :
	mTransform(nullptr) {

	*this = in_other;
}

template<class T, uint n>
TransformableObject<T, n>& TransformableObject<T, n>::operator=(const TransformableObject<T, n>& in_other) {
	mTransform.get_deleter().mNeuter = in_other.mTransform.get_deleter().mNeuter;
	if (mTransform.get_deleter().mNeuter) {
		mTransform.reset(in_other.mTransform.get());
	}
	else {
		mTransform.reset(new Transform<T, n>(*in_other.mTransform));
	}
	return *this;
}

template<class T, uint n>
Transform<T, n>& TransformableObject<T, n>::getTransform() {
	return *mTransform;
}

template<class T, uint n>
Transform<T, n> const & TransformableObject<T, n>::getTransform() const {
	return *mTransform;
}

template<class T, uint n>
void TransformableObject<T, n>::subTransform(Transform<T, n>* in_transform) {
	if (mTransform.get() == in_transform) return;

	Transform<T, n>* temp = in_transform;
	if (temp == nullptr) temp = new Transform<T, n>();

	mTransform.reset(temp);
	mTransform.get_deleter().mNeuter = (in_transform != nullptr);
}

template class TransformableObject<float, 2>;
template class TransformableObject<double, 2>;
template class TransformableObject<float, 3>;
template class TransformableObject<double, 3>;
