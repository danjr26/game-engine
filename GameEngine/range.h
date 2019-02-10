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
		Set_Values(in_val1, in_val2);
	}

	Range(uint in_nVals, const T* in_vals) {
		Min_Max(in_vals, in_nVals, mLow, mHigh);
	}

	Range(const std::initializer_list<T>& in_vals) {
		Min_Max(in_vals, mLow, mHigh);
	}

	Range(const std::vector<T>& in_vals) {
		Min_Max(in_vals, mLow, mHigh);
	}

	bool operator==(const Range<T>& in_other) {
		return mLow == in_other.mLow && mHigh == in_other.mHigh;
	}

	T Get_Low() const {
		return mLow;
	};

	T Get_High() const {
		return mHigh;
	};

	void Set_Low(T in_value) {
		mLow = in_value;
		mHigh = Max(mHigh, in_value);
	}

	void Set_High(T in_value) {
		mHigh = in_value;
		mLow = Min(mLow, in_value);
	}

	T Get_Span() const {
		return mHigh - mLow;
	};

	T Get_Mean() const {
		return (mHigh + mLow) / 2;
	}

	void Expand_To(T in_val) {
		mHigh = Max(mHigh, in_val);
		mLow = Min(mLow, in_val);
	}
	
	void Set_Values(T in_val1, T in_val2) {
		Min_Max({ in_val1, in_val2 }, mLow, mHigh);
	}

	bool Contains_Inc(T in_val) {
		return in_val >= mLow && in_val <= mHigh;
	}

	bool Contains_Exc(T in_val) {
		return in_val > mLow && in_val < mHigh;
	}

	bool Intersection(const Range& in_other, Range& out_result) {
		if (mLow >= in_other.Get_Low()) {
			if (mLow <= in_other.Get_High()) {
				out_result.Set_Values(mLow, Min<T>(mHigh, in_other.Get_High()));
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (mHigh >= in_other.Get_Low()) {
				out_result.Set_Values(in_other.Get_Low(), Min<T>(mHigh, in_other.Get_High()));
				return true;
			}
			else {
				return false;
			}
		}
	}

	bool Intersection(const Range& in_other) {
		if (mLow >= in_other.Get_Low()) {
			return mLow <= in_other.Get_High();
		}
		else {
			return mHigh >= in_other.Get_Low();
		}
	}
};

using Rangeui = Range<uint>;
using Rangei = Range<int>;
using Rangef = Range<float>;
using Ranged = Range<double>;

#endif

