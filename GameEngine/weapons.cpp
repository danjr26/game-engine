#include "weapons.h"
#include "framework.h"

Weapon::Weapon(Entity* parent, Transform3d transform) :
parent		(parent),
transform	(transform),
stepper		(0.0f),
ready		(false),
subjloc		(true)
{}

Weapon::~Weapon()
{}

void Weapon::Unready() {
	ready = false;
	stepper.accumulation = 0.0f;
}

void Weapon::Fire() 
{}

void Weapon::Update(double t) {
	if (stepper.Step_Number(t))
		ready = true;
}

LaserGun::LaserGun(Entity* parent, Transform3d& transform, LaserGunSettings& settings) :
Weapon(parent, transform),
settings(settings) {
	stepper.step = 1.0f / settings.firerate;
}

LaserGun::~LaserGun()
{}

void LaserGun::Fire() {
	Laser* laser = new Laser(parent, settings.laser, parent->transform.S_To_O(transform));
	//laser->transform.orientation.Rotate(Vector3d::Random_Normalized() * Rand_Float(0, 1));
	LaserFireFlash(parent->transform.S_To_O(transform).position);
	Unready();
}

MissileLauncher::MissileLauncher(Entity* parent, Transform3d& transform, MissileLauncherSettings& settings)
: Weapon(parent, transform),
settings(settings) {
	stepper.step = 1.0f / settings.firerate;
}

MissileLauncher::~MissileLauncher()
{}

Entity* MissileLauncher::Get_Target() {
	Entity* target = nullptr;
	float bestd = -1.0f;
	for (int i = 0; i < GEngine::Get().AI().Entities().List().Size(); i++) {
		if (GEngine::Get().AI().Entities().List()[i]->type == Identity::swarmdroid &&
			(GEngine::Get().AI().Entities().List()[i]->transform.position - parent->transform.position).Normalized().Dot(parent->transform.orientation.forward) > bestd) {
			target = GEngine::Get().AI().Entities().List()[i];
			bestd = (GEngine::Get().AI().Entities().List()[i]->transform.position - parent->transform.position).Normalized().Dot(parent->transform.orientation.forward);
		}
	}
	return target;
}

void MissileLauncher::Fire() {
	Entity* target = Get_Target();
	Transform3d mtransform = parent->transform.S_To_O(transform);
	Missile* m = new Missile(parent, target, settings.missile, mtransform);
	Unready();
}

PulseGun::PulseGun(Entity* parent, Transform3d& transform, PulseGunSettings& settings)
: Weapon(parent, transform),
settings(settings) {
	stepper.step = 1.0f / settings.firerate;
}

PulseGun::~PulseGun()
{}

void PulseGun::Fire() {
	Transform3d t = (subjloc) ? parent->transform.S_To_O(transform) : transform;
	new Pulse(parent, settings.pulse, t);
	PulseFireFlash(t.position);
	Unready();
}

void LaserFireFlash(Vector3d position) {
	EmitterSettings esflash = {
		new LaserFlashGen(),
		nullptr,
		Transform3d(position),
		(Texture2D*)GEngine::Get().Resource().Get_Resource("spark.tga"),
		BlendSettings{
		true,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		2,
		2,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		FACE_CAM,
		true
	};

	new Emitter(esflash, 0.01);
}

void PulseFireFlash(Vector3d position) {
	EmitterSettings esflash = {
		new PulseFlashGen(),
		nullptr,
		Transform3d(position),
		(Texture2D*)GEngine::Get().Resource().Get_Resource("explosion.tga"),
		BlendSettings{
		true,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		2,
		2,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		FACE_CAM,
		true
	};

	new Emitter(esflash, 0.01);
}