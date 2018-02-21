#include "overlay.h"
#include "framework.h"

OverlayManager* OverlayManager::active = nullptr;

OverlayManager::OverlayManager() :
RenderComponent(RenderType::transparent),
components(512, offsetof(OverlayComponent, overlayManagerArrayIndex), true) {
	active = this;
	new TextManager();
}

OverlayManager::~OverlayManager() {
	while (components.Size())
		delete components[0];
}

void OverlayManager::Add(OverlayComponent* component) {
	Vector3d cloc = component->Get_Loc();
	for (int i = 0; i < components.Size(); i++) {
		if (cloc.z > components[i]->Get_Loc().z) {
			components.Insert(component, i);
			return;
		}
	}
	components.Add(component);
}

void OverlayManager::Remove(OverlayComponent* component) {
	components.Remove(component);
}

void OverlayManager::Sort() {
	//insertion sort
	OverlayComponent* c;
	for (int i = 0; i < components.Size(); i++) {
		for (int j = i - 1; j >= 0; j--) {
			if (components[j]->Get_Loc().z > components[j + 1]->Get_Loc().z)
				components.Swap(components[j], components[j + 1]);
			else 
				break;
		}
	}
}

void OverlayManager::Render(Flags callflags) {
 	Sort();
	for (int i = 0; i < components.Size(); i++)
		components[i]->Render();
}

void OverlayManager::Process_Click(Vector2d coords) {
	for (int i = components.Size() - 1; i >= 0; i--) {
		if (components[i]->Process_Click(coords))
			return;
	}
}

Rect2D::Rect2D(Transform2d transform, double depth, Texture2D* tex, Color4f col, uint maxchildren) :
OverlayComponent(maxchildren),
transform	(transform),
depth		(depth),
tex			(tex),
col			(col),
blend		({true, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_FUNC_ADD}),
flags		(Rect2DFlags::changes) {
	Init_Arrays();
}

