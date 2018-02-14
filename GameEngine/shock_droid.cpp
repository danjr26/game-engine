#include "shock_droid.h"
#include "framework.h"

ShockDroidPropulsion::ShockDroidPropulsion(ShockDroid* parent, ShipPropulsionSettings& settings) :
	ShipSystem(parent, &settings.shipsys),
	settings(settings)
{}

void ShockDroidPropulsion::Update(double t) {
	Vector3d accel = parent->ai.tgtdir * parent->ai.tgtspeed - parent->velocity;
	accel.Normalize();
	accel *= 40.0f;

	parent->velocity += accel * t;
	//parent->rotationalVelocity = parent->ai.tgtdir * parent->velocity.Magnitude() * 100.0;

	if (accel.Dot(parent->velocity) < 0 && parent->velocity.Magnitude() < 1.0) {
		parent->velocity = Vector3d();
	}

	/*if (parent->velocity.Is_Zero()) {
		parent->rotationalVelocity = parent->transform.orientation.forward * 180.0;
	}*/

	parent->transform.orientation.Rotate(parent->rotationalVelocity * t);
	parent->velocity.Rotate(parent->rotationalVelocity * t);
	parent->transform.position += parent->velocity * t;
}

ShockDroidPulseGun::ShockDroidPulseGun(ShockDroid* parent, ShipPulseGunSettings& settings) :
ShipSystem(parent, &settings.shipsys),
settings(settings),
gun		(parent, Transform3d(Vector3d(0, 0, 0)), settings.gun) {
	gun.subjloc = false;
}

void ShockDroidPulseGun::Update(double t) {
	gun.Update(t);
	for (int i = 0; i < GEngine::Get().AI().Entities().List().Size(); i++) {
		Entity* e = GEngine::Get().AI().Entities().List()[i];
		if (e->allegiance == parent->allegiance || e->allegiance == Allegiance::neutral)
			continue;

		Vector3d target = (e->transform.position + e->velocity * (e->transform.position - parent->transform.position).Magnitude() / settings.gun.pulse.speed) - parent->transform.position;
		Vector3d targetn = target.Normalized();
		gun.transform.position =	parent->transform.position + (targetn - (parent->transform.orientation.up * parent->transform.orientation.up.Dot(targetn))).Normalized() * 4.0;
		gun.transform.orientation.Rotate(gun.transform.orientation.forward.Cross(targetn) * -gun.transform.orientation.forward.Theta(targetn));

		if (fabs(targetn.Dot(parent->transform.orientation.up)) < 0.5 && parent->velocity.Is_Zero() && gun.ready) {
			Fire();
			break;
		}
	}
}

void ShockDroidPulseGun::Fire() {
	gun.Fire();
}

ShockDroidReactor::ShockDroidReactor(ShockDroid* parent, ShipReactorSettings& settings)
	: ShipSystem<ShockDroid>(parent, &settings.shipsys),
	settings(settings)
{}

void ShockDroidReactor::Update(double t)
{}

ShockDroid::ShockDroid(Transform3d transform, ShockDroidSettings& settings) :
	NPC(transform, Identity::shockdroid, Allegiance::alien, ((Mesh*)GEngine::Get().Resource().Get_Resource("shock_droid.obj"))->tree,
		0.01f, 0.1),
	settings(settings),
	reactor(this, settings.reactor),
	propulsion(this, settings.propulsion),
	pulsegun(this, settings.pulsegun),
	ai(this),
	spawn(1.0),
	spawned(false) {

	mesh = new MeshInstance((Mesh*)GEngine::Get().Resource().Get_Resource("shock_droid.obj"));

	GEngine::Get().Collision().Remove(this);
	GEngine::Get().AI().Remove(this);
	GEngine::Get().AI().Entities().Remove(this);
	GEngine::Get().Render().Remove(mesh);

	new AlienEntrance(transform.position, mask->tree->trunk->radius);
}

ShockDroid::~ShockDroid() {
	delete mesh;
}

void ShockDroid::Update(double t) {
	if (!spawned) {
		if (spawn.Step_Number(t)) {
			GEngine::Get().Collision().Add(this);
			GEngine::Get().AI().Add(this);
			GEngine::Get().AI().Entities().Add(this);
			GEngine::Get().Render().Add(mesh);
			spawned = true;
		}
		else
			return;
	}
//	CollidableComponent::stepper.step = 2.5 / velocity.Magnitude();
	reactor.Update(t);
	propulsion.Update(t);
	pulsegun.Update(t);
	mesh->transform = transform;
}

bool ShockDroid::Collide(CollidableComponent* that, Collision* collision) {
	/*switch (that->type) {
	case TYPE_PULSE:
		if (((Pulse*)that)->parent == (Entity*)this)
			return false;
		Take_Damage(((Pulse*)that)->settings.damage);
		break;
	case TYPE_LASER:
		Take_Damage(((Laser*)that)->settings.damage);
		break;
	case TYPE_SWARM_DROID:
		reactor.efficiency = 0.0;
		break;
	case TYPE_PLAYER_SHIP:
		reactor.efficiency = 0.0;
		break;
	case TYPE_ASTEROID:
		reactor.efficiency = 0.0;
		break;
	case TYPE_EXPLOSION_BLAST:
		reactor.efficiency = 0.0;
		break;
	case TYPE_MISSILE:
		reactor.efficiency = 0.0;
		break;
	}*/

	if (reactor.efficiency == 0.0) {
		DefaultExplosion(transform.position, 2.0);
		return true;
	}
	return false;
}


void ShockDroid::(double t) {
	ai.();
}


void ShockDroid::Take_Damage(float damage) {
	float damages[4];
	for (int i = 0; i < 3; i++)
		damages[i] = damage;
	for (int i = 0; i < 30; i++) {
		int n1 = rand() % 3;
		int n2 = rand() % 3;
		float f = Rand_Float(0.0f, damages[n1]);
		damages[n1] -= f;
		damages[n2] += f;
	}
	reactor.Take_Damage(damages[0]);
	propulsion.Take_Damage(damages[1]);
	pulsegun.Take_Damage(damages[2]);
}
