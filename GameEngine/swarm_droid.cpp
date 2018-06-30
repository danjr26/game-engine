#include "swarm_droid.h"
#include "framework.h"

SwarmDroidPropulsion::SwarmDroidPropulsion(SwarmDroid* parent, ShipPropulsionSettings& settings) :
ShipSystem	(parent, &settings.shipsys),
settings	(settings)
{}

void SwarmDroidPropulsion::Update(double t) {
	if (parent->defvel + parent->fbvel > parent->ai.tgtspeed)
		parent->fbvel -= 30.0 * t;
	if (parent->defvel + parent->fbvel < parent->ai.tgtspeed)
		parent->fbvel += 30.0 * t;
	parent->fbvel = Clamp(parent->fbvel, -20.0f, 0.0f);

	parent->velocity = parent->transform.orientation.forward * (parent->defvel + parent->fbvel);
	parent->rotationalVelocity = Calculate_Rotation(parent->transform.orientation.forward, parent->ai.tgtdir).Normalized() * 180.0;//parent->ai.tgtdir.Cross(parent->transform.orientation.forward).Normalized() * 180.0f;

	parent->transform.orientation.Rotate(parent->rotationalVelocity * t);
	parent->velocity.Rotate(parent->rotationalVelocity * t);
	parent->transform.position += parent->velocity * t;
}

SwarmDroidPulseGun::SwarmDroidPulseGun(SwarmDroid* parent, ShipPulseGunSettings& settings) :
ShipSystem(parent, &settings.shipsys),
settings	(settings),
gun			(parent, Transform3d(Vector3d(0.0, 0.0, -3.70)), settings.gun)
{}

void SwarmDroidPulseGun::Update(double t) {
	gun.Update(t);
	for (int i = 0; i < GEngine::Get().AI().Entities().List().Size(); i++) {
		Entity* e = GEngine::Get().AI().Entities().List()[i];
		if (e->allegiance == parent->allegiance || e->allegiance == Allegiance::neutral)
			continue;
		Vector3d target = (e->transform.position + e->velocity * (e->transform.position - parent->transform.position).Magnitude() / settings.gun.pulse.speed) - parent->transform.position;
		if (target.Normalized().Dot(parent->transform.orientation.forward) > 0 && 
			tanf(acosf(target.Normalized().Dot(parent->transform.orientation.forward))) * target.Magnitude() < e->mask->tree->trunk->radius + 2.0 && 
			gun.ready) {

			Fire();
			break;
		}
	}
}

void SwarmDroidPulseGun::Fire() {
	gun.Fire();
}

SwarmDroidReactor::SwarmDroidReactor(SwarmDroid* parent, ShipReactorSettings& settings) :
ShipSystem<SwarmDroid>(parent, &settings.shipsys),
settings(settings) 
{}

void SwarmDroidReactor::Update(double t) 
{}

SwarmDroid::SwarmDroid(Transform3d transform, SwarmDroidSettings& settings) :
NPC(transform, Identity::swarmdroid, Allegiance::alien, ((Mesh*)GEngine::Get().Resource().Get("swarm_droid_2.obj"))->tree, 
	0.01f, 0.1),
settings	(settings),
reactor		(this, settings.reactor),
propulsion	(this, settings.propulsion),
pulsegun	(this, settings.pulsegun),
ai			(this),
spawn		(1.0),
spawned		(false),
fbvel		(0.0f),
defvel		(0.0f) {
	mesh = new MeshInstance((Mesh*)GEngine::Get().Resource().Get("swarm_droid_2.obj"));

	GEngine::Get().Collision().		Remove(this);
	GEngine::Get().AI().			Remove(this);
	GEngine::Get().AI().Entities().	Remove(this);
	GEngine::Get().Render().		Remove(mesh);

	new AlienEntrance(transform.position, mask->tree->trunk->radius);
}

SwarmDroid::~SwarmDroid() {
	delete mesh;
}

void SwarmDroid::Update(double t) {
	if (!spawned) {
		if (spawn.Step_Number(t)) {
			GEngine::Get().Collision().		Add(this);
			GEngine::Get().AI().			Add(this);
			GEngine::Get().AI().Entities().	Add(this);
			GEngine::Get().Render().		Add(mesh);
			spawned = true;
		} 
		else
			return;
	}
//	CollidableComponent3::stepper.step = 2.5 / velocity.Magnitude();
	reactor.	Update(t);
	propulsion.	Update(t);
	pulsegun.	Update(t);
	mesh->transform = transform;
}

bool SwarmDroid::Collide(CollidableComponent3* that, Collision* collision) {
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
		DefaultExplosion(transform.position, 1.2);
		return true;
	}
	return false;
}

void SwarmDroid::(double t) {
	ai.();
}

void SwarmDroid::Take_Damage(float damage) {
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
	reactor.	Take_Damage(damages[0]);
	propulsion.	Take_Damage(damages[1]);
	pulsegun.	Take_Damage(damages[2]);
}

void DefaultExplosion(Vector3d position, float size) {
	EmitterSettings essmoke = {
		new ExplosionSmokeGen(size),
		nullptr,
		Transform3d(position),
		(Texture2*)GEngine::Get().Resource().Get("explosion.tga"),
		BlendSettings{
		true,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		10,
		10,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		FACE_CAM,
		true
	};
	EmitterSettings esflame = {
		new ExplosionFlameGen(size),
		nullptr,
		Transform3d(position),
		(Texture2*)GEngine::Get().Resource().Get("explosion.tga"),
		BlendSettings{
		true,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		15,
		15,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		FACE_CAM,
		true
	};
	EmitterSettings esflash = {
		new ExplosionFlashGen(size),
		nullptr,
		Transform3d(position),
		(Texture2*)GEngine::Get().Resource().Get("explosion.tga"),
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
	EmitterSettings essparks = {
		new ExplosionSparksGen(size),
		nullptr,
		Transform3d(position),
		(Texture2*)GEngine::Get().Resource().Get("explosion.tga"),
		BlendSettings{
		true,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		10,
		10,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		FACE_CAM,
		true
	};
	EmitterSettings essparktrails = {
		new ExplosionSparkTrailsGen(size),
		nullptr,
		Transform3d(position),
		(Texture2*)GEngine::Get().Resource().Get("explosion.tga"),
		BlendSettings{
		true,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		20,
		20,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		FACE_CAM_RES,
		true
	};
	EmitterSettings essmoketrails = {
		new ExplosionSmokeTrailsGen(size),
		nullptr,
		Transform3d(position),
		(Texture2*)GEngine::Get().Resource().Get("explosion.tga"),
		BlendSettings{
		true,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		20,
		20,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		FACE_CAM_RES,
		true
	};

	new Emitter(essmoke, 0.01);
	new Emitter(esflame, 0.01);
	new Emitter(esflash, 0.01);
	new Emitter(essparks, 0.01);
	new Emitter(essparktrails, 0.01);
	new Emitter(essmoketrails, 0.01);

}

