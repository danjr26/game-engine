#include "projectiles.h"
#include "framework.h"

Laser::Laser(Entity* parent, LaserSettings& settings, Transform3d& transform) :
Entity(transform, Identity::laser, Allegiance::human,
	((Mesh*)GEngine::Get().Resource().Get_Resource("laser.obj"))->tree, 
	0.005),
parent			(parent),
settings		(settings),
timeout			(settings.timeout),
visual			(transform, 3.0, 1.5, Color4f(0.0, 0.25, 1.0, 1.0)) {
	velocity = transform.orientation.forward * settings.speed;
}

Laser::~Laser() 
{}

void Laser::Update(double t) {
	transform.position += transform.orientation.forward * settings.speed * t;
	timeout -= t;
	visual.transform = transform;
	if (timeout <= 0)
		delete this;
}

bool Laser::Collide(CollidableComponent* that, Collision* collision) {
	if (that->type == Identity::laser || that->type == Identity::pulse || that->type == Identity::explosionblast || that->type == Identity::playership)
		return false;
	LaserRecoil(collision->collisionPoint, transform.orientation.forward);
	return true;
}

bool Laser::Should_Check_Collision(CollidableComponent* that) {
	switch (that->type) {
	case Identity::laser:
	case Identity::pulse:
		return false;
	default:
		return true;
	}
}

Missile::Missile(Entity* parent, Entity* target, MissileSettings& settings, Transform3d& transform) :
NPC(transform, Identity::missile, Allegiance::human,
	((Mesh*)GEngine::Get().Resource().Get_Resource("missile.obj"))->tree, 
	0.01f, 0.5f),
parent		(parent),
settings	(settings),
timeout		(settings.timeout) {
	mesh = new MeshInstance((Mesh*)GEngine::Get().Resource().Get_Resource("missile.obj"));
	mesh->transform = transform;
}

Missile::~Missile() {
	
}

void Missile::Update(double t) {
//	CollidableComponent::stepper.step = 2.0 / velocity.Magnitude();
	/*
	double selflinmag = selfvel.Magnitude();

	if (tgtspeed > selflinmag)
		selflinmag += settings.accel * t;

	//if (tgtspeed < selflinmag)
	//	selflinmag -= settings.thrust / settings.mass * t;

	if (selflinmag > settings.maxspeed)
		selflinmag = settings.maxspeed;

	selfvel = transform.orientation.forward * selflinmag;

	if (vel.Normalized().Dot(tgtdir) < 0.999f)
		selfrotvel = NPC::Calculate_Rotation(transform.orientation.forward, tgtdir).Normalized() * settings.turnrate;

	double othervelmag = othervel.Magnitude();
	othervelmag -= 5.0 * t;
	if (othervelmag < 0.0)
		othervelmag = 0.0;
	othervel = othervel.Normalized() * othervelmag;

	vel = selfvel + othervel;
	rotationalVelocity = selfrotvel + otherrotvel;
	transform.orientation.Rotate(rotationalVelocity * t);
	selfvel.Rotate(rotationalVelocity * t);
	transform.position += vel * t;

	mesh->transform = transform;
	Update_Oct_Coords();

	if (timeout.Step_Number(t)) {
		MissileExplosion(transform.position);
		delete this;
		return;
	}*/
}

bool Missile::Collide(CollidableComponent* that, Collision* collision) {
	MissileExplosion(transform.position);
	return true;
}

void Missile::(double t) {/*
	Vector3d tgt = ai.Refresh();
	if (tgt.Is_Zero())
		tgt = transform.orientation.forward;
	tgtdir = tgt.Normalized();
	tgtspeed = tgt.Magnitude();*/
}

Pulse::Pulse(Entity* parent, PulseSettings& settings, Transform3d& transform) :
Entity(transform, Identity::pulse, Allegiance::alien,
	((Mesh*)GEngine::Get().Resource().Get_Resource("pulse.obj"))->tree, 
	0.01f),
parent	(parent),
settings(settings),
timeout	(settings.timeout),
visual  (transform, 2.0, 0.25, Color4f(0.1, 1.0, 0.2, 1.0)) {
	velocity = transform.orientation.forward * settings.speed;
}

Pulse::~Pulse() 
{}

void Pulse::Update(double t) {
//	CollidableComponent::stepper.step = 0.75 / settings.speed;
	transform.orientation.up.Rotate(1000.0f * t, transform.orientation.forward);
	transform.position += transform.orientation.forward * settings.speed * t;
	timeout -= t;
	visual.transform = transform;
	if (timeout <= 0)
		delete this;
}

