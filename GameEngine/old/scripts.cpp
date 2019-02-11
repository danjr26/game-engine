#include "scripts.h"
#include "framework.h"

Script::Script() :
MechanicalComponent( 0.01f)
{}

void Script::Update(double t)
{}

const string InitConfigFile::path = "config/";
const string InitConfigFile::name = "initcfg.txt";

InitConfigFile::InitConfigFile() :
models		(nullptr),
fonts		(nullptr),
textures	(nullptr),
colls		(nullptr),
nmeshes		(0),
nfonts		(0),
ntextures	(0),
ncolls		(0) {
	ConfigData cd = ConfigData(path, name);

	ConfigDatum* meshpathdata =		cd.Get("meshpath");
	ConfigDatum* fontpathdata =		cd.Get("fontpath");
	ConfigDatum* texturepathdata =	cd.Get("texturepath");
	ConfigDatum* collpathdata =		cd.Get("collpath");

	meshpath =		meshpathdata->		elements[0];
	fontpath =		fontpathdata->		elements[0];
	texturepath =	texturepathdata->	elements[0];
	collpath =		collpathdata->		elements[0];

	ConfigDatum* meshData =		cd.Get("meshnames");
	ConfigDatum* fontdata =		cd.Get("fontnames");
	ConfigDatum* texturedata =	cd.Get("texturenames");
	ConfigDatum* colldata =		cd.Get("collnames");

	nmeshes =	meshData->nelements;
	nfonts =	fontdata->nelements;
	ntextures = texturedata->nelements;
	ncolls =	colldata->nelements;

	if (nmeshes != ncolls)
		Die("InitConfigFile.InitConfigFile: mesh / coll count mismatch");

	models =	new string[nmeshes];
	fonts =		new string[nfonts];
	textures =	new string[ntextures];
	colls =		new string[ncolls];

	for (int i = 0; i < nmeshes; i++)
		models[i] = meshData->elements[i];
	for (int i = 0; i < nfonts; i++)
		fonts[i] = fontdata->elements[i];
	for (int i = 0; i < ntextures; i++)
		textures[i] = texturedata->elements[i];
	for (int i = 0; i < ncolls; i++)
		colls[i] = colldata->elements[i];
}

InitConfigFile::~InitConfigFile() {
	delete[] models;
	delete[] textures;
	delete[] fonts;
	delete[] colls;
}

InitScript::InitScript() {
	OutputDebugStringA("\count");
	OutputDebugStringA("Loading OpenGL...\count");
	Init_OpenGL		();
	OutputDebugStringA("Loading meshes...\count");
	Init_Meshes		();
	OutputDebugStringA("Loading fonts...\count");
	Init_Fonts		();
	OutputDebugStringA("Loading textures...\count");
	Init_Textures	();
	OutputDebugStringA("Finishing up...\count");
	Init_Misc		();
	OutputDebugStringA("Done\count\count");
}

void InitScript::Init_OpenGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_SCISSOR_TEST);

	Shader vshader = Shader(GL_VERTEX_SHADER, "shaders/", "default_v.glsl");
	Shader fshader = Shader(GL_FRAGMENT_SHADER, "shaders/", "default_f.glsl");
	ShaderProgram* program = new ShaderProgram("default_shader", &vshader, &fshader, nullptr, nullptr);

	program->Activate();

	Shader vshader2 = Shader(GL_VERTEX_SHADER, "shaders/", "text_v.glsl");
	Shader fshader2 = Shader(GL_FRAGMENT_SHADER, "shaders/", "text_f.glsl");
	program = new ShaderProgram("text_shader", &vshader2, &fshader2, nullptr, nullptr);

	Shader vshader3 = Shader(GL_VERTEX_SHADER, "shaders/", "skybox_v.glsl");
	Shader fshader3 = Shader(GL_FRAGMENT_SHADER, "shaders/", "skybox_f.glsl");
	program = new ShaderProgram("skybox_shader", &vshader3, &fshader3, nullptr, nullptr);

	Shader vshader4 = Shader(GL_VERTEX_SHADER, "shaders/", "particle_v.glsl");
	Shader fshader4 = Shader(GL_FRAGMENT_SHADER, "shaders/", "particle_f.glsl");
	Shader gshader4 = Shader(GL_GEOMETRY_SHADER, "shaders/", "particle_g.glsl");
	program = new ShaderProgram("particle_shader", &vshader4, &fshader4, &gshader4, nullptr);

	Shader vshader5 = Shader(GL_VERTEX_SHADER, "shaders/", "beam_v.glsl");
	Shader fshader5 = Shader(GL_FRAGMENT_SHADER, "shaders/", "beam_f.glsl");
	Shader gshader5 = Shader(GL_GEOMETRY_SHADER, "shaders/", "beam_g.glsl");
	program = new ShaderProgram("beam_shader", &vshader5, &fshader5, &gshader5, nullptr);

	Shader vshader6 = Shader(GL_VERTEX_SHADER, "shaders/", "overlay_v.glsl");
	Shader fshader6 = Shader(GL_FRAGMENT_SHADER, "shaders/", "overlay_f.glsl");
	program = new ShaderProgram("overlay_shader", &vshader6, &fshader6, nullptr, nullptr);

	Shader vshader7 = Shader(GL_VERTEX_SHADER, "shaders/", "framebuffer_v.glsl");
	Shader fshader7 = Shader(GL_FRAGMENT_SHADER, "shaders/", "framebuffer_f.glsl");
	program = new ShaderProgram("framebuffer_shader", &vshader7, &fshader7, nullptr, nullptr);
}

