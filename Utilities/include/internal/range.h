#ifndef RANGE_H
#define RANGE_H

#include "misc.h"
#include "definitions.h"

template<class T>
class Range {
private:
	T mLow;
	T mHigh;

public:
	Range() :
		mLow(0),
		mHigh(0)
	{}

	Range(T i_val) :
		mLow(i_val),
		mHigh(i_val)
	{}

	Range(T i_val1, T i_val2) {
		setValues(i_val1, i_val2);
	}

	template<class InputIt>
	Range(InputIt i_begin, InputIt i_end) {
		auto result = std::minmax_element(i_begin, i_end);
		mLow = result.first;
		mHigh = result.second;
	}

	Range(const std::initializer_list<T>& i_vals) {
		auto result = std::minmax(i_vals);
		mLow = result.first;
		mHigh = result.second;
	}

	bool operator==(const Range<T>& i_other) {
		return mLow == i_other.mLow && mHigh == i_other.mHigh;
	}

	T getLow() const {
		return mLow;
	};

	T getHigh() const {
		return mHigh;
	};

	void setLow(T i_value) {
		mLow = i_value;
		mHigh = std::max(mHigh, i_value);
	}

	void setHigh(T i_value) {
		mHigh = i_value;
		mLow = std::min(mLow, i_value);
	}

	T getSpan() const {
		return mHigh - mLow;
	};

	T getMean() const {
		return (mHigh + mLow) / 2;
	}

	void expandTo(T i_val) {
		mHigh = std::max(mHigh, i_val);
		mLow = std::min(mLow, i_val);
	}
	
	void setValues(T i_val1, T i_val2) {
		auto result = std::minmax(i_val1, i_val2);
		mLow = result.first;
		mHigh = result.second;
	}

	bool containsInc(T i_val) {
		return i_val >= mLow && i_val <= mHigh;
	}

	bool containsExc(T i_val) {
		return i_val > mLow && i_val < mHigh;
	}

	bool intersect(const Range& i_other, Range& o_result) {
		if (mLow >= i_other.getLow()) {
			if (mLow <= i_other.getHigh()) {
				o_result.setValues(mLow, std::min(mHigh, i_other.getHigh()));
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (mHigh >= i_other.getLow()) {
				o_result.setValues(i_other.getLow(), std::min<T>(mHigh, i_other.getHigh()));
				return true;
			}
			else {
				return false;
			}
		}
	}

	bool intersect(const Range& i_other) {
		if (mLow >= i_other.getLow()) {
			return mLow <= i_other.getHigh();
		}
		else {
			return mHigh >= i_other.getLow();
		}
	}
};

using Rangeui = Range<uint>;
using Rangei = Range<int>;
using Rangef = Range<float>;
using Ranged = Range<double>;

#endif

