#include <windows.h>
#include <iostream>
#include "async_task_manager.h"
#include "log.h"
#include "clock.h"
#include "bit_manipulation.h"
#include "packaged_async_task.h"
#include "matrix.h"
#include "window.h"
#include "color.h"
#include "sprite.h"
#include "framebuffer.h"
#include "game_engine.h"
#include "font_face.h"
#include "text2.h"
#include "test_input_context.h"
#include "in_place_collision_evaluator.h"
#include "circle_renderer.h"
#include "debug_mesh_vertex_data_renderer.h"
#include "binary_collision_tree.h"
#include "keyboard_text_input_context.h"
#include "editable_text.h"
#include "uniform_force_field.h"
#include "basic_collision_mask.h"
#include "particle_system2_specifiers.h"
#include "mesh_sphere_tree.h"
#include "player_ship.h"
#include "pointer_input_context.h"
#include "windows_error.h"
#include <exception>
#include <set>

void Test_Render(Window* window) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FontFace::loadXMLList("fonts/font_list.xml");
	ShaderProgram::loadXMLList("shaders/shader_list.xml");
	Texture::loadXMLList("img/texture_list.xml");

	PointerInputContext pointerInput;
	GE.input().add(&pointerInput);

	Clock c;
	uint n = 10000;
	double t1 = c.now();
	for (uint i = 0; i < n; i++) {
		
	}
	double t2 = c.now();
	double averageT = (t2 - t1) / n;
	Log::main(std::to_string(averageT * 1000000) + " us");

	double aspectRatio = (double)window->getDimensions().x() / (double)window->getDimensions().y();

	Camera camera1;
	camera1.setProjection(Projectiond(Vector3d(0.0, 10.0, 1.0), Vector3d(10.0 * aspectRatio, 0.0, -1.0)));
	GE.cameras().add(CameraManager::ID::main, &camera1);

	RenderPass testPass =
		RenderPass(window, &camera1)
		.clearColor(ColorRGBAf(0.08f, 0.05f, 0.10f, 1.0f))
		.sortOrder(RenderPass::SortOrder::back_to_front);

	GE.render().mPasses.push_back(&testPass);

	window->setVisible(true);

	GE.begin();
}

int WINAPI WinMain(HINSTANCE in_hInst, HINSTANCE in_hPrevInst, LPSTR arg, int nArgs) {
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	_CrtSetDbgFlag(tmpFlag | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);

	Log::main(getWindowsErrorMessage());

	new GameEngine();

	Window::Params params =
		Window::Params()
		.Name(L"Test")
		.Dimensions(Vector2i(800, 600))
		.Fullscreen(false)
		.Always_Front(false)
		.Visible(false);

	Window window(params);
		
	wglSwapIntervalEXT(0);

	GE.render().mMainTarget = &window;
	GE.render().mMainWindow = &window;

	Test_Render(&window);

	return 0;
}