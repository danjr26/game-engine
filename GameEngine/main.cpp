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
#include <exception>

void Test_Render(Window* window) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	Shader vertexShaderDeferred(Shader::Type::vertex, "shaders/deferred_1_v.glsl");
	Shader fragmentShaderDeferred(Shader::Type::fragment, "shaders/deferred_1_f.glsl");
	std::vector<Shader*> shadersDeferred;
	shadersDeferred.push_back(&vertexShaderDeferred);
	shadersDeferred.push_back(&fragmentShaderDeferred);
	ShaderProgram* shaderProgramDeferred = new ShaderProgram(shadersDeferred);
	GE.Assets().Add("DeferredShader", shaderProgramDeferred);

	Shader vertexShaderText(Shader::Type::vertex, "shaders/text_v.glsl");
	Shader fragmentShaderText(Shader::Type::fragment, "shaders/text_f.glsl");
	std::vector<Shader*> shadersText;
	shadersText.push_back(&vertexShaderText);
	shadersText.push_back(&fragmentShaderText);
	ShaderProgram* shaderProgramText = new ShaderProgram(shadersText);
	GE.Assets().Add("TextShader", shaderProgramText);
	*/

	ShaderProgram::Load_XML_List("shaders/shader_list.xml");

	FontFace* fontFace = GE.Assets().Get<FontFace>("ConsolasFontFace");

	FontFaceRasterSet* rasterSet = fontFace->Rasterize(24);

	Transform textTransform = Transform();
	textTransform.Translate(Vector3d(75, 150, -0.25));
	textTransform.Set_Rotation(Matrix4f::Rotation(Vector3d(0, 0, -1), 0));
	//textTransform.Translate(Vector3d(50, -50, 0));

	Text2 testText = Text2("A line of text.\nYay more text!", rasterSet, textTransform);

	GE.Render().Add(&testText);

	Vector3d pos = Vector3f(0, 100, 0);//DisplayUnits3::Pixels(Vector3f(50, 50, 0), window->Get_Dimensions()).OpenGL_Position();
	Vector2d dim = Vector2f(200, 200); // DisplayUnits2::Pixels(Vector2f(50, 50), window->Get_Dimensions()).OpenGL_Displacement();

	Texture imageTex = Texture(Texture::Type::_2d, "img/pngtest.png", 8, Texture::Flags::mipmaps);

	Sprite rect = Sprite(pos, dim, &imageTex);

	TestInputContext inputContext = TestInputContext();
	GE.Input().Add_After(nullptr, &inputContext);

	TestSpriteMover mover = TestSpriteMover(&inputContext, &rect);

	//rect.Get_Transform().Translate(Vector3d(1, 1, -0.5));
	//rect.Get_Transform().Set_Rotation(Matrix4f::Rotation(Vector3f(0, 0, -1), PI / 4));

	GE.Render().Add(&rect);

	rect.Texture_Instance().Settings().Set_Minify_Filter(TextureSettings::FilterMode::none);
	rect.Texture_Instance().Settings().Set_Magnify_Filter(TextureSettings::FilterMode::none);
	Vector2d imageDim = DisplayUnits2::Pixels((Vector2i)rect.Texture_Instance().Get_Texture()->Get_Dimensions(), window->Get_Dimensions()).OpenGL_Displacement();
	//rect.Get_Transform().Set_Scale_Factor(Vector3d(imageDim, 1.0));

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
	Clock clock;

	uint n = 1;

	double t1 = clock.Now();
	for (uint i = 0; i < n; i++) {
		this_thread::sleep_for(std::chrono::milliseconds(16));
		GE.Render().Render_Frame();
		window->Flip_Buffers();
	}
	double t2 = clock.Now();
	double t = (t2 - t1);

	t1 = clock.Now();
	for (uint i = 0; i < n; i++) {
		this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	t2 = clock.Now();

	t -= (t2 - t1);

	t *= 1000.0 / n;

	Log::main(string("\nAverage execution time:\n") + to_string(t) + " millisec");

	delete rasterSet;
	*/

	GE.Begin();
	
}

int WINAPI WinMain(HINSTANCE in_hInst, HINSTANCE in_hPrevInst, LPSTR arg, int nArgs) {
	new GameEngine();

	FatalAppExitA(0, "Oh no!");

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