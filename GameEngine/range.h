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

	Range(T in_val) :
		mLow(in_val),
		mHigh(in_val)
	{}

	Range(T in_val1, T in_val2) {
		setValues(in_val1, in_val2);
	}

	Range(uint in_nVals, const T* in_vals) {
		GEUtil::minMax(in_vals, in_nVals, mLow, mHigh);
	}

	Range(const std::initializer_list<T>& in_vals) {
		GEUtil::minMax(in_vals, mLow, mHigh);
	}

	Range(const std::vector<T>& in_vals) {
		GEUtil::minMax(in_vals, mLow, mHigh);
	}

	bool operator==(const Range<T>& in_other) {
		return mLow == in_other.mLow && mHigh == in_other.mHigh;
	}

	T getLow() const {
		return mLow;
	};

	T getHigh() const {
		return mHigh;
	};

	void setLow(T in_value) {
		mLow = in_value;
		mHigh = GEUtil::max(mHigh, in_value);
	}

	void setHigh(T in_value) {
		mHigh = in_value;
		mLow = GEUtil::min(mLow, in_value);
	}

	T getSpan() const {
		return mHigh - mLow;
	};

	T getMean() const {
		return (mHigh + mLow) / 2;
	}

	void expandTo(T in_val) {
		mHigh = GEUtil::max(mHigh, in_val);
		mLow = GEUtil::min(mLow, in_val);
	}
	
	void setValues(T in_val1, T in_val2) {
		GEUtil::minMax({ in_val1, in_val2 }, mLow, mHigh);
	}

	bool containsInc(T in_val) {
		return in_val >= mLow && in_val <= mHigh;
	}

	bool containsExc(T in_val) {
		return in_val > mLow && in_val < mHigh;
	}

	bool intersection(const Range& in_other, Range& out_result) {
		if (mLow >= in_other.getLow()) {
			if (mLow <= in_other.getHigh()) {
				out_result.setValues(mLow, GEUtil::min<T>(mHigh, in_other.getHigh()));
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (mHigh >= in_other.getLow()) {
				out_result.setValues(in_other.getLow(), GEUtil::min<T>(mHigh, in_other.getHigh()));
				return true;
			}
			else {
				return false;
			}
		}
	}

	bool intersection(const Range& in_other) {
		if (mLow >= in_other.getLow()) {
			return mLow <= in_other.getHigh();
		}
		else {
			return mHigh >= in_other.getLow();
		}
	}
};

using Rangeui = Range<uint>;
using Rangei = Range<int>;
using Rangef = Range<float>;
using Ranged = Range<double>;

#endif

