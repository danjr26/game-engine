#include "player_ship.h"
#include "framework.h"

PlayerShipLaserGuns::PlayerShipLaserGuns(PlayerShip* parent, ShipLaserGunSettings& settings) :
ShipSystem<PlayerShip>(parent, &settings.shipsys),
settings	(settings),
lgun		(parent, Transform3d(Vector3f(-1.68f, 0.20f, -1.55f)), settings.gun),
rgun		(parent, Transform3d(Vector3f(1.68f, 0.20f, -1.55f)), settings.gun),
alternater	(0.5 / settings.gun.firerate),
left		(true),
altready	(true),
ready		(true)
{}

void PlayerShipLaserGuns::Update(double t) {
	if (efficiency == 0.0)
		return;

	float step = Fire_Rate();

	alternater.step = step * 0.5;

	lgun.stepper.step = step;
	rgun.stepper.step = step;

	lgun.Update(t);
	rgun.Update(t);

	if (alternater.Step_Number(t))
		altready = true;

	ready = altready && ((left) ? lgun.ready : rgun.ready);

	if (ready && (GetKeyState(VK_LBUTTON) & 0x8000))
		Fire();
}

void PlayerShipLaserGuns::Fire() {
	Vector2d	mouse =		Get_Mouse_Coords_Gradient	();
	double		mousemag =	mouse.Magnitude				();

	if (mousemag > 0.25) {
		mouse /= mousemag;
		mouse *= 0.25;
	}

	//GEngine::Get().Render().Camera().transform.orientation.Calculate_Right();

	Vector3d shiprelpos = Vector3d(
		GEngine::Get().Render().Camera().transform.orientation.right.	Dot(parent->transform.position - GEngine::Get().Render().Camera().transform.position),
		GEngine::Get().Render().Camera().transform.orientation.up.	Dot(parent->transform.position - GEngine::Get().Render().Camera().transform.position),
		GEngine::Get().Render().Camera().transform.orientation.forward.	Dot(parent->transform.position - GEngine::Get().Render().Camera().transform.position) * -1.0
	);

	Vector3d farproj = Vector3d(
		mouse.x * GEngine::Get().Render().Camera().projection.right / GEngine::Get().Render().Camera().projection.close * GEngine::Get().Render().Camera().projection.distant,
		mouse.y * GEngine::Get().Render().Camera().projection.top   / GEngine::Get().Render().Camera().projection.close * GEngine::Get().Render().Camera().projection.distant,
		GEngine::Get().Render().Camera().projection.distant * -1.0) - shiprelpos /*- parent->transform.S_To_O(lgun.transform.position + rgun.transform.position) * 0.5*/;

 	farproj.Normalize();

	if (left) {
		Orientation3d orientation = lgun.transform.orientation;
		lgun.transform.orientation.Rotate(Calculate_Rotation(lgun.transform.orientation.forward, farproj));
		lgun.Fire();
		lgun.transform.orientation = orientation;
		left = false;
	}
	else {
		Orientation3d orientation = rgun.transform.orientation;
		rgun.transform.orientation.Rotate(Calculate_Rotation(rgun.transform.orientation.forward, farproj));
		rgun.Fire();
		rgun.transform.orientation = orientation;
		left = true;
	}

	altready = false;
	ready = false;
	alternater.accumulation = 0.0;

	if (GetKeyState(VK_SPACE) & 0x8000)
		Die("");
}

float PlayerShipLaserGuns::Fire_Rate() {
	return 1.0f / (settings.gun.firerate * efficiency);
}

PlayerShipMissileLauncher::PlayerShipMissileLauncher(PlayerShip* parent, ShipMissileLauncherSettings& settings) :
ShipSystem<PlayerShip>	(parent, &settings.shipsys),
settings				(settings),
launcher				(parent, Transform3d(Vector3d(0, -2.0, 0)), settings.launcher),
nmissiles				(settings.nmissiles)
{}

void PlayerShipMissileLauncher::Update(double t) {
	if (efficiency == 0.0)
		return;

	launcher.Update(t);

	ready = launcher.ready && nmissiles >= 0;

	if (ready && GetKeyState('E') & 0x8000)
		Prime();

	if (primed && !(GetKeyState('E') & 0x8000)) {
		Fire();
		Unprime();
	}
}

void PlayerShipMissileLauncher::Prime() {
	primed = true;
}

void PlayerShipMissileLauncher::Unprime() {
	primed = false;
}

