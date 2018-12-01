#ifndef RANGE_H
#define RANGE_H

#include "misc.h"
#include "definitions.h"

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

	Range(T in_val) :
		low(in_val),
		high(in_val)
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

	bool operator==(const Range<T>& in_other) {
		return low == in_other.low && high == in_other.high;
	}

	T Get_Low() const {
		return low;
	};

	T Get_High() const {
		return high;
	};

	void Set_Low(T in_value) {
		low = in_value;
		high = Max(high, in_value);
	}

	void Set_High(T in_value) {
		high = in_value;
		low = Min(low, in_value);
	}

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

	bool Contains_Inc(T in_val) {
		return in_val >= low && in_val <= high;
	}

	bool Contains_Exc(T in_val) {
		return in_val > low && in_val < high;
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

using Rangeui = Range<uint>;
using Rangei = Range<int>;
using Rangef = Range<float>;
using Ranged = Range<double>;

#endif

