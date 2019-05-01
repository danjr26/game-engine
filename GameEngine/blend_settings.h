#ifndef BLEND_SETTINGS_H
#define BLEND_SETTINGS_H

#include "misc.h"

class BlendSettings {
public:
	enum class Factor : GLenum {
		zero = GL_ZERO,
		one = GL_ONE,
		src_a = GL_SRC_ALPHA,
		dst_a = GL_DST_ALPHA,
		inv_src_a = GL_ONE_MINUS_SRC_ALPHA,
		inv_dst_a = GL_ONE_MINUS_DST_ALPHA,
		src_rgba = GL_SRC_COLOR,
		dst_rgba = GL_DST_COLOR,
		inv_src_rgba = GL_ONE_MINUS_SRC_COLOR,
		inv_dst_rgba = GL_ONE_MINUS_DST_COLOR
	};

	enum class Operation : GLenum {
		add = GL_ADD,
		subtract = GL_SUBTRACT,
		multiply = GL_MULT
	};

private:
	Factor mDestinationFactor;
	Factor mSourceFactor;
	Operation mOperation;

	BlendSettings(Factor in_destinationFactor, Factor in_sourceFactor, Operation in_operation);

public:
	void use(const BlendSettings* in_currentSettings = nullptr);

	static BlendSettings none();
	static BlendSettings standard();
	static BlendSettings additive();
	static BlendSettings subtractive();
	static BlendSettings multiply();
	static BlendSettings custom(Factor in_destinationFactor, Factor in_sourceFactor, Operation in_operation);
};

#endif