void InitScript::Init_Meshes() {
	for (int i = 0; i < file.nmeshes; i++) {
		Mesh* mesh = new Mesh(file.meshpath, file.models[i]);
		mesh->Load_OBJ();
		if (file.colls[i] != "NULL") {
			Mesh* collmesh = new Mesh(file.collpath, file.colls[i]);
			collmesh->Load_OBJ();
			mesh->tree = new SphereTree(collmesh->rawvertices, collmesh->ntriangles, 0.0);
			delete collmesh;
		}
		else {
			mesh->tree = new SphereTree(mesh->rawvertices, mesh->ntriangles, 0.0);
		}
	}
	//((Mesh*)GEngine::Get().Resource().Get_Resource("pulse.obj"))->submeshes[0]->material.blend.blend = true;
}

void InitScript::Init_Fonts() {
	for (int i = 0; i < file.nfonts; i++)
		new FontFace(file.fontpath, file.fonts[i]);
}

void InitScript::Init_Textures() {
	for (int i = 0; i < file.ntextures; i++) {
		Texture2* texture = new Texture2(file.texturepath, file.textures[i], GL_RGBA8, 0);
		texture->Load_TGA();
	}
}

void InitScript::Init_Misc() {
	srand(time(0));
	new FPSCounter(0.5);
#ifdef GENG_PERF
	for (int i = 0; i < 512; i++) {
		Profiler::times[i] = 0;
		Profiler::keys[i] = "";
	}
#endif
}

void InitScript::Update(double t) {
	new Skybox("C:/Users/danie/Downloads/spacescape-0.5.1/spacescape/save/", "nebula1");
	new IntroScript();
	GEngine::Get().TRQ().Remove(timeRequest);
}

IntroScript::IntroScript() {
	
}

void IntroScript::Update(double t) {
	new Level1Script();
	delete this;
}

MainMenuScript* MainMenuScript::active = nullptr;

MainMenuScript::MainMenuScript() {
	/*active = this;

	isdone = false;

	background = new Rect2D(nullptr, Color4f(0.125, 0.125, 0.125, 1.0), Vector3d(0, 0, -1.0), 
		Vector2d(800, 600), nullptr, 0);
	background->clickopaque = true;

	fade = new ScreenFade(nullptr, Color4f(0, 0, 0, 1), Color4f(0, 0, 0, 0), 1.0);

	TextFormat tf = {
		(Font*)GEngine::Get().Resource().Get_Resource("consola.ttf"),
		TJ_CENTER,
		Color4f(1, 1, 1, 1),
		50,
		Vector2d(200, 50)
	};
	Text2D* playbuttontext = new Text2D(Vector3d(0, 3, 0.01), "PLAY", tf, 8);
	SimpleRectButtonSettings playbuttonsettings = {
		Vector2d(200, 50),

		nullptr,
		Color4f(0.13, 0.13, 0.13, 1.0),
		Color4f(1.0, 1.0, 1.0, 1.0),

		Color4f(0.15, 0.15, 0.15, 1.0),
		Color4f(1.0, 1.0, 1.0, 1.0),
		0.25,

		Color4f(0.2, 0.2, 0.2, 1.0),
		Color4f(0.0, 0.0, 0.0, 1.0),
		0.25
	};
	playbutton = new SimpleRectButton(Vector3d(300, 350, 0.0), playbuttonsettings, nullptr, playbuttontext, Play, nullptr);*/
}

