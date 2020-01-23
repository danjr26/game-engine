#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "vertex_pusher.h"

class Renderable {
public:
	using flags_t = uint32_t;
	enum Flags : flags_t {
		opaque = 1 << 0,
		transparent = 1 << 1,
		cast_shadow = 1 << 2
	};

public:
	virtual VertexPusher* getVertexPusher() = 0;
	virtual flags_t getFlags() const = 0;
};

#endif