void PlayerShipMissileLauncher::Fire() {
	if (Jam_Chance() > Rand_Float(0.0f, 1.0f)) {
		Jam();
		return;
	}
	launcher.Fire();
	nmissiles--;
	ready = false;
}

float PlayerShipMissileLauncher::Jam_Chance() {
	float j = 1.0f - efficiency;
	float jamchance = settings.jammult * j * j;
	if (jamchance < 0.0f)
		jamchance = 0.0f;
	if (jamchance > 1.0f)
		jamchance = 1.0f;
	return jamchance;
}

void PlayerShipMissileLauncher::Jam() {
	efficiency = 0.0f;
}

PlayerShipPropulsion::PlayerShipPropulsion(PlayerShip* parent, ShipPropulsionSettings& settings) :
ShipSystem<PlayerShip>(parent, &settings.shipsys),
settings(settings),
fbvel	(0.0),
rlvel	(0.0),
defvel	(0.0) 
{}

void PlayerShipPropulsion::Update(double t) {
	if (GetKeyState('W') & 0x8000) {
		if (GetKeyState('S') & 0x8000) {
			if (fbvel > 0.0f)
				fbvel = (float)Clamp_Floor(fbvel - 10.0 * t, 0.0);
			else
				fbvel = (float)Clamp_Ceiling(fbvel + 10.0 * t, 0.0);
		} 
		else
			fbvel += (float)(20.0 * t * efficiency);
	}
	else {
		if (GetKeyState('S') & 0x8000)
			fbvel -= (float)(20.0 * t * efficiency);
		else {
			if (fbvel > 0.0f)
				fbvel = (float)Clamp_Floor(fbvel - 10.0 * t, 0.0);
			else
				fbvel = (float)Clamp_Ceiling(fbvel + 10.0 * t, 0.0);
		}
	}
	
	fbvel = Clamp(fbvel, -10.0f, 50.0f);

	if (GetKeyState('D') & 0x8000) {
		if (GetKeyState('A') & 0x8000) {
			if (rlvel > 0.0f)
				rlvel = (float)Clamp_Floor(rlvel - 30.0 * t, 0.0);
			else
				rlvel = (float)Clamp_Ceiling(rlvel + 30.0 * t, 0.0);
		}
		else
			rlvel += (float)(60.0 * t * efficiency);
	}
	else {
		if (GetKeyState('A') & 0x8000)
			rlvel -= (float)(60.0 * t * efficiency);
		else {
			if (rlvel > 0.0f)
				rlvel = (float)Clamp_Floor(rlvel - 30.0 * t, 0.0);
			else
				rlvel = (float)Clamp_Ceiling(rlvel + 30.0 * t, 0.0);
		}
	}

	rlvel = Clamp(rlvel, -30.0f, 30.0f);

	//parent->transform.orientation.Calculate_Right();
	parent->velocity = parent->transform.orientation.forward * (defvel + fbvel) + parent->transform.orientation.right * rlvel;

	Vector2d mc2 = Get_Mouse_Coords_Gradient();
	Vector3d mc3 = Vector3d(mc2.x, mc2.y, 0);

	if (!mc3.Is_Zero()) {
		double mc3mag = mc3.Magnitude();

		if (mc3mag > 1.0f) {
			mc3 /= mc3mag;
			mc3mag = 1.0f;
		}

		Vector3d axis = Vector3d(0, 0, -1).Cross(mc3 / mc3mag);
		axis = parent->transform.orientation.S_To_O(axis);

		double angle;

		if (mc3mag < 0.25f)
			angle = 0.0;
		else
			angle = 4.0 / 3.0 * (mc3mag - 0.25) * settings.maxturnrate;

		parent->rotationalVelocity = axis * angle;
	}
	else
		parent->rotationalVelocity = Vector3d();

	parent->velocity *= efficiency;
	parent->rotationalVelocity *= efficiency;

	parent->transform.orientation.Rotate(parent->rotationalVelocity * t);
	parent->velocity.Rotate(parent->rotationalVelocity * t);
	parent->transform.position += parent->velocity * t;
}

PlayerShipReactor::PlayerShipReactor(PlayerShip* parent, ShipReactorSettings& settings) :
ShipSystem<PlayerShip>(parent, &settings.shipsys),
settings(settings)
{}

void PlayerShipReactor::Update(double t)
{}

PlayerShipRepairer::PlayerShipRepairer(PlayerShip* parent, ShipRepairerSettings& settings) :
ShipSystem<PlayerShip>(parent, &settings.shipsys),
settings(settings),
trans	(settings.startup, settings.cooldown)
{}