MainMenuScript::~MainMenuScript() {
	/*delete fade;
	delete background;
	delete playbutton;
	active = nullptr;*/
}

void MainMenuScript::Update(double t) {
	/*if (isdone && fade->color == fade->color2) {
		new LevelSelectScript();
		delete this;
		return;
	}*/
}

void MainMenuScript::Play(void* arg) {
	/*if (active->isdone)
		return;
	active->fade->Reverse();
	active->isdone = true;*/
}

LevelRewardScript::LevelRewardScript() {

}

void LevelRewardScript::Update(double t) 
{}

LevelSelectScript* LevelSelectScript::active = nullptr;

LevelSelectScript::LevelSelectScript() {
	/*active = this;

	fade = new ScreenFade(
		nullptr, 
		Color4f(0, 0, 0, 1), 
		Color4f(0, 0, 0, 0), 
		1.0);

	background = new Rect2D(nullptr, 
		Color4f(0.125, 0.125, 0.125, 1.0), 
		Vector3d(0, 0, -1.0),
		Vector2d(800, 600), 
		nullptr, 0);
	background->clickopaque = true;

	TextFormat tf = {
		(Font*)GEngine::Get().Resource().Get_Resource("consola.ttf"),
		TJ_CENTER,
		Color4f(1, 1, 1, 1),
		50,
		Vector2d(50, 50)
	};

	SimpleRectButtonSettings levelbuttonsettings = {
		Vector2d(50, 50),

		nullptr,
		Color4f(0.13, 0.13, 0.13, 1.0),
		Color4f(1.0, 1.0, 1.0, 1.0),

		Color4f(0.15, 0.15, 0.15, 1.0),
		Color4f(1.0, 1.0, 1.0, 1.0),
		0.25,

		Color4f(0.2, 0.2, 0.2, 1.0),
		Color4f(0.0, 0.0, 0.0, 1.0),
		0.25
	};

	nlevelbuttons = 1;
	levelbuttons = new SimpleRectButton*[nlevelbuttons];
	levelbuttons[0] = new SimpleRectButton(Vector3d(375, 350, 0.0), levelbuttonsettings, nullptr,
		new Text2D(Vector3d(0, 3, 0.01), "1", tf, 2), Play_Level, new int(1));
	/*levelbuttons[1] = new SimpleRectButton(Vector3d(260, 500, 0.0), levelbuttonsettings, nullptr,
		new Text2D(Vector3d(0, 3, 0.01), "2", tf), Play_Level_2);
	levelbuttons[2] = new SimpleRectButton(Vector3d(320, 500, 0.0), levelbuttonsettings, nullptr,
		new Text2D(Vector3d(0, 3, 0.01), "3", tf), Play_Level_3);
	levelbuttons[3] = new SimpleRectButton(Vector3d(380, 500, 0.0), levelbuttonsettings, nullptr,
		new Text2D(Vector3d(0, 3, 0.01), "4", tf), Play_Level_4);
	levelbuttons[4] = new SimpleRectButton(Vector3d(440, 500, 0.0), levelbuttonsettings, nullptr,
		new Text2D(Vector3d(0, 3, 0.01), "5", tf), Play_Level_5);*

	SimpleRectButtonSettings customizebuttonsettings = levelbuttonsettings;
	customizebuttonsettings.dim = Vector2d(200, 40);
	tf.dim = Vector2d(200, 30);
	tf.size = 30;*/
}

LevelSelectScript::~LevelSelectScript() {
	/*delete fade;
	delete background;
	for (int i = 0; i < nlevelbuttons; i++)
		delete levelbuttons[i];
	delete[] levelbuttons;
	active = nullptr;
	/*delete titletext;
	delete levelinfo;*/
}

void LevelSelectScript::Update(double t) {
	/*if (isdone && fade->color == fade->color2) {
		new CustomizeScript(destination);
		delete this;
		return;
	}*/
}

void LevelSelectScript::Play_Level(void* arg) {
	/*if (active->isdone)
		return;
	active->destination = *(int*)arg;
	active->isdone = true;
	active->fade->Reverse();*/
}

CustomizeScript* CustomizeScript::active = nullptr;

