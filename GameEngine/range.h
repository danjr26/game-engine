#ifndef RANGE_H
#define RANGE_H

#include "misc.h"

template<class T>
class Range {
private:
	T low;
	T high;

public:
	Range() :
		low(0),
		high(0)
	{}

	Range(T in_val1, T in_val2) {
		Set_Values(in_val1, in_val2);
	}

	Range(uint in_nVals, const T* in_vals) {
		Min_Max(in_vals, in_nVals, low, high);
	}

	Range(const std::initializer_list<T>& in_vals) {
		Min_Max(in_vals, low, high);
	}

	Range(const std::vector<T>& in_vals) {
		Min_Max(in_vals, low, high);
	}

	T Get_Low() const {
		return low;
	};

	T Get_High() const {
		return high;
	};

	T Get_Span() const {
		return high - low;
	};

	T Get_Mean() const {
		return (high + low) / 2;
	}

	void Expand_To(T in_val) {
		high = Max(high, in_val);
		low = Min(low, in_val);
	}
	
	void Set_Values(T in_val1, T in_val2) {
		Min_Max({ in_val1, in_val2 }, low, high);
	}

	bool Intersection(const Range& in_other, Range& out_result) {
		if (low >= in_other.Get_Low()) {
			if (low <= in_other.Get_High()) {
				out_result.Set_Values(low, Min<T>(high, in_other.Get_High()));
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (high >= in_other.Get_Low()) {
				out_result.Set_Values(in_other.Get_Low(), Min<T>(high, in_other.Get_High()));
				return true;
			}
			else {
				return false;
			}
		}
	}

	bool Intersection(const Range& in_other) {
		if (low >= in_other.Get_Low()) {
			return low <= in_other.Get_High();
		}
		else {
			return high >= in_other.Get_Low();
		}
	}
};

using Rangef = Range<float>;
using Ranged = Range<double>;

#endif

