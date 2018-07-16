#include <Windows.h>
#include <iostream>
#include "async_task_manager.h"
#include "log.h"
#include "clock.h"
#include "bit_manipulation.h"
#include "packaged_async_task.h"
#include "renderer.h"
#include "matrix.h"
#include "window.h"
#include "color.h"
#include "sprite.h"
#include "framebuffer.h"
#include "game_engine.h"
#include "font_face.h"
#include "text2.h"
#include "test_input_context.h"
#include "circle_collision_mask.h"
#include "in_place_collision_evaluator.h"
#include "circle_renderer.h"
#include <exception>

void Test_Render(Window* window) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderProgram::Load_XML_List("shaders/shader_list.xml");

	FontFace* fontFace = GE.Assets().Get<FontFace>("ConsolasFontFace");
	FontFaceRasterSet* rasterSet = fontFace->Rasterize(24);

	CircleRenderer circle(Vector3d(400, 400, 0), 100.0, ColorRGBAf(0.5, 0.7, 0.9, 0.5), 20, ColorRGBAf(0, 0, 0, 0.5));

	GE.Render().Add(&circle);

	Vector2i winDim = window->Get_Dimensions();

	Texture fbColor = Texture(Texture::Type::_2d, Vector3i(winDim, 1), ColorRGBAc(255, 255, 255, 255), 4, 16, Texture::Flags::hdr);
	Texture fbDepth = Texture(Texture::Type::_2d, Vector3i(winDim, 1), ColorRGBAc(0, 0, 0, 0), 1, 32, Texture::Flags::depth);

	std::vector<Texture*> fbColors;
	fbColors.push_back(&fbColor);
	Framebuffer fb = Framebuffer(fbColors, &fbDepth);

	Camera camera1 = Camera(Transform(), Projection(Vector3f(0, window->Get_Dimensions().Y(), 0), Vector3f(window->Get_Dimensions().X(), 0, -1)));

	RenderPass testPass = 
		RenderPass(&fb, &camera1)
		.Clear_Color(ColorRGBAf(0.3f, 0.3f, 0.3f, 1.0f))
		.Sort_Order(RenderPass::SortOrder::back_to_front);

	Transform cam1Transform = Transform();

	GE.Render().passes.push_back(&testPass);

	Sprite rect2 = Sprite(
		DisplayUnits3::Percent(Vector3f(0, 0, 0)).OpenGL_Position(),
		DisplayUnits2::Percent(Vector2f(100, 100)).OpenGL_Displacement(),
		&fbColor
	);

	rect2.Texture_Instance().Settings().Set_Magnify_Filter(TextureSettings::FilterMode::bilinear);
	rect2.Texture_Instance().Settings().Set_Minify_Filter(TextureSettings::FilterMode::bilinear);

	GE.Render().Add(&rect2);

	rect2.Add_Filter(0);

	Camera camera2 = Camera();

	RenderPass testPass2 =
		RenderPass(window, &camera2)
		.Filter(FilterQuery::Fits(0))
		.Clear_Color(ColorRGBAf(0.3f, 0.3f, 0.3f, 1.0f));

	GE.Render().passes.push_back(&testPass2);

	/*
	std::vector<uint> m;
	m.push_back(10);
	uint q = 0;
	Clock clock;
	uint n = 1000000;
	double t1 = clock.Now();
	for (uint i = 0; i < n; i++) {
		q += m[0];
	}
	double t2 = clock.Now();
	double t = (t2 - t1);
	t1 = clock.Now();
	for (volatile uint i = 0; i < n; i++) {}
	t2 = clock.Now();
	t -= (t2 - t1);
	t *= 1000.0 / n;
	Log::main(string("\nAverage execution time:\n") + to_string(t) + " millisec");
	delete rasterSet;
	*/
	GE.Begin();
}

int WINAPI WinMain(HINSTANCE in_hInst, HINSTANCE in_hPrevInst, LPSTR arg, int nArgs) {
	Transform transform1 = Transform();
	Transform transform2 = Transform();
	transform2.Set_Parent(&transform1);

	transform1.Translate_Local(Vector3d(5, 0, 0));
	transform2.Rotate_Local(Rotationd(Vector3d(1, 0, 0), Vector3d(0, 1, 1).Normalized()));

	Vector3d v = transform2.Apply_To_World_Point(Vector3d(6, 0, 0));

	new GameEngine();

	Window::Params params =
		Window::Params(in_hInst)
		.Name(L"Test")
		.Dimensions(Vector2i(800, 600))
		.Fullscreen(false)
		.Always_Front(false);

	Window window(params);
	GE.Render().mainTarget = &window;
	GE.Render().mainWindow = &window;
	FontFace face = FontFace("fonts/consola.ttf");
	GE.Assets().Add("ConsolasFontFace", &face);

	Test_Render(&window);

	return 0;
}