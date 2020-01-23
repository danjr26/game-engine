#ifndef BLEND_SETTINGS_H
#define BLEND_SETTINGS_H

#include <GL/glew.h>

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
		add = GL_FUNC_ADD,
		subtract = GL_FUNC_SUBTRACT,
		inv_subtract = GL_FUNC_REVERSE_SUBTRACT
	};

private:
	Factor mDestinationFactor;
	Factor mSourceFactor;
	Operation mOperation;

	BlendSettings(Factor i_destinationFactor, Factor i_sourceFactor, Operation i_operation);

public:
	BlendSettings();

	static BlendSettings none();
	static BlendSettings standard();
	static BlendSettings additive();
	static BlendSettings subtractive();
	static BlendSettings multiply();
	static BlendSettings custom(Factor i_destinationFactor, Factor i_sourceFactor, Operation i_operation);
};

#endif