CustomizeScript::CustomizeScript(int level) :
level(level) {
	/*active = this;
	fade = new ScreenFade(
		nullptr,
		Color4f(0, 0, 0, 1),
		Color4f(0, 0, 0, 0),
		1.0);
	background = new Rect2D(nullptr,
		Color4f(0.125, 0.125, 0.125, 1.0),
		Vector3d(0, 0, -1.0),
		Vector2d(800, 600),
		nullptr, 0);
	background->clickopaque = true;
	SimpleRectButtonSettings continuebuttonsettings = {
		Vector2d(200, 40),

		nullptr,
		Color4f(0.13, 0.13, 0.13, 1.0),
		Color4f(1.0, 1.0, 1.0, 1.0),

		Color4f(0.15, 0.15, 0.15, 1.0),
		Color4f(1.0, 1.0, 1.0, 1.0),
		0.25,

		Color4f(0.2, 0.2, 0.2, 1.0),
		Color4f(0.0, 0.0, 0.0, 1.0),
		0.25
	};
	TextFormat tf = {
		(Font*)GEngine::Get().Resource().Get_Resource("consola.ttf"),
		TJ_CENTER,
		Color4f(1, 1, 1, 1),
		30,
		Vector2d(200, 30)
	};
	continuebutton = new SimpleRectButton(
		Vector3d(300, 350, 0.0), 
		continuebuttonsettings, 
		nullptr, new Text2D(Vector3d(0, 3, 0.01), "CONTINUE", tf, 16), 
		Continue, nullptr);*/
}

CustomizeScript::~CustomizeScript() {
	/*delete continuebutton;
	delete background;
	delete fade;
	active = nullptr;*/
}

void CustomizeScript::Update(double t) {
	/*if (isdone && fade->color == fade->color2) {
		new Level1Script();
		delete this;
		return;
	}*/
}

void CustomizeScript::Continue(void* arg) {
	/*if (active->isdone)
		return;
	active->isdone = true;
	active->fade->Reverse();*/
}

Level1Script::Level1Script() {
	GEngine::Get().Clock().Pause();
	playership = new PlayerShip(Transform3d(Vector3d(0, 0, 0)), PlayerShip::defsettings);
	SwarmDroid* swarmdroid = new SwarmDroid(Transform3d(Vector3d(0, 0, -10)), SwarmDroid::defsettings);

	new SpaceDust();
	new Asteroid(Asteroid::Random(4, 2.0, 4.0), Transform3d(Vector3d(0, 10, 0)));

	//new CameraShake(5.0, 0.5, 0.1);

	//Collision collisiontest = playership->mask->Is_Colliding(swarmdroid->mask, 0.0);
	/*
	
	new Asteroid(Asteroid::Random(4, 4.0, 4.0), Transform3d(Vector3d(0, 5, 0)));
	new Asteroid(Asteroid::Random(4, 4.0, 4.0), Transform3d(Vector3d(0, -5, 0)));
	new Asteroid(Asteroid::Random(4, 4.0, 4.0), Transform3d(Vector3d(0, 10, 0)));
	new Asteroid(Asteroid::Random(4, 4.0, 4.0), Transform3d(Vector3d(0, -10, 0)));
	new Asteroid(Asteroid::Random(4, 4.0, 4.0), Transform3d(Vector3d(0, 0, 0)));
	
	Clock clock;
	int count = 100;
	double t1 = clock.Now();
	for (int i = 0; i < count; i++)
		GEngine::Get().Collision().Check_Collisions();
	double t2 = clock.Now();
	double dt = t2 - t1;
	double at = dt / count;
	OutputDebugStringA(to_string(at).c_str());
	exit(-1);
	*/
	//DefaultExplosion(Vector3d(0, 0, -5), 2.0);
	/*for (int i = 0; i < 5; i++) {
		SwarmDroid* sd = new SwarmDroid(Transform3d(
			Vector3d(i * 30, 30, 0), Orientation3d(), 1.0), SwarmDroid::defsettings);
	}
	for (int i = 0; i < 0; i++) {
		ShockDroid* sd = new ShockDroid(Transform3d(
			Vector3d(i * 30, 0, 0), Orientation3d(), 1.0), ShockDroid::defsettings);
	}*/

	/*Light* l1 = new Light(
		Color4f(0.0, 0.0, 0.0, 1.0),
		Color4f(0.5, 0.5, 0.5, 1.0),
		Color4f(1.0, 1.0, 1.0, 1.0),
		Vector3f(0, 0, 10), false
		);
	l1->followcam = true;*/
	Light* l2 = new Light(
		Color4f(0.1, 0.15, 0.2, 1.0), 
		Color4f(0.6, 0.6, 0.8, 1.0), 
		Color4f(1.0, 1.0, 1.0, 1.0), 
		Vector3f(0.0, -1.0, 0.0), LightFlags::on | LightFlags::directional);

	Light* l3 = new Light(
		Color4f(0.0, 0.0, 0.0, 1.0),
		Color4f(0.3, 0.4, 0.3, 1.0),
		Color4f(0.5, 0.5, 1.0, 1.0),
		Vector3f(0.0, 1.0, 0.0), LightFlags::on | LightFlags::directional);

	new Light(
		Color4f(0.0, 0.0, 0.0, 1.0),
		Color4f(0.3, 0.3, 1.0, 1.0),
		Color4f(0.8, 0.8, 1.0, 1.0),
		Vector3f(0.0, 0.0, 5.0), LightFlags::on | LightFlags::point);

	/*for (int i = 0; i < 10; i++) {
		float r = Rand_Float(1, 20);
		new Asteroid(Asteroid::Random(4, r * 0.3, r), 
			Transform3d(Vector3d(Rand_Float(-175, 175), Rand_Float(-175, 175), Rand_Float(-175, 175)), Orientation3d(), 1.0));
	}
	*/
	//enforcer = new MeshInstance((Mesh*)GEngine::Get().Resource().Get_Resource("test1.obj"));

	GEngine::Get().Render().Camera().transform.position = Vector3d(0, 0, 10);

	new Reticule(playership);
	new Cursor();

	new Rect2D(Transform2d(Vector2d(399, 299), Orientation2d(), Vector2d(2, 2)), 0.5, nullptr, Color4f(1.0, 1.0, 1.0, 1.0), 0);

	GEngine::Get().Clock().Resume();
}

