#include "fx.h"
#include "framework.h"

AlienEntrance::AlienEntrance(Vector3d in_position, float in_radius) :
MechanicalComponent(&GEngine::Get().Play_Clock(), 1.0 / 60.0),
tracker(1, 1),
timeAccumulation(0.0) {
	RawMesh rawmesh = Ico_Sphere(4, 1.0, true, false);
	Material matl;
	matl.diffuse =		Color4f(1.0f, 1.0f, 1.0f, 0.05f);
	matl.lighting =		false;
	matl.blend.blend =	true;
	matl.blend.dst =	GL_ONE;
	matl.blend.src =	GL_SRC_ALPHA;
	matl.blend.eqn =	GL_FUNC_REVERSE_SUBTRACT;
	meshData = new Mesh("", "");
	meshData->Load_Raw(rawmesh.vertices, rawmesh.norms, rawmesh.texcs, rawmesh.ntris, matl);
	models = new MeshInstance*[2];
	for (int i = 0; i < 2; i++) {
		models[i] = new MeshInstance(meshData);
		models[i]->transform.position = in_position;
	}
	models[1]->mesh->submeshes[0]->material = matl;
	models[1]->mesh->submeshes[0]->material.diffuse = Color4f(1.0, 1.0, 1.0, 1.0);
}

AlienEntrance::~AlienEntrance() {
	delete meshData;
	for (int i = 0; i < 2; i++)
		delete models[i];
	delete[] models;
}

void AlienEntrance::Update(double in_timePassed) {
	tracker.Update(in_timePassed);
	timeAccumulation += in_timePassed;
	double scaleFactor1, scaleFactor2;
	if (timeAccumulation < (tracker.Get_Turn_On_Duration() + tracker.Get_Turn_Off_Duration()))
		scaleFactor1 = 10.0 * sin(tracker.Lin_Interp(0.0, PI));
	else
		scaleFactor1 = 0.0;
	if (timeAccumulation < (tracker.Get_Turn_On_Duration() + tracker.Get_Turn_Off_Duration()) * 0.75 && timeAccumulation > 
		(tracker.Get_Turn_On_Duration() + tracker.Get_Turn_Off_Duration()) * 0.25)
		scaleFactor2 = 10.0 * sin(tracker.Lin_Interp(0.0, PI) + PI * 0.5);
	else
		scaleFactor2 = 0.0;

	models[0]->transform.scale = Vector3d(scaleFactor1, scaleFactor1, scaleFactor1);
	models[1]->transform.scale = Vector3d(scaleFactor2, scaleFactor2, scaleFactor2);
	if (!tracker.Is_Transitioning())
		tracker.Toggle();
}

LightningBolt::LightningBolt(Vector3d in_position1, Vector3d in_position2) :
RenderComponent(RenderType::transparent),
MechanicalComponent(&GEngine::Get().Play_Clock(), 0.05) {
	double length = (in_position1 - in_position2).Magnitude();
	thickness = 0.5;
	nVertices = (uint)(length / (thickness * 3.0));
	vertices = new Vector3d[nVertices];
	vertices[0] = in_position1;
	vertices[nVertices - 1] = in_position2;
	Calculate_Vertices();
}

LightningBolt::~LightningBolt() {
	delete[] vertices;
}

void LightningBolt::Render(Flags in_renderFlags) {
	Vector3d* quads = Get_Quads();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glDepthMask(0);
	glDisable(GL_LIGHTING);

	Texture2D* tex = (Texture2D*)GEngine::Get().Resource().Get_Resource("lightning.tga");
	tex->Activate();
	glColor4f(0.0, 0.5, 0.0, 1.0);
	glBegin(GL_QUAD_STRIP);
	for (uint i = 0; i < (nVertices - 1); i++) {
		glTexCoord2f(1, 0);
		glVertex3dv(&quads[i * 4 + 0].x);
		glTexCoord2f(1, 1);
		glVertex3dv(&quads[i * 4 + 1].x);
		glTexCoord2f(0, 0);
		glVertex3dv(&quads[i * 4 + 2].x);
		glTexCoord2f(0, 1);
		glVertex3dv(&quads[i * 4 + 3].x);
	}
	glEnd();
	tex->Deactivate();

	glDisable(GL_BLEND);
	glDepthMask(1);
	glEnable(GL_LIGHTING);

	delete[] quads;
}

