#ifndef DISPLAY_UNITS_H
#define DISPLAY_UNITS_H

#include "render_target.h"
#include "definitions.h"
#include "misc.h"

/*
template<uint n>
struct DisplayDistance {
	Vector<double, n> value;
	enum Units {
		percent,
		percent_x,
		percent_y,
		pixels,
		opengl
	} units;

	DisplayDistance(Vector<double, n> in_value, Units in_units) :
		value(in_value),
		units(in_units)
	{}

	Vector<double, n> As_Displacement(const RenderTarget& in_target) {
		Vector<double, n> output;
		Vector2i dimensions = in_target.Get_Dimensions();
		switch (units) {
		case percent:
			for (uint i = 0; i < Min(n, 2U); i++) {
				output[i] = value[i] / 100.0 * 2.0;
			}
			break;
		case percent_x:
			for (uint i = 0; i < Min(n, 2U); i++) {
				output[i] = value[i] * dimensions[i] / dimensions[0] / 100.0 * 2.0;
			}
			break;
		case percent_y:
			for (uint i = 0; i < Min(n, 2U); i++) {
				output[i] = value[i] * dimensions[i] / dimensions[1] / 100.0 * 2.0;
			}
			break;
		case pixels:
			for (uint i = 0; i < Min(n, 2U); i++) {
				output[i] = value[i] / dimensions[i] * 2.0f;
			}
			break;
		}

		return output;
	}

	Vector<double, n> As_Position(const RenderTarget& in_target) {
		Vector<double, n> output = As_Displacement(in_target);
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] -= 1.0;
		}

		return output;
	}
};

using DisplayDistance1 = DisplayDistance<1>;
using DisplayDistance2 = DisplayDistance<2>;
using DisplayDistance3 = DisplayDistance<3>;
*/


template<class T, uint n>
class DisplayUnits {
private:
	Vector<T, n> mValue;

private:
	DisplayUnits(Vector<T, n> in_value) :
		mValue(in_value)
	{}

public:
	Vector<T, n> Percent() {
		Vector<T, n> output = mValue;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = mValue[i] * 100.0f;
		}
		return output;
	}

	Vector<T, n> Percent_X(Vector2i in_targetDimensions) {
		Vector<T, n> output = mValue;
		output[0] = output[0] * 100.0f;
		output[1] = output[1] * 100.0f * in_targetDimensions[0] / in_targetDimensions[1];
		return output;
	}

	Vector<T, n> Percent_Y(Vector2i in_targetDimensions) {
		Vector<T, n> output = mValue;
		output[0] = output[0] * 100.0f * in_targetDimensions[1] / in_targetDimensions[0];
		output[1] = output[1] * 100.0f;
		return output;
	}

	Vector<T, n> Pixels(Vector2i in_targetDimensions) {
		Vector<T, n> output = mValue;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] * in_targetDimensions[i];
		}
		return output;
	}

	Vector<T, n> OpenGL_Position() {
		Vector<T, n> output = mValue;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] * 2.0f - 1.0f;
		}
		return output;
	}

	Vector<T, n> OpenGL_Displacement() {
		Vector<T, n> output = mValue;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] * 2.0f;
		}
		return output;
	}

	static DisplayUnits Percent(Vector<T, n> in_value) {
		Vector<T, n> output = in_value;
		output[0] = output[0] / 100.0f;
		output[1] = output[1] / 100.0f;
		return output;
	}

	static DisplayUnits Percent_X(Vector<T, n> in_value, Vector2i in_targetDimensions) {
		Vector<T, n> output = in_value;
		output[0] = output[0] / 100.0f;
		output[1] = output[1] / 100.0f / in_targetDimensions[0] * in_targetDimensions[1];
		return output;
	}

	static DisplayUnits Percent_Y(Vector<T, n> in_value, Vector2i in_targetDimensions) {
		Vector<T, n> output = in_value;
		output[0] = output[0] / 100.0f / in_targetDimensions[1] * in_targetDimensions[0];
		output[1] = output[1] / 100.0f;
		return output;
	}

	static DisplayUnits Pixels(Vector<T, n> in_value, Vector2i in_targetDimensions) {
		Vector<T, n> output = in_value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] / in_targetDimensions[i];
		}
		return output;
	}

	static DisplayUnits OpenGL_Position(Vector<T, n> in_value) {
		Vector<T, n> output = in_value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = (output[i] + 1.0f) / 2.0f;
		}
		return output;
	}

	static DisplayUnits OpenGL_Displacement(Vector<T, n> in_value) {
		Vector<T, n> output = in_value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] / 2.0f;
		}
		return output;
	}
};

using DisplayUnits1f = DisplayUnits<float, 1>;
using DisplayUnits1d = DisplayUnits<double, 1>;
using DisplayUnits2f = DisplayUnits<float, 2>;
using DisplayUnits2d = DisplayUnits<double, 2>;
using DisplayUnits3f = DisplayUnits<float, 3>;
using DisplayUnits3d = DisplayUnits<double, 3>;

#endif