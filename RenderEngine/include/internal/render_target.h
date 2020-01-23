#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "../../../Utilities/include/utilities.h"

class RenderTarget {
friend class RenderEngine;
protected:
	virtual void _activate() = 0;
	virtual Vector2ui _getRenderTargetDimen() const = 0;
};

#endif