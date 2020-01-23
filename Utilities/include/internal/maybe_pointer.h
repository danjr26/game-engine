#ifndef MAYBE_POINTER_H
#define MAYBE_POINTER_H

#include "misc.h"

template<class T>
class MaybePointer {
	T* mPointer;
	bool mDelete;

public:
	MaybePointer() : 
	mPointer(nullptr), 
	mDelete(false) 
	{}

	MaybePointer(T* i_pointer, bool i_delete) :
	mPointer(i_pointer),
	mDelete(i_delete)
	{}

	MaybePointer(const MaybePointer& i_other) :
	mPointer(i_other.mPointer),
	mDelete(i_other.mDelete) {
		if (mDelete) fail();
	}

	MaybePointer(MaybePointer&& i_other) :
	mPointer(i_other.mPointer),
	mDelete(i_other.mDelete) {
		if (mDelete) {
			i_other.mPointer = nullptr;
			i_other.mDelete = false;
		}
	}

	~MaybePointer() {
		if (mDelete) delete mPointer;
	}

	operator bool() {
		return (bool)mPointer;
	}

	T& operator*() {
		return *mPointer;
	}

	const T& operator*() const {
		return *mPointer;
	}

	T* operator->() {
		return mPointer;
	}

	void sub(T* i_pointer, bool i_delete) {
		if (mDelete) delete mPointer;
		mPointer = i_pointer;
		mDelete = i_delete;
	}
};

#endif