#ifndef CLEAR_COMMAND_H
#define CLEAR_COMMAND_H

#include "../../../Utilities/include/utilities.h"
#include "enum_wrappers.h"
#include "render_target.h"

struct ClearCommand {
	uint mBits;
	Color4f mColor;
	float mDepth;
	uint mStencil;

	ClearCommand() :
	mBits(0),
	mColor(),
	mDepth(1.0f),
	mStencil(0)
	{}
};

#endif