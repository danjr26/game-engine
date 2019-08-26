#include "render_target.h"
#include "game_engine.h"

void RenderTarget::drawToThis() {
	GE.render().mActiveTarget = this;
	_drawToThis();
}
