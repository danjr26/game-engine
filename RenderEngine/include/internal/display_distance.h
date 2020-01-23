#ifndef RenderTarget_DISTANCE_H
#define RenderTarget_DISTANCE_H

#include "../../../Utilities/include/utilities.h"

enum class RenderTargetDistanceType {
	decimal_xy,
	decimal_x,
	decimal_y,
	pixels,
	gl_point,
	gl_vector
};

template<class T, uint n, typename = typename std::enable_if_t<(n >= 2)>>
struct RenderTargetDistance {
	RenderTargetDistanceType mType;
	Vector<T, n> mValue;

	RenderTargetDistance<T, n> convert(RenderTargetDistanceType i_type, Vector2ui i_dimen) {
		RenderTargetDistance<T, n> out;
		out.mType = i_type;
		out.mValue = mValue;

		switch (i_from.mType) {
		case RenderTargetDistanceType::decimal_xy:
			break;
		case RenderTargetDistanceType::decimal_x:
			out.mValue.y() *= i_dimen.y() / i_dimen.x();
			break;
		case RenderTargetDistanceType::decimal_y:
			out.mValue.x() *= i_dimen.x() / i_dimen.y();
			break;
		case RenderTargetDistanceType::pixels:
			out.mValue.x() /= i_dimen.x();
			out.mValue.y() /= i_dimen.y();
			break;
		case RenderTargetDistanceType::gl_point:
			out.mValue.x() += 1;
			out.mValue.y() += 1;
		case RenderTargetDistanceType::gl_vector:
			out.mValue.x() /= 2;
			out.mValue.y() /= 2;
			break;
		default: fail();
		}

		switch (i_from.mType) {
		case RenderTargetDistanceType::decimal_xy:
			break;
		case RenderTargetDistanceType::decimal_x:
			out.mValue.y() *= i_dimen.x() / i_dimen.y();
			break;
		case RenderTargetDistanceType::decimal_y:
			out.mValue.x() *= i_dimen.y() / i_dimen.x();
			break;
		case RenderTargetDistanceType::pixels:
			out.mValue.x() *= i_dimen.x();
			out.mValue.y() *= i_dimen.y();
			break;
		case RenderTargetDistanceType::gl_vector:
			out.mValue.x() *= 2;
			out.mValue.y() *= 2;
		case RenderTargetDistanceType::gl_point:
			out.mValue.x() -= 1;
			out.mValue.y() -= 1;
			break;
		default: fail();
		}

		return out;
	}
};

using RenderTargetDistance2f = RenderTargetDistance<float, 2>;
using RenderTargetDistance2d = RenderTargetDistance<double, 2>;
using RenderTargetDistance3f = RenderTargetDistance<float, 3>;
using RenderTargetDistance3d = RenderTargetDistance<double, 3>;

#endif