void LightningBolt::Update(double in_timePassed) {
	Calculate_Vertices();
}

void LightningBolt::Calculate_Vertices() {
	Vector3d loc1 = vertices[0];
	Vector3d loc2 = vertices[nVertices - 1];
	Vector3d toloc2 = loc2 - loc1;
	float length = (float)toloc2.Magnitude();
	toloc2 /= length;
	for (uint i = 0; i < nVertices; i++)
		vertices[i] = loc2 * i / (nVertices - 1) + loc1 * (nVertices - i - 1) / (nVertices - 1);

	Vector3d var = Vector3d::Random_Normalized();
	var -= toloc2 * var.Dot(toloc2);
	var *= thickness;

	for (uint i = 1; i < nVertices - 1; i++) {
		Vector3d rand = Vector3d::Random_Normalized();
		rand -= toloc2 * rand.Dot(toloc2);
		rand.Normalize();
		if (var.Normalized().Dot(rand) > 0) {
			if (var.Magnitude() > (nVertices - i) * thickness * 0.707)
				rand = rand * -1;
			else
				rand *= 1 + var.Normalized().Dot(rand);
		}
		var += rand * 1.0 * thickness;
		vertices[i] += var;
	}
}

Vector3d* LightningBolt::Get_Quads() {
	Vector3d* rects = new Vector3d[(nVertices - 1) * 4];
	for (uint i = 0; i < nVertices - 1; i++) {
		Vector3d midpt = (vertices[i] + vertices[i + 1]) * 0.5;
		Vector3d toloc1 = (vertices[i] - vertices[i + 1]).Normalized();
		Vector3d toloc2 = (vertices[i + 1] - vertices[i]).Normalized();
		Vector3d tocamera = GEngine::Get().Render().Camera().transform.position - midpt;
		tocamera.Normalize();
		Vector3d axis = tocamera.Cross((vertices[i] - vertices[i + 1]).Normalized()).Normalized();
		rects[i * 4 + 0] = vertices[i] + axis * thickness - toloc1 * thickness;
		rects[i * 4 + 1] = vertices[i] - axis * thickness - toloc1 * thickness;
		rects[i * 4 + 2] = vertices[i + 1] + axis * thickness - toloc2 * thickness;
		rects[i * 4 + 3] = vertices[i + 1] - axis * thickness - toloc2 * thickness;
	}
	return rects;
}

GlowLaser::GlowLaser(Transform3d in_transform, double in_length, double in_width, Color4f in_color) :
transform	(in_transform),
length		(in_length),
width		(in_width),
color		(in_color) {
	GEngine::Get().Render().Laser().Add(this);
}

GlowLaser::~GlowLaser() {
	GEngine::Get().Render().Laser().Remove(this);
}

void GlowLaser::Calculate_Vertices() {
	Vector3d vertex1 = transform.position - transform.orientation.forward * length * 0.5;
	Vector3d vertex2 = transform.position + transform.orientation.forward * length * 0.5;
	
	LaserManager* renderer = &GEngine::Get().Render().Laser();

	renderer->positions[laserManagerArrayIndex * 2 + 0] = vertex1;
	renderer->positions[laserManagerArrayIndex * 2 + 1] = vertex2;
	renderer->colors[laserManagerArrayIndex * 2 + 0] = color;
	renderer->colors[laserManagerArrayIndex * 2 + 1] = color;
	renderer->uvs[laserManagerArrayIndex * 2 + 0] = Vector2f(0, 0);
	renderer->uvs[laserManagerArrayIndex * 2 + 1] = Vector2f(1, 1);
	renderer->widths[laserManagerArrayIndex * 2 + 0] = width;
	renderer->widths[laserManagerArrayIndex * 2 + 1] = width;
}