void Level1Script::Update(double t) {
	/*Camera* cam = Camera::active;

	if (GetKeyState('W') & 0x8000)
		cam->transform.orientation.Rotate(Vector3f(1.0, 0.0, 0.0) * 90.0 * t);
	if (GetKeyState('S') & 0x8000)
		cam->transform.orientation.Rotate(Vector3f(1.0, 0.0, 0.0) * -90.0 * t);
	if (GetKeyState('D') & 0x8000)
		cam->transform.orientation.Rotate(Vector3f(0.0, 1.0, 0.0) * 90.0 * t);
	if (GetKeyState('A') & 0x8000)
		cam->transform.orientation.Rotate(Vector3f(0.0, 1.0, 0.0) * -90.0 * t);
	if (GetKeyState('E') & 0x8000)
		cam->transform.orientation.Rotate(Vector3f(0.0, 0.0, 1.0) * 90.0 * t);
	if (GetKeyState('Q') & 0x8000)
		cam->transform.orientation.Rotate(Vector3f(0.0, 0.0, 1.0) * -90.0 * t);
	*/
	/*
	if (GetKeyState('W') & 0x8000)
		enforcer->transform.orientation.Rotate(Vector3f(0.0, 0.0, -1.0) * 90.0 * t);
	if (GetKeyState('S') & 0x8000)
		enforcer->transform.orientation.Rotate(Vector3f(0.0, 0.0, -1.0) * -90.0 * t);
	if (GetKeyState('D') & 0x8000)
		enforcer->transform.orientation.Rotate(Vector3f(0.0, 1.0, 0.0) * 90.0 * t);
	if (GetKeyState('A') & 0x8000)
		enforcer->transform.orientation.Rotate(Vector3f(0.0, 1.0, 0.0) * -90.0 * t);
	if (GetKeyState('E') & 0x8000)
		enforcer->transform.orientation.Rotate(Vector3f(1.0, 0.0, 0.0) * 90.0 * t);
	if (GetKeyState('Q') & 0x8000)
		enforcer->transform.orientation.Rotate(Vector3f(1.0, 0.0, 0.0) * -90.0 * t);
	
	
	if (GetKeyState('W') & 0x8000)
		enforcer->transform.position += Vector3f(0, 0, -5) * t;
	if (GetKeyState('S') & 0x8000)
		enforcer->transform.position -= Vector3f(0, 0, -5) * t;
	if (GetKeyState('D') & 0x8000)
		enforcer->transform.position += Vector3f(5, 0, 0) * t;
	if (GetKeyState('A') & 0x8000)
		enforcer->transform.position -= Vector3f(5, 0, 0) * t;
	if (GetKeyState('E') & 0x8000)
		enforcer->transform.position += Vector3f(0, 5, 0) * t;
	if (GetKeyState('Q') & 0x8000)
		enforcer->transform.position -= Vector3f(0, 5, 0) * t;
		*/
	/*enforcer->transform.orientation.Rotate(Vector3f(0.707, 0.707, 0) * 90.0 * t);*/
}
