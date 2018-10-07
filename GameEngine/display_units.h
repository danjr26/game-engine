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


template<uint n>
class DisplayUnits {
private:
	Vector<float , n> value;

private:
	DisplayUnits(Vector<float, n> in_value) :
		value(in_value)
	{}

public:
	Vector<float, n> Percent() {
		Vector<float, n> output = value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = value[i] * 100.0f;
		}
		return output;
	}

	Vector<float, n> Percent_X(Vector2i in_targetDimensions) {
		Vector<float, n> output = value;
		output[0] = output[0] * 100.0f;
		output[1] = output[1] * 100.0f * in_targetDimensions[0] / in_targetDimensions[1];
		return output;
	}

	Vector<float, n> Percent_Y(Vector2i in_targetDimensions) {
		Vector<float, n> output = value;
		output[0] = output[0] * 100.0f * in_targetDimensions[1] / in_targetDimensions[0];
		output[1] = output[1] * 100.0f;
		return output;
	}

	Vector<float, n> Pixels(Vector2i in_targetDimensions) {
		Vector<float, n> output = value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] * in_targetDimensions[i];
		}
		return output;
	}

	Vector<float, n> OpenGL_Position() {
		Vector<float, n> output = value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] * 2.0f - 1.0f;
		}
		return output;
	}

	Vector<float, n> OpenGL_Displacement() {
		Vector<float, n> output = value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] * 2.0f;
		}
		return output;
	}

	static DisplayUnits Percent(Vector<float, n> in_value) {
		Vector<float, n> output = in_value;
		output[0] = output[0] / 100.0f;
		output[1] = output[1] / 100.0f;
		return output;
	}

	static DisplayUnits Percent_X(Vector<float, n> in_value, Vector2i in_targetDimensions) {
		Vector<float, n> output = in_value;
		output[0] = output[0] / 100.0f;
		output[1] = output[1] / 100.0f / in_targetDimensions[0] * in_targetDimensions[1];
		return output;
	}

	static DisplayUnits Percent_Y(Vector<float, n> in_value, Vector2i in_targetDimensions) {
		Vector<float, n> output = in_value;
		output[0] = output[0] / 100.0f / in_targetDimensions[1] * in_targetDimensions[0];
		output[1] = output[1] / 100.0f;
		return output;
	}

	static DisplayUnits Pixels(Vector<float, n> in_value, Vector2i in_targetDimensions) {
		Vector<float, n> output = in_value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] / in_targetDimensions[i];
		}
		return output;
	}

	static DisplayUnits OpenGL_Position(Vector<float, n> in_value) {
		Vector<float, n> output = in_value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = (output[i] + 1.0f) / 2.0f;
		}
		return output;
	}

	static DisplayUnits OpenGL_Displacement(Vector<float, n> in_value) {
		Vector<float, n> output = in_value;
		for (uint i = 0; i < Min(n, 2U); i++) {
			output[i] = output[i] / 2.0f;
		}
		return output;
	}
};

using DisplayUnits1 = DisplayUnits<1>;
using DisplayUnits2 = DisplayUnits<2>;
using DisplayUnits3 = DisplayUnits<3>;

#endif