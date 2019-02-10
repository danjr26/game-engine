#include "render_target.h"
#include "game_engine.h"

void RenderTarget::Draw_To_This() {
	GE.render().mActiveTarget = this;
	_drawToThis();
}