LaserManager::LaserManager() :
RenderComponent(RenderType::transparent),
components(CAPACITY, offsetof(GlowLaser, laserManagerArrayIndex)) {
	positions = new Vector3f[CAPACITY * 2];
	colors =	new Color4f	[CAPACITY * 2];
	uvs =		new Vector2f[CAPACITY * 2];
	widths =	new float	[CAPACITY * 2];
}

LaserManager::~LaserManager() {
	while (components.Size())
		delete components[0];
	delete[] positions;
	delete[] colors;
	delete[] uvs;
	delete[] widths;
	glDeleteBuffers(4, &positionBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void LaserManager::Add(GlowLaser* in_component) {
	components.Add(in_component);
}

void LaserManager::Remove(GlowLaser* in_component) {
	components.Remove(in_component);
}

void LaserManager::Init_Arrays() {
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	glGenBuffers(4, &positionBuffer);

	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 2 * CAPACITY, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color4f) * 2 * CAPACITY, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * 2 * CAPACITY, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, widthBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * CAPACITY, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LaserManager::Render(Flags in_callFlags) {
	if (in_callFlags == RenderType::transparent) {
		for (uint i = 0; i < components.Size(); i++) {
			components[i]->Calculate_Vertices();
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glDepthMask(0);
		glDisable(GL_CULL_FACE);

		glActiveTexture(GL_TEXTURE0);
		Texture2D* texture = (Texture2D*)GEngine::Get().Resource().Get_Resource("sparkglow.tga");
		texture->Activate();

		ShaderProgram::Activate("beam_shader");

		Matrix4f viewMatrix =		viewStack.Top();
		Matrix4f projectionMatrix = projectionStack.Top();
		Matrix4f modelMatrix =		Matrix4f();//modelStack.Top();

		glBindVertexArray(vertexArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * components.Size() * 2, positions);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Color4f) * components.Size() * 2, colors);

		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector2f) * components.Size() * 2, uvs);

		glBindBuffer(GL_ARRAY_BUFFER, widthBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * components.Size() * 2, widths);

		glActiveTexture(GL_TEXTURE0);
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "viematrix"), 1, GL_FALSE, viewMatrix.data);
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "promatrix"), 1, GL_TRUE, projectionMatrix.data);
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "modmatrix"), 1, GL_TRUE, modelMatrix.data);
		glUniform1i(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "tex"), 0);

		glDrawArrays(GL_LINES, 0, components.Size() * 2);

		Color4f* colors2 = new Color4f[components.Size() * 2];
		for (uint i = 0; i < components.Size() * 2; i++) {
			colors2[i] = Color4f(1.0, 1.0, 1.0, colors[i].a);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Color4f) * components.Size() * 2, colors2);
		texture = (Texture2D*)GEngine::Get().Resource().Get_Resource("sparkparticle.tga");
		texture->Activate();
		glDrawArrays(GL_LINES, 0, components.Size() * 2);

		texture->Deactivate();

		ShaderProgram::Deactivate();

		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glDepthMask(1);
	}
}
/*
CameraShake::CameraShake(double mag, double period, double decrate) :
RenderComponent	(RenderType::beforecam | RenderType::first),
MechanicalComponent	(0.01),
progressTracker		(mag / decrate, true),
mag			(mag),
period		(period),
decrate		(decrate),
offset		() 
{}

CameraShake::~CameraShake() 
{}

void CameraShake::Render(Flags callflags) {
	if (callflags & RenderType::beforecam) {
		if (progressTracker.Is_Done()) {
			delete this;
			return;
		}
		offset = Vector3d(sin(progressTracker.accumulation * 2.0 * PI / period), 0, 0) * progressTracker.Lin_Interp<double>(mag, 0.0);
		offset = GEngine::Get().Render().Camera().transform.orientation.S_To_O(offset);
		GEngine::Get().Render().Camera().transform.position += offset;
	}
	else
		GEngine::Get().Render().Camera().transform.position -= offset;
}

void CameraShake::Update(double t) {
	progressTracker.Update(t);
}
*/


