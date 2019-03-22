#include "deep_object.h"

template<class T, uint n>
DeepObject<T, n>::DeepObject() :
	mDepthTransform(nullptr) {

	mDepthTransform.reset(new DepthTransform<T, n>());
	mDepthTransform.get_deleter().mNeuter = false;
}

template<class T, uint n>
DeepObject<T, n>::DeepObject(DepthTransform<T, n>* in_depthTransform) :
	mDepthTransform(in_depthTransform) {

	if (in_depthTransform == nullptr) {
		mDepthTransform.reset(new DepthTransform<T, n>());
		mDepthTransform.get_deleter().mNeuter = false;
	}
	else {
		mDepthTransform.get_deleter().mNeuter = true;
	}
}

template<class T, uint n>
DepthTransform<T, n>& DeepObject<T, n>::getDepthTransform() {
	return  *mDepthTransform;
}

template<class T, uint n>
DepthTransform<T, n> const& DeepObject<T, n>::getDepthTransform() const {
	return *mDepthTransform;
}

template<class T, uint n>
void DeepObject<T, n>::subDepthTransform(DepthTransform<T, n>* in_depthTransform) {
	if (mDepthTransform.get() == in_depthTransform) return;

	DepthTransform<T, n>* temp = in_depthTransform;
	if (temp == nullptr) temp = new DepthTransform<T, n>();

	mDepthTransform.reset(temp);
	mDepthTransform.get_deleter().mNeuter = (in_depthTransform != nullptr);
}

template class DeepObject<float, 2>;
template class DeepObject<double, 2>;
