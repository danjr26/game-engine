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
#include "debug_mesh_vertex_data_renderer.h"
#include "binary_collision_tree.h"
#include "keyboard_text_input_context.h"
#include "editable_text.h"
#include <exception>
#include <set>

void Test_Render(Window* window) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FontFace::Load_XML_List("fonts/font_list.xml");
	ShaderProgram::Load_XML_List("shaders/shader_list.xml");

	FontFace* fontFace = GE.Assets().Get<FontFace>("ConsolasFontFace");
	FontFaceRasterSet* rasterSet = fontFace->Rasterize(24);

	Circled circle1 = Circled::From_Point_Radius(Vector2d(50.0, 50.0), 100.0);
	CircleRenderer circleRenderer1(DeepCircled(circle1, 0.0), ColorRGBAf(1, 1, 1, 0.5), 10.0, ColorRGBAf(1, 0.5, 0.5, 1.0));
	GE.Render().Add(&circleRenderer1);
	CircleCollisionMask circleCollider1(circle1);

	Circled circle2 = Circled::From_Point_Radius(Vector2d(450.0, 200.0), 50.0);
	CircleRenderer circleRenderer2(DeepCircled(circle2, 0.0), ColorRGBAf(1, 1, 1, 0), 2.0, ColorRGBAf(1, 1, 1, 1.0));
	GE.Render().Add(&circleRenderer2);
	CircleCollisionMask circleCollider2(circle2);

	InPlaceCollisionEvaluator2d collEval = InPlaceCollisionEvaluator2d();
	Collision coll = collEval.Evaluate(circleCollider1, circleCollider2);

	///////////////////////////
	
	KeyboardTextInputContext textInput;
	GE.Input().Add_After(nullptr, &textInput);

	EditableText eText = EditableText(&textInput, rasterSet);
	eText.Get_Transform().Translate_Local(Vector2d(200, 0));
	GE.Render().Add(&eText);

	TestInputContext ic;
	GE.Input().Add_After(nullptr, &ic);

	AxisAlignedRectangled box = AxisAlignedRectangled::From_Extrema(Vector2d(0, 0), Vector2d(800, 600));
	CollisionContext2d collisionContext(box, 3);
	GE.Collision().Make_Active(&collisionContext);

	circleCollider1.Add_Filter(0);
	circleCollider2.Add_Filter(0);
	collisionContext.Add(&circleCollider1);
	collisionContext.Add(&circleCollider2);
	//collisionContext.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(0, 0), true);

	TestSpriteMover sm = TestSpriteMover(&ic, &circleRenderer1, &circleCollider1);
	GE.Per_Frame_Update().Add(&sm);

	QuadTreed quadTree(AxisAlignedRectangled::From_Extrema(Vector2d(0, 0), Vector2d(800, 600)));

	Triangle2d triangle = Triangle2d::From_Points(
		Vector2d(1.0f, 1.0f),
		Vector2d(0.0f, 100.0f),
		Vector2d(100.0f, 0.0f)
	);

	Triangle2CollisionMask triangleMask = Triangle2CollisionMask(triangle);

	std::vector<ullong> testVec(10000);

	Clock c;
	uint n = 10000;
	double t1 = c.Now();
	for (uint i = 0; i < n; i++) {
		std::find(testVec.begin(), testVec.end(), 10);
	}
	double t2 = c.Now();
	double averageT = (t2 - t1) / n;
	Log::main(to_string(averageT * 1000000) + " us");

	////////////////////////////

	Vector3f positions[] = {
		Vector3f(0.0f, 0.0f, 0.0f),
		Vector3f(0.0f, 100.0f, 0.0f),
		Vector3f(200.0f, 200.0f, 0.0f),
		Vector3f(100.0f, 0.0f, 0.0f)
	};

	uchar indices[] = {
		0, 3, 1,
		2, 1, 3
	};

	MeshVertexData meshData(MeshVertexData::DataType::_ubyte);
	meshData.Add_Vertices(4, {});
	meshData.Add_Member(MeshVertexData::MemberID::position, MeshVertexData::DataType::_float, 3, positions);
	meshData.Add_Faces(2, indices);

	DebugMeshVertexDataRenderer meshRenderer(&meshData);
	GE.Render().Add(&meshRenderer);

	Vector2i winDim = window->Get_Dimensions();

	Texture fbColor = Texture(Texture::Type::_2d, Vector3i(winDim, 1), ColorRGBAc(255, 255, 255, 255), 4, 16, Texture::Flags::hdr);
	Texture fbDepth = Texture(Texture::Type::_2d, Vector3i(winDim, 1), ColorRGBAc(0, 0, 0, 0), 1, 32, Texture::Flags::depth);

	std::vector<Texture*> fbColors;
	fbColors.push_back(&fbColor);
	Framebuffer fb = Framebuffer(fbColors, &fbDepth);

	Camera camera1;
	camera1.Set_Projection(Projectiond(Vector3d(0, window->Get_Dimensions().Y(), 0), Vector3d(window->Get_Dimensions().X(), 0, -1)));

	RenderPass testPass = 
		RenderPass(&fb, &camera1)
		.Clear_Color(ColorRGBAf(0.3f, 0.3f, 0.3f, 1.0f))
		.Sort_Order(RenderPass::SortOrder::back_to_front);

	GE.Render().passes.push_back(&testPass);

	Sprite rect2 = Sprite(AxisAlignedRectangled::From_Extrema(
			DisplayUnits2d::Percent(Vector2d(0.0, 0.0)).OpenGL_Position(),
			DisplayUnits2d::Percent(Vector2d(100.0, 100.0)).OpenGL_Position()
		), &fbColor
	);

	rect2.Texture_Instance().Settings().Set_Magnify_Filter(TextureSettings::FilterMode::bilinear);
	rect2.Texture_Instance().Settings().Set_Minify_Filter(TextureSettings::FilterMode::bilinear);

	GE.Render().Add(&rect2);

	rect2.Add_Filter(0);

	Camera camera2;

	RenderPass testPass2 =
		RenderPass(window, &camera2)
		.Filter(FilterQuery::Fits(0))
		.Clear_Color(ColorRGBAf(0.3f, 0.3f, 0.3f, 1.0f));

	GE.Render().passes.push_back(&testPass2);

	GE.Begin();
}

int WINAPI WinMain(HINSTANCE in_hInst, HINSTANCE in_hPrevInst, LPSTR arg, int nArgs) {
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

	Test_Render(&window);

	return 0;
}