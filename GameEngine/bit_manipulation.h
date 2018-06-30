#ifndef BIT_MANIPULATION_H
#define BIT_MANIPULATION_H

#include "definitions.h"

template<class T>
void Set_Bit(T& inout_field, T in_bit, bool in_value) {
	inout_field &= ~in_bit;
	if (in_value) {
		inout_field |= in_bit;
	}
}

#endif