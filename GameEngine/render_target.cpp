#include "render_target.h"
#include "game_engine.h"

void RenderTarget::Draw_To_This() {
	GE.Render().activeTarget = this;
	_Draw_To_This();
}
