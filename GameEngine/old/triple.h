#ifndef TRIPLE_H
#define TRIPLE_H

template<class T>
struct Triple {
	T values[3];

	Triple() :
		values()
	{};

	Triple(T const& in_value1, T const& in_value2, T const& in_value3) :
		values() {
		values[0] = in_value1;
		values[1] = in_value2;
		values[2] = in_value3;
	}

	Triple(const T* in_values) :
		values() {
		if (in_values == nullptr) {
			throw NullPointerException("Triple:Triple:in_values");
		}
		values[0] = in_values[0];
		values[1] = in_values[1];
		values[2] = in_values[2];
	}

	T& operator[] (uint in_index) const {
		if (in_index >= 3) {
			throw InvalidIndexException(in_index, 3, "Triple:operator[]:values");
		}
		return values[in_index];
	}
};

#endif TRIPLE_H
