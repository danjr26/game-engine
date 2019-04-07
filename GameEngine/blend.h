#ifndef BLEND_H
#define BLEND_H

#include "misc.h"

class BlendSettings {
public:
	enum Factor : GLenum {
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

	enum Operation {
		add = GL_ADD,
		subtract = GL_SUBTRACT,
		multiply = GL_MULT
	};

private:
	GLenum	mDestinationFactor;
	GLenum	mSourceFactor;
	GLenum	mOperation;

	BlendSettings(Factor in_destinationFactor, Factor in_sourceFactor, Factor in_operation);

public:
	static BlendSettings none();
	static BlendSettings standard();
	static BlendSettings additive();
	static BlendSettings subtractive();
	static BlendSettings multiply();
	static BlendSettings custom();
};

#endif