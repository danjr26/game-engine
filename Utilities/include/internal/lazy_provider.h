#ifndef LAZY_PROVIDER_H
#define LAZY_PROVIDER_H

#include <forward_list>
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
	std::forward_list<Element> mElements;

private:
	void wash(T& i_data) {
		if (mWasher) mWasher(i_data);
	}

	Element& create() {
		Element newElement = {
			std::mutex(),
			mCreator ? mCreator() : T()
		};
	}

public:
	LazyProvider(std::function<T(void)> i_creator = nullptr, std::function<void(T&)> i_washer = nullptr) :
		mCreator(i_creator),
		mWasher(i_washer),
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

	void release(T& i_element) {
		for (auto it = mElements.begin(); it != mElements.end(); it++) {
			if (&it->mData == &i_element) {
				it->mMutex.unlock();
				return;
			}
		}
	}
};

#endif