void PlayerShipRepairer::Update(double t) {
	trans.Update(t);
	Repair(parent->laserguns.efficiency, settings.lasergunrate, t);
	Repair(parent->missilelauncher.efficiency, settings.missilelauncherrate, t);
	Repair(parent->propulsion.efficiency, settings.propulsionrate, t);
	Repair(parent->repairer.efficiency, settings.repairerrate, t);
	Repair(parent->shield.efficiency, settings.shieldrate, t);
}

void PlayerShipRepairer::Repair(float& sysefficiency, float sysrate, double t) {
	sysefficiency += Clamp(Repair_Amount(sysefficiency, sysrate) * (float)t, 0.0f, Clamp_Floor(settings.maxrepair - sysefficiency, 0.0f));
}

float PlayerShipRepairer::Repair_Amount(float sysefficiency, float sysrate) {
	float diff = settings.maxrepair - sysefficiency;
	if (diff > 0.0f)
		return trans.Lin_Interp(0.0f, diff * diff * sysrate * efficiency);
	else
		return 0.0f;
}

PlayerShipShield::PlayerShipShield(PlayerShip* parent, ShipShieldSettings& settings) :
ShipSystem<PlayerShip>	(parent, &settings.shipsys),
settings				(settings),
visual					(nullptr),
trans					(settings.startup, settings.cooldown) {
	MeshBlanketSettings mbsettings = {
		new PlayerShipShieldMesBlaGen(),
		parent->mesh,
		(Texture2*)GEngine::Get().Resource().Get("glow.tga"),
		BlendSettings {
			true,
			GL_ONE_MINUS_SRC_ALPHA,
			GL_SRC_ALPHA,
			GL_FUNC_ADD
		},
	500,
		{0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0}
	};
	visual = new MeshBlanket(mbsettings, 0.01);
	trans.Toggle();
}

PlayerShipShield::~PlayerShipShield() {
	delete visual;
}

void PlayerShipShield::Update(double t) {
	return;
	visual->transform = parent->mesh->transform;
	trans.Update(t);

	if ((GetKeyState('Q') & 0x8000) && !trans.Is_Transitioning() && 
		!parent->repairer.trans.Is_Transitioning()) {
		trans.Toggle();
		parent->repairer.trans.Toggle();
	}

	/*visual->data.Set<Color4f>(BBPS_COLOR, 0, 
		trans.Lin_Interp(Color4f(0.0f, 0.0f, 0.0f, 0.0f), visual->settings.color));*/

	for (int i = 0; i < visual->settings.npcls; i++) {
		visual->data.Set<Color4f>(PS_COL, i, 
			visual->data.Get<Color4f>(PS_COL, i) + 
			(Color4f(0.8f, 0.8f, 1.0f, 0.01f) - visual->data.Get<Color4f>(PS_COL, i)) * (float)(5.0 * t));
	}
}

void PlayerShipShield::Get_Hit(Vector3d position, float intensity) {
	intensity *= intensity;
	for (int i = 0; i < visual->settings.npcls; i++) {
		Vector3d d = visual->transform.S_To_O(visual->data.Get<Vector3f>(PS_LOC, i)) - position;
		float d2 = Clamp_Floor<float>(intensity - (float)d.Dot(d), 0.0f) / intensity;
		visual->data.Set<Color4f>(PS_COL, i, visual->data.Get<Color4f>(PS_COL, i) + Color4f(0.0f, 0.0f, 0.0f, 0.4f) * d2);
	}
}

float PlayerShipShield::Mod_Damage(float damage) {
	return trans.Lin_Interp(damage, damage * (1.0f - ((1.0f - settings.damagemult) * efficiency)));
}

PlayerShip::PlayerShip(Transform3d& transform, PlayerShipSettings& settings) :
Entity(transform,
Identity::playership, Allegiance::human,
((Mesh*)GEngine::Get().Resource().Get("test1.obj"))->tree, 
 0.01),