Rect2D::~Rect2D() {
	glDeleteBuffers(3, &positionBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Rect2D::Render() {
	if (blend.blend)
		glEnable	(GL_BLEND);
	else
		glDisable	(GL_BLEND);
	glBlendFunc		(blend.src, blend.dst);
	glBlendEquation	(blend.eqn);
	glDepthMask		(0);
	glDisable		(GL_CULL_FACE);

	glBindVertexArray(vertexArrayID);

	if (flags & Rect2DFlags::locchange) {
		Vector3f positions[4] = {
			Vector3f(0, 1, depth),
			Vector3f(1, 1, depth),
			Vector3f(1, 0, depth),
			Vector3f(0, 0, depth)
		};

		glBindBuffer	(GL_ARRAY_BUFFER, positionBuffer);
		glBufferSubData	(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * 4, positions);

		flags &= ~Rect2DFlags::locchange;
	}

	if (flags & Rect2DFlags::colchange) {
		Color4f colors[4] = {col, col, col, col};

		glBindBuffer	(GL_ARRAY_BUFFER, colorBuffer);
		glBufferSubData	(GL_ARRAY_BUFFER, 0, sizeof(Color4f) * 4, colors);

		flags &= ~Rect2DFlags::colchange;
	}

	if (flags & Rect2DFlags::txcchange) {
		Vector2f uvBuffer[4] = {
			Vector2f(0, 0),
			Vector2f(1, 0),
			Vector2f(1, 1),
			Vector2f(0, 1)
		};

		glBindBuffer	(GL_ARRAY_BUFFER, uvBuffer);
		glBufferSubData	(GL_ARRAY_BUFFER, 0, sizeof(Vector2f) * 4, uvBuffer);

		flags &= ~Rect2DFlags::txcchange;
	}

	glActiveTexture(GL_TEXTURE0);
	if(tex != nullptr)
		tex->Activate();

	ShaderProgram::Activate("overlay_shader");

	transform.Apply_Object();
	Matrix4f modmatrix = modelStack.Top();

	uint modmatloc = glGetUniformLocation(ShaderProgram::active->vertexArrayID, "modmatrix");
	uint coltexloc = glGetUniformLocation(ShaderProgram::active->vertexArrayID, "tex");
	uint coltexbndloc = glGetUniformLocation(ShaderProgram::active->vertexArrayID, "texbnd");

	glUniformMatrix4fv(	modmatloc, 1, GL_FALSE, modmatrix.data);
	glUniform1i(		coltexloc, 0);
	glUniform1i(		coltexbndloc, (tex != nullptr));

	glDrawArrays(GL_QUADS, 0, 4);

	transform.Unapply_Object();

	if (tex != nullptr)
		tex->Deactivate();

	glEnable	(GL_CULL_FACE);
	glDisable	(GL_BLEND);
	glDepthMask	(1);

	ShaderProgram::Deactivate();
}

Vector3d Rect2D::Get_Loc() {
	return Vector3d(transform.position.x, transform.position.y, depth);
}

bool Rect2D::Process_Click(Vector2d coords) {
	return (flags & Rect2DFlags::clickopaque) && Is_In_Rect(transform, coords);
}

void Rect2D::Init_Arrays() {
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	glGenBuffers(3, &positionBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color4f) * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ScreenFade::ScreenFade(Texture2D* tex, Color4f color1, Color4f color2, double lifet) :
Rect2D(Transform2d(Vector2d(0, 0), Orientation2d(), Vector2d(800, 600)), 0.99, tex, color1, 0),
MechanicalComponent(0.017),
color1	(color1),
color2	(color2),
lifet	(lifet),
timeAccumulation	(0.0) 
{}

ScreenFade::~ScreenFade()
{}

void ScreenFade::Reset() {
	timeAccumulation = 0.0;
}

void ScreenFade::Reverse() {
	timeAccumulation = lifet - timeAccumulation;
	if (timeAccumulation < 0.0)
		timeAccumulation = 0.0;
	Color4f c = color1;
	color1 = color2;
	color2 = c;
}

void ScreenFade::Update(double t) {
	timeAccumulation += t;
	if (timeAccumulation < lifet) {
		double pctdone = timeAccumulation / lifet;
		double invdone = 1.0 - pctdone;
		col = color1 * invdone + color2 * pctdone;
	}
	else
		col = color2;
	flags |= Rect2DFlags::colchange;
}

SimpleRectButton::SimpleRectButton(Transform2d transform, double depth, SimpleRectButtonSettings settings, Text2D* text, void(*On_Click)(void*), void* arg) :
Rect2D			(transform, depth, settings.tex, settings.color, 1),
MechanicalComponent	(0.01),
settings		(settings),
text			(text),
On_Click		(On_Click),
arg				(arg),
hovering		(false),
rectcolor1		(settings.color),
rectcolor2		(settings.color),
textcolor1		(),
textcolor2		(),
colorstartt		(0.0),
colorlifet		(0.0) {
	flags |= Rect2DFlags::clickopaque;
	if (text) {
		Add(text);
		text->format.color = settings.textcolor;
		textcolor1 = text->format.color;
		textcolor2 = text->format.color;
	}
}

SimpleRectButton::~SimpleRectButton() {
	if (text)
		delete text;
	if (arg)
		delete arg;
}

void SimpleRectButton::Update(double t) {
	Vector2d mouseloc = Get_Mouse_Coords_Pixel();
	bool ishovering = Is_In_Rect(transform, mouseloc);
	if (hovering && !ishovering) {
		hovering = false;
		Process_Unhover();
	}
	if (!hovering && ishovering) {
		hovering = true;
		Process_Hover();
	}

	double colort = GEngine::Get().Clock().Now() - colorstartt;

	if (colort < colorlifet) {
		double pctdone = colort / colorlifet;
		double invdone = 1.0 - pctdone;

		col = rectcolor1 * invdone + rectcolor2 * pctdone;
		if (text != nullptr)
			text->format.color = textcolor1 * invdone + textcolor2 * pctdone;
	}
	else {
		col = rectcolor2;
		if (text != nullptr)
			text->format.color = textcolor2;
	}
	flags |= Rect2DFlags::colchange;
}

void SimpleRectButton::Process_Hover() {
	rectcolor1 = settings.color;
	rectcolor2 = settings.hovercolor;
	if (text != nullptr) {
		textcolor1 = settings.textcolor;
		textcolor2 = settings.texthovercolor;
	}
	colorstartt = GEngine::Get().Clock().Now();
	colorlifet = settings.hovert;
}

void SimpleRectButton::Process_Unhover() {
	rectcolor2 = settings.color;
	rectcolor1 = settings.hovercolor;
	if (text != nullptr) {
		textcolor2 = settings.textcolor;
		textcolor1 = settings.texthovercolor;
	}
	colorstartt = GEngine::Get().Clock().Now();
	colorlifet = settings.hovert;
}

bool SimpleRectButton::Process_Click(Vector2d coords) {
	if (Is_In_Rect(transform, coords)) {
		rectcolor2 = settings.hovercolor;
		rectcolor1 = settings.clickcolor;
		if (text != nullptr) {
			textcolor2 = settings.texthovercolor;
			textcolor1 = settings.textclickcolor;
		}
		colorstartt = GEngine::Get().Clock().Now();
		colorlifet = settings.clickt;
		if (On_Click != nullptr)
			On_Click(arg);
		return true;
	}
	return false;
}

PlayerShipSpeedometer::PlayerShipSpeedometer(PlayerShip* parent, double step) : 
OverlayComponent(0),
MechanicalComponent(step),
parent(parent),
text(nullptr) {
	TextFormat format = {
		(Font*)GEngine::Get().Resource().Get("consola.ttf"),
		TJ_LEFT,
		Color4f(1, 1, 1, 1),
		12,
		Vector2d(100, 12)
	};

	//text = new Text2D(Vector3f(0, 12, 0), "hello", format, 64);
}

PlayerShipSpeedometer::~PlayerShipSpeedometer() {
	if (text != nullptr) 
		delete text;
}

void PlayerShipSpeedometer::Update(double t) {
	return;
	text->Set_Text(Double_To_String(parent->velocity.Magnitude(), 1) + " m/s");
}
  
PlayerShipSystemsStatus::PlayerShipSystemsStatus(PlayerShip* parent, double step) : 
OverlayComponent(0),
MechanicalComponent(step),
parent(parent) {
	Rect2D** icons[6] = {
		&reactoricon,
		&shieldicon,
		&repairericon,
		&propulsionicon,
		&lasergunicon,
		&missilelaunchericon,
	};
	string iconnames[6] = {
		"reactor_icon.tga",
		"shield_icon.tga",
		"repair_icon.tga",
		"propulsion_icon.tga",
		"laser_icon.tga",
		"missile_icon.tga",
	};
	float ystart = 24.0;
	float size = 30.0;
	for (int i = 0; i < 6; i++) {
		*icons[i] = new Rect2D(
				Transform2d(Vector2d(0, ystart + size * i), Orientation2d(), Vector2d(size, size)), 0.0, 
				(Texture2D*)GEngine::Get().Resource().Get(iconnames[i]), 
				Color4f(1.0, 1.0, 1.0, 1.0), 0
				);
	}
}

PlayerShipSystemsStatus::~PlayerShipSystemsStatus() {
	delete lasergunicon;
	delete missilelaunchericon;
	delete propulsionicon;
	delete reactoricon;
	delete repairericon;
	delete shieldicon;
}

void PlayerShipSystemsStatus::Update(double t) {
	lasergunicon->			col.a = parent->laserguns.		efficiency;
	missilelaunchericon->	col.a = parent->missilelauncher.efficiency;
	propulsionicon->		col.a = parent->propulsion.		efficiency;
	reactoricon->			col.a = parent->reactor.		efficiency;
	repairericon->			col.a = parent->repairer.		efficiency;
	shieldicon->			col.a = parent->shield.			efficiency;

	Rect2D** icons[6] = {
		&reactoricon,
		&shieldicon,
		&repairericon,
		&propulsionicon,
		&lasergunicon,
		&missilelaunchericon,
	};

	for (int i = 0; i < 6; i++)
		(*icons[i])->flags |= Rect2DFlags::colchange;
	/*text->Set_Text(
		string("Rctr: ") +	Double_To_String(parent->reactor.			efficiency * 100, 1) + "%\count" +
		string("Shld: ") +	Double_To_String(parent->shield.			efficiency * 100, 1) + "%\count" +
		string("Prpn: ") +	Double_To_String(parent->propulsion.		efficiency * 100, 1) + "%\count" +
		string("Repr: ") +	Double_To_String(parent->repairer.			efficiency * 100, 1) + "%\count" +
		string("Stbl: ") +	Double_To_String(parent->stabilizers.		efficiency * 100, 1) + "%\count" +
		string("Lsrs: ") +	Double_To_String(parent->laserguns.			efficiency * 100, 1) + "%\count" +
		string("Mssl: ") +	Double_To_String(parent->missilelauncher.	efficiency * 100, 1) + "%"
		);*/
}

Reticule::Reticule(PlayerShip* parent) :
MechanicalComponent(0.01),
Rect2D(Transform2d(Vector2d(0, 0), Orientation2d(), Vector2d(32, 32)), 0.5, 
(Texture2D*)GEngine::Get().Resource().Get("reticule.tga"), 
Color4f(1.0, 1.0, 1.0, 1.0), 0),
parent(parent) 
{}

Reticule::~Reticule() 
{}

void Reticule::Update(double t) {
	Vector2d mouse = Get_Mouse_Coords_Gradient();
	double mousemag = mouse.Magnitude();

	if (mousemag > 0.25) {
		mouse /= mousemag;
		mouse *= 0.25;
	}

	Vector2d pixmouse = Gradient_To_Pixel_Coords(mouse);
	transform.position += Vector2d(16, 16);

	//if ((pixmouse - transform.position).Magnitude() > 500.0 * stepper.step)
	//	pixmouse = transform.position + (pixmouse - transform.position).Normalized() * 500.0 * stepper.step;

	transform.position = pixmouse - Vector2d(16, 16);
}

Cursor::Cursor() :
MechanicalComponent(0.01),
Rect2D(Transform2d(Vector2d(0, 0), Orientation2d(), Vector2d(10, 10)), 0.5, 
(Texture2D*)GEngine::Get().Resource().Get("cursor.tga"), 
Color4f(1.0, 1.0, 1.0, 1.0), 0) {
	ShowCursor(false);
}

Cursor::~Cursor() {
	ShowCursor(true);
}

void Cursor::Update(double t) {
	transform.position = Get_Mouse_Coords_Pixel() - Vector2d(5, 5);
}

bool Is_In_Rect(Transform2d transform, Vector2d v) {
	transform.orientation.Calculate_Up();
	v -= transform.position;
	Vector2d v1 = transform.orientation.right * transform.scale.x;
	Vector2d v2 = transform.orientation.up * -transform.scale.y;
	double d1 = v1.Dot(v);
	double d2 = v2.Dot(v);
	return (d1 >= 0 && d2 >= 0 && v1.Dot(v1) >= d1 && v2.Dot(v2) >= d2);
}