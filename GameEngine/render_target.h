#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "vector.h"

class RenderTarget {
public:
	void Draw_To_This();
	virtual Vector2i getDimensions() const = 0;
protected:
	virtual void _drawToThis() = 0;
};

#endif