bool Pulse::Collide(CollidableComponent* that, Collision* collision) {
	if (that->type == Identity::playership && ((PlayerShip*)that)->firstperson == true)
		return true;
	PulseRecoil(collision->collisionPoint, transform.orientation.forward);
	return true;
}

bool Pulse::Should_Check_Collision(CollidableComponent* that) {
	switch (that->type) {
	case Identity::laser:
	case Identity::pulse:
		return false;
	default:
		return true;
	}
}

ExplosionBlast::ExplosionBlast(Transform3d transform, float expansion, float lifet,
	float damagek, float forcek) :
MechanicalComponent(0.01),
CollidableComponent(Identity::explosionblast, new CollisionMask(&this->transform, &this->vel, nullptr)),
expansion	(expansion),
damagek		(damagek),
forcek		(forcek),
life		(lifet),
transform	(transform) {
	SphereNode* s = new SphereNode;
	s->localCenter = Vector3f();
	s->radius = 0.0f;
	s->child1 = nullptr;
	s->child2 = nullptr;
	s->tri = nullptr;
	mask->tree = new SphereTree(s, 1, 0.0f);
}

ExplosionBlast::~ExplosionBlast() {
	delete mask->tree;
}

void ExplosionBlast::Update(double t) {
	if (life.Step_Number(t)) {
		delete this;
		return;
	}
	mask->tree->trunk->radius += expansion * t;
}

bool ExplosionBlast::Collide(CollidableComponent* that, Collision* collision) {
	return false;
}

float ExplosionBlast::Get_Damage(float d) {
	float damage = damagek / (d * d);
	if (damage > damagek * 2)
		damage = damagek * 2;
	return damage;
}

float ExplosionBlast::Get_Force(float d) {
	float force = forcek / (d * d);
	if (force > forcek * 2)
		force = forcek * 2;
	return force;
}

void MissileExplosion(Vector3d position) {/*
	EmitterSettings sparkssettings = {
		500,
		position, 0.0f,
		24.0f, 24.0f,
		0.0f, 0.0f,
		0.5f, 0.5f,
		(Texture2D*)GEngine::Get().Resource().Get_Resource("spark.tga"),
		Vector2f(1.0f, 1.0f), 5.5, 5.8,
		Color4f(0.5f, 0.75f, 1.0f, 1.0f),
		Color4f(0.1f, 0.2f, 1.0f, 0.0f)
	};
	EmitterSettings smokesettings = {
		100,
		position, 4.0f,
		0.0f, 4.0f,
		0.0f, 90.0f,
		3.0f, 4.5f,
		(Texture2D*)GEngine::Get().Resource().Get_Resource("smoke.tga"),
		Vector2f(1.0f, 1.0f), 2.0, 4.0,
		Color4f(0.95f, 0.95f, 1.0f, 0.02f),
		Color4f(0.0f, 0.0f, 0.0f, 0.0f)
	};
	EmitterSettings flashsettings = {
		2,
		position, 5.0f,
		0.0f, 2.0f,
		0.0f, 360.0f,
		0.1f, 0.25f,
		(Texture2D*)GEngine::Get().Resource().Get_Resource("flash.tga"),
		Vector2f(1.0f, 1.0f), 10.0, 20.0,
		Color4f(1.0f, 1.0f, 1.0f, 0.75f),
		Color4f(1.0f, 1.0f, 1.0f, 0.0f)
	};

	new Emitter(sparkssettings, 1.0f / 30.0f);
	new Emitter(smokesettings, 1.0f / 30.0f);
	new ExplosionBlast(position, 100.0f, 0.12f, 2.0f, 20000.0f);
	//new Emitter(flashsettings, 1.0f / 30.0f);*/
}

void LaserRecoil(Vector3d position, Vector3d forward) {
	EmitterSettings eslaserrecoil = {
		new LaserRecoilGen(),
		nullptr,
		Transform3d(position),
		(Texture2D*)GEngine::Get().Resource().Get_Resource("glow.tga"),
		BlendSettings{
		true,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		20,
		20,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		FACE_CAM,
		true
	};

	new Emitter(eslaserrecoil, 0.01);
}

void PulseRecoil(Vector3d position, Vector3d forward) {
	EmitterSettings espulserecoil = {
		new PulseRecoilGen(),
		nullptr,
		Transform3d(position),
		(Texture2D*)GEngine::Get().Resource().Get_Resource("explosion.tga"),
		BlendSettings{
		true,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		15,
		15,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		FACE_CAM,
		true
	};

	new Emitter(espulserecoil, 0.01);
}