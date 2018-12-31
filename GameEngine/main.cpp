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
#include "uniform_force_field.h"
#include <exception>
#include <set>

void Test_Render(Window* window) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FontFace::Load_XML_List("fonts/font_list.xml");
	ShaderProgram::Load_XML_List("shaders/shader_list.xml");

	FontFace* fontFace = GE.Assets().Get<FontFace>("ConsolasFontFace");
	FontFaceRasterSet* rasterSet = fontFace->Rasterize(24);

	Circled circle1 = Circled::From_Point_Radius(Vector2d(0.0, 0.0), 100.0);
	CircleRenderer circleRenderer1(circle1, ColorRGBAf(1, 1, 1, 0.5), 10.0, ColorRGBAf(1, 0.5, 0.5, 1.0));
	//Sprite circleRenderer1(AxisAlignedRectangled::From_Center(Vector2d(0, 0), Vector2d(200, 200)), nullptr);
	GE.Render().Add(&circleRenderer1);
	CircleCollisionMask circleCollider1(circle1);

	Circled circle2 = Circled::From_Point_Radius(Vector2d(0, 0), 50.0);
	CircleRenderer circleRenderer2(circle2, ColorRGBAf(1, 1, 1, 0), 2.0, ColorRGBAf(1, 1, 1, 1.0));
	GE.Render().Add(&circleRenderer2);
	CircleCollisionMask circleCollider2(circle2);

	CircleRenderer circleRenderer3(circle2, ColorRGBAf(1, 1, 1, 0), 2.0, ColorRGBAf(1, 1, 1, 1.0));
	GE.Render().Add(&circleRenderer3);

	Texture arrow(Texture::Type::_2d, "img/test_arrow.png", 8, Texture::Flags::none);

	//CircleRenderer circleRenderer4(circle2, ColorRGBAf(1, 1, 1, 0), 2.0, ColorRGBAf(1, 1, 1, 1.0));
	Sprite circleRenderer4(AxisAlignedRectangled::From_Center(Vector2d(0, 0), Vector2d(1000, 1000)), &arrow);
	circleRenderer4.Set_Color(ColorRGBAf(0.8, 0.4, 0.4, 0.5));
	GE.Render().Add(&circleRenderer4);

	InPlaceCollisionEvaluator2d collEval = InPlaceCollisionEvaluator2d();
	Collision coll = collEval.Evaluate(circleCollider1, circleCollider2);
	
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

	circleCollider1.Add_Filter(CollisionContext2d::user_defined);
	circleCollider2.Add_Filter(CollisionContext2d::user_defined);
	collisionContext.Add(&circleCollider1);
	collisionContext.Add(&circleCollider2);
	//collisionContext.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(CollisionContext2d::user_defined, CollisionContext2d::user_defined), true);
	
	TestSpriteMover sm1 = TestSpriteMover(&circleRenderer1, circleCollider1);
	GE.Per_Frame_Update().Add(&sm1);
	
	TestSpriteMover sm2 = TestSpriteMover(&circleRenderer2, circleCollider2);
	GE.Per_Frame_Update().Add(&sm2);

	TestSpriteMover sm3 = TestSpriteMover(&circleRenderer3, circleCollider2);
	GE.Per_Frame_Update().Add(&sm3);

	TestSpriteMover2 sm4 = TestSpriteMover2(&circleRenderer4);
	GE.Per_Frame_Update().Add(&sm4);
	
	AxisAlignedHalfSpace2d aaHalfSpace = AxisAlignedHalfSpace2d::From_Dimension_Value(1, 0, true);
	AxisAlignedHalfSpace2CollisionMask aaHalfSpaceMask(aaHalfSpace);
	aaHalfSpaceMask.Get_Transform().Translate_World(Vector2d(0, 600));
	RigidBody2 aaHalfSpaceBody(aaHalfSpaceMask);
	aaHalfSpaceBody.Set_Unstoppable(true);
	aaHalfSpaceBody.Set_Angular_Mass(0.0);
	aaHalfSpaceBody.Set_Linear_Mass(0.0);
	GE.Physics().Add(&aaHalfSpaceBody);

	UniformForceField2 gravity(Vector2d(0, 500));
	GE.Physics().Add(&gravity);

	QuadTreed quadTree(AxisAlignedRectangled::From_Extrema(Vector2d(0, 0), Vector2d(800, 600)));

	/*
	Clock c;
	uint n = 10000;
	double t1 = c.Now();
	for (uint i = 0; i < n; i++) {
		//std::find(testVec.begin(), testVec.end(), 10);
	}
	double t2 = c.Now();
	double averageT = (t2 - t1) / n;
	Log::main(std::to_string(averageT * 1000000) + " us");

	*/

	Vector2d positions[] = {
		Vector2d(100.0f, 0.0f),
		Vector2d(-50.0f, -86.6f),
		Vector2d(-50.0f, 86.6f)
	};

	Triangle2d triangle = Triangle2d::From_Points(positions);
	Triangle2CollisionMask triangleMask = Triangle2CollisionMask(triangle);

	uchar indices[] = { 0, 1, 2 };

	MeshVertexData meshData(MeshVertexData::DataType::_ubyte);
	meshData.Add_Vertices(3, {});
	meshData.Add_Member(MeshVertexData::MemberID::position, MeshVertexData::DataType::_double, 2, positions);
	meshData.Add_Faces(1, indices);

	DebugMeshVertexDataRenderer meshRenderer(&meshData);
	GE.Render().Add(&meshRenderer);

	RigidBody2* rigidBody = new RigidBody2(triangleMask);
	meshRenderer.Get_Transform().Set_Parent(&rigidBody->Get_Transform());
	rigidBody->Get_Transform().Set_Local_Position(Vector2d(200, 100));
	rigidBody->Get_Transform().Set_Local_Rotation(PI / 4);
	rigidBody->Set_Linear_Velocity(Vector2d(50, 0));
	GE.Physics().Add(rigidBody);

	Vector2d polyPoints[] = {
		Vector2d(0, 0),
		Vector2d(100, 0),
		Vector2d(50, 50),
		Vector2d(100, 100),
		Vector2d(0, 100)
	};

	MeshVertexData poly(MeshVertexData::DataType::_ubyte);
	poly.Add_Vertices(5, {});
	poly.Add_Member(MeshVertexData::MemberID::position, MeshVertexData::DataType::_double, 2, polyPoints);
	poly.Add_Faces_Polygon<double>();

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