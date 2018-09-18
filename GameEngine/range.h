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
		if (in_nVals == 0) {
			low = 0;
			high = 0;
		}
		else {
			low = in_vals[0];
			high = in_vals[0];

			for (uint i = 1; i < in_nVals; i++) {
				if (in_vals[i] < low) {
					low = in_vals[i];
				}
				else if (in_vals[i] > high) {
					high = in_vals[i];
				}
			}
		}
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
	
	void Set_Values(T in_val1, T in_val2) {
		if (in_val1 > in_val2) {
			low = in_val2;
			high = in_val1;
		}
		else {
			low = in_val1;
			high = in_val2;
		}
	}

	bool Intersection(const Range& in_other, Range& out_result) {
		if (low >= in_other.Get_Low()) {
			if (low <= in_other.Get_High()) {
				out_result.Set_Values(low, Lesser_Of<T>(high, in_other.Get_High()));
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (high >= in_other.Get_Low()) {
				out_result.Set_Values(in_other.Get_Low(), Lesser_Of<T>(high, in_other.Get_High()));
				return true;
			}
			else {
				return false;
			}
		}
	}
};

using Rangef = Range<float>;
using Ranged = Range<double>;

#endif

