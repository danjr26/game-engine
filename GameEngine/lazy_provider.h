#ifndef LAZY_PROVIDER_H
#define LAZY_PROVIDER_H

#include <list>
#include <functional>
#include <mutex>

template<class T>
class LazyProvider {
private:
	struct Element {
		std::mutex mMutex;
		T mData;
	};

	std::function<T(void)> mCreator;
	std::function<void(T&)> mWasher;
	std::list<Element> mElements;

private:
	void wash(T& in_data) {
		if (mWasher) mWasher(in_data);
	}

	Element& create() {
		Element newElement = {
			std::mutex(),
			mCreator ? mCreator() : T()
		};
	}

public:
	LazyProvider(std::function<T(void)> in_creator = nullptr, std::function<void(T&)> in_washer = nullptr) :
		mCreator(in_creator),
		mWasher(in_washer),
		mElements()
	{}

	~LazyProvider()
	{}

	T& borrow() {
		// try existing ones
		for (auto it = mElements.begin(); it != mElements.end(); it++) {
			if (it->mMutex.try_lock()) {
				wash(it->mData);
				return it->mData;
			}
		}

		// else return new one
		return create().mData;
	}

	void release(T& in_element) {
		for (auto it = mElements.begin(); it != mElements.end(); it++) {
			if (&it->mData == &in_element) {
				it->mMutex.unlock();
				return;
			}
		}
	}
};

#endif