mesh			(new MeshInstance((Mesh*)GEngine::Get().Resource().Get("test1.obj"))),
settings		(settings),
reactor			(this, settings.reactor),
shield			(this, settings.shield),
propulsion		(this, settings.propulsion),
repairer		(this, settings.repairer),
laserguns		(this, settings.lasergun),
missilelauncher	(this, settings.missilelauncher),
firstperson		(false) {

	new PlayerShipSpeedometer(this, 0.25);
	new PlayerShipSystemsStatus(this, 0.5);
	GEngine::Get().Render().Camera().transform = transform;
	GEngine::Get().Render().Camera().transform.position += GEngine::Get().Render().Camera().transform.orientation.S_To_O(settings.cameraloc) * 5;

	EmitterSettings es = {
		new PlayerShipTrailGen(this),
		new PlayerShipTrailPop(),
		Transform3d(Vector3d()),
		(Texture2*)GEngine::Get().Resource().Get("explosion.tga"),
		BlendSettings{
		true,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_FUNC_ADD
	},
		100,
		0,
		{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		FACE_CAM,
		false
	};

	trail = new Emitter(es, 0.01);

	Set_Person(true);

	/*EmitterSettings smokesettings = {
		300,
		1 / 150.0,
		Vector3d(), 0.5f,
		Vector3d(0, 0, -1), 180.0f,
		1.0f, 1.5f,
		2.0f, 2.0f,
		(Texture2*) GEngine::Get().Resource().Get_Resource("smoke.tga"),
		Vector2f(1, 1), 0.5, 1.0, 
		Color4f(0.5, 0.5, 0.5, 0.5),
		Color4f(0.0, 0.0, 0.0, 0.0),
		false
	};
	smoke = new Emitter(smokesettings, 1.0 / 30.0);
	smoke->glblendeq = GL_FUNC_REVERSE_SUBTRACT;*/
}

PlayerShip::~PlayerShip() {
	delete mesh;
	delete trail;
}

void PlayerShip::Take_Damage(float damage) {
	damage = shield.Mod_Damage(damage);
	float damages[6];
	for (int i = 0; i < 6; i++)
		damages[i] = damage;
	for (int i = 0; i < 30; i++) {
		int n1 = rand() % 6;
		int n2 = rand() % 6;
		float f = Rand_Float(0.0f, damages[n1]);
		damages[n1] -= f;
		damages[n2] += f;
	}

	reactor.		Take_Damage(damages[0]);
	shield.			Take_Damage(damages[1]);
	propulsion.		Take_Damage(damages[2]);
	repairer.		Take_Damage(damages[3]);
	laserguns.		Take_Damage(damages[4]);
	missilelauncher.Take_Damage(damages[5]);
}

void PlayerShip::Set_Person(bool firstperson) {
	bool lastperson = this->firstperson;
	this->firstperson = firstperson;
	if (firstperson) {
		GEngine::Get().Render().Remove(mesh);
		ParticleSystemManager::active->Remove(shield.visual);
		ParticleSystemManager::active->Remove(trail);
		settings.cameraloc = Vector3d(0, 0, -2);
	}
	else {
		if (lastperson) {
			GEngine::Get().Render().Add(mesh);
			ParticleSystemManager::active->Add(shield.visual);
			ParticleSystemManager::active->Add(trail);
			settings.cameraloc = Vector3d(0, 6, 20);
		}
	}
}

void PlayerShip::Update(double t) {
//	CollidableComponent3::stepper.step = (velocity.Is_Zero()) ? 1.0 : 1.0 / velocity.Magnitude();
	propulsion.		Update(t);
	reactor.		Update(t);
	shield.			Update(t);
	repairer.		Update(t);
	laserguns.		Update(t);
	missilelauncher.Update(t);

	GEngine::Get().Render().Camera().transform = transform;
	if(!firstperson)
		GEngine::Get().Render().Camera().transform.orientation.Rotate(rotationalVelocity.Normalized() * sqrt(rotationalVelocity.Magnitude()) * -2.0);
	GEngine::Get().Render().Camera().transform.position += GEngine::Get().Render().Camera().transform.orientation.S_To_O(settings.cameraloc);
	if(!firstperson)
		GEngine::Get().Render().Camera().transform.position += transform.orientation.forward * sqrtf(fabs(propulsion.fbvel)) * -0.5 * ((signbit(propulsion.fbvel)) ? -1.0 : 1.0);
	mesh->transform = transform;
	if(!firstperson)
		mesh->transform.orientation.Rotate(transform.orientation.forward * propulsion.rlvel * 0.5);
	//smoke->settings.position = transform.S_To_O(Vector3d(0.25, 1, 1));
}

bool PlayerShip::Collide(CollidableComponent3* that, Collision* collision) {
	switch (that->type) {
	case Identity::pulse:
		new CameraShake(0.4, 0.1, 1.2);
		break;
	}

	if (reactor.efficiency == 0.0) {
		DefaultExplosion(transform.position, 3.0);
		return true;
	}
	return false;
}
