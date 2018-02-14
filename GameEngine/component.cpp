#include "component.h"
#include "framework.h"

MutexProtected::MutexProtected()
{}

MutexProtected::~MutexProtected()
{}

Stepper::Stepper(double in_step) :
step		(in_step),
accumulation(0.0) 
{}

Stepper::Stepper(double in_step, double in_accum) :
step		(in_step),
accumulation(in_accum)
{}

Stepper::~Stepper()
{}

int Stepper::Step_Number(double in_newAccumulation) {
	accumulation += in_newAccumulation;
	if (accumulation >= step) {
		int nStepsPassed;
		if (step != 0.0f) {
			nStepsPassed = (int)(accumulation / step);
			accumulation -= step * nStepsPassed;
		}
		else {
			nStepsPassed = 1;
			accumulation = 0.0f;
		}
		return nStepsPassed;
	}
	return 0;
}

double Stepper::Step_Total(double in_newAccumulation) {
	return Step_Number(in_newAccumulation) * step;
}

ProgressTracker::ProgressTracker(double in_maxAccumulation, bool in_capAtMax)  :
accumulation	(0.0),
maxAccumulation	(in_maxAccumulation),
capAtMax		(in_capAtMax)
{}

void ProgressTracker::Update(double in_newAccumulation) {
	accumulation += in_newAccumulation;
	if (capAtMax)
		accumulation = Clamp(accumulation, 0.0, maxAccumulation);
}

double ProgressTracker::Progress() const {
	return accumulation / maxAccumulation;
}

double ProgressTracker::Inv_Progress() const {
	return 1.0 - accumulation / maxAccumulation;
}

bool ProgressTracker::Is_Done() const {
	return accumulation >= maxAccumulation;
}

TransitionTracker::TransitionTracker(double offont, double onofft) :
state	(TransitionStates::off),
progressTracker	(offont, true),
offToOnDuration	(offont),
onToOffDuration	(onofft) 
{}

void TransitionTracker::Update(double in_newAccumulation) {
	switch (state) {
	case TransitionStates::off:
	case TransitionStates::on:
		break;
	case TransitionStates::offToOn:
		progressTracker.Update(in_newAccumulation);
		if (progressTracker.Is_Done())
			state = TransitionStates::on;
		break;
	case TransitionStates::onToOff:
		progressTracker.Update(in_newAccumulation);
		if (progressTracker.Is_Done())
			state = TransitionStates::off;
		break;
	}
}

void TransitionTracker::Turn_On() {
	switch (state) {
	case TransitionStates::on:
	case TransitionStates::offToOn:
		break;
	case TransitionStates::off:
		state = TransitionStates::offToOn;
		progressTracker = ProgressTracker(offToOnDuration, true);
		break;
	case TransitionStates::onToOff:
		state = TransitionStates::offToOn;
		progressTracker = ProgressTracker(offToOnDuration, true);
		progressTracker.accumulation = offToOnDuration * progressTracker.Inv_Progress();
		break;
	}
}

void TransitionTracker::Turn_Off() {
	switch (state) {
	case TransitionStates::off:
	case TransitionStates::onToOff:
		break;
	case TransitionStates::on:
		state = TransitionStates::onToOff;
		progressTracker = ProgressTracker(onToOffDuration, true);
		break;
	case TransitionStates::offToOn:
		state = TransitionStates::onToOff;
		progressTracker = ProgressTracker(onToOffDuration, true);
		progressTracker.accumulation = onToOffDuration * progressTracker.Inv_Progress();
		break;
	}
}

void TransitionTracker::Toggle() {
	switch (state) {
	case TransitionStates::on:
	case TransitionStates::offToOn:
		Turn_Off();
		break;
	case TransitionStates::off:
	case TransitionStates::onToOff:
		Turn_On();
		break;
	}
}

bool TransitionTracker::Is_Transitioning() const {
	return state == TransitionStates::offToOn || state == TransitionStates::onToOff;
}

Flags TransitionTracker::Get_State() const {
	return state;
}

double TransitionTracker::Get_Turn_On_Duration() const {
	return offToOnDuration;
}

double TransitionTracker::Get_Turn_Off_Duration() const {
	return onToOffDuration;
}

double TransitionTracker::Get_Progress_Accumulation() const {
	return progressTracker.accumulation;
}

void TransitionTracker::Set_State(Flags in_state) {
	state = in_state;
	progressTracker.accumulation = 0.0;
	Update(0.0);
}

void TransitionTracker::Set_Turn_On_Duration(double in_offToOnDuration) {
	offToOnDuration = in_offToOnDuration;
	Update(0.0);
}

void TransitionTracker::Set_Turn_Off_Duration(double in_onToOffDuration) {
	onToOffDuration = in_onToOffDuration;
	Update(0.0);
}

void TransitionTracker::Set_Progress_Accumulation(double in_progressAccumulation) {
	progressTracker.accumulation = in_progressAccumulation;
	Update(0.0);
}


MechanicalComponent::MechanicalComponent(Clock* in_clock, double in_step) {
	timeRequest = new MechTR(in_clock, this, in_step);
	GEngine::Get().Mech().Add(this);
}

MechanicalComponent::~MechanicalComponent() {
	GEngine::Get().Mech().Remove(this);
	delete timeRequest;
}

void MechanicalComponent::Update(double in_timePassed)
{}

RenderComponent::RenderComponent(Flags in_renderFlags) :
renderFlags(renderFlags) {
	GEngine::Get().Render().Add(this);
}

RenderComponent::~RenderComponent() {
	GEngine::Get().Render().Remove(this);
}

void RenderComponent::Render(Flags in_callFlags)
{}

OverlayComponent::OverlayComponent(OverlayComponent* in_parent, uint in_maxChildren, Vector3d(*in_Get_Position)(), Vector3d(*in_Get_Dimensions)()) :
parent			(in_parent),
children		(in_maxChildren, offsetof(OverlayComponent, parentArrayIndex), false),
Get_Position	(in_Get_Position),
Get_Dimensions	(in_Get_Dimensions) {
	OverlayManager::active->Add(this);
}

OverlayComponent::~OverlayComponent() {
	for (uint i = 0; i < children.Size(); i++)
		delete children[i];
	if (parent != nullptr)
		parent->Remove(this);
	OverlayManager::active->Remove(this);
}

void OverlayComponent::Render()
{}

void OverlayComponent::Add(OverlayComponent* in_child) {
	children.Add(in_child);
	in_child->parent = this;
}

void OverlayComponent::Remove(OverlayComponent* in_child) {
	children.CustomArray<OverlayComponent*>::Remove(in_child);
	in_child->parent = nullptr;
}

AIComponent::AIComponent(double in_step) :
timeRequest(nullptr) {
	GEngine::Get().AI().Add(this);
	timeRequest = new AITR(&GEngine::Get().Play_Clock(), this, in_step);
}

AIComponent::~AIComponent() {
	GEngine::Get().AI().Remove(this);
	delete timeRequest;
}

void AIComponent::Refresh()
{}

CollidableComponent::CollidableComponent(Identity in_type, CollisionMask * in_mask) :
type		(type),
mask		(mask) {
	GEngine::Get().Collision().Add(this);
}

CollidableComponent::~CollidableComponent() {
	GEngine::Get().Collision().Remove(this);
	delete mask;
}

bool CollidableComponent::Collide(const CollidableComponent* in_that, const Collision* in_collision) {
	return false;
}

bool CollidableComponent::Should_Check_Collision(const CollidableComponent* in_that) const {
	return true;
}

CollisionMask const* CollidableComponent::Mask() const {
	return mask;
}

Identity CollidableComponent::Type() const {
	return type;
}

Entity::Entity(Transform3d in_transform, Identity in_type, Allegiance in_allegiance, SphereTree* in_tree, double in_mechanicsStep) :
MechanicalComponent	(&GEngine::Get().Play_Clock(), in_mechanicsStep),
CollidableComponent	(in_type, new CollisionMask(&transform, &velocity, in_tree)),
transform			(in_transform),
velocity			(),
rotationalVelocity	(),
allegiance			(in_allegiance) {
	GEngine::Get().AI().Entities().Add(this);
}

Entity::~Entity() {
	GEngine::Get().AI().Entities().Remove(this);
}

void Entity::Update(double in_timePassed) 
{}

bool Entity::Collide(const CollidableComponent* in_that, const Collision* in_collision) {
	return false;
}

bool Entity::Should_Check_Collision(const CollidableComponent* in_that) const {
	return true;
}

NPC::NPC(Transform3d in_transform, Identity in_type, Allegiance in_allegiance, SphereTree* in_tree, double in_mechanicsStep, double in_aiStep) :
Entity		(in_transform, in_type, in_allegiance, in_tree, in_mechanicsStep),
AIComponent	(in_aiStep)
{}

NPC::~NPC() 
{}

void NPC::Update(double t)
{}

bool NPC::Collide(const CollidableComponent* in_that, const Collision* in_collision) {
	return false;
}

bool NPC::Should_Check_Collision(const CollidableComponent* in_that) const {
	return true;
}

void NPC::Refresh()
{}

/*
double When_Closest	(Vector3d loc1, Vector3d vel1, Vector3d loc2, Vector3d vel2) {
	Vector3d locdif = loc1 - loc2;
	Vector3d veldif = vel1 - vel2;
	double dot1 = locdif.Dot(veldif);
	double dot2 = veldif.Dot(veldif);
	if (dot2 == 0.0)
		return 0.0;
	return dot1 / dot2 * -1;
}

Vector3d How_Close	(Vector3d loc1, Vector3d vel1, Vector3d loc2, Vector3d vel2, double t) {
	Vector3d p1 = loc1 + vel1 * t;
	Vector3d p2 = loc2 + vel2 * t;
	return (p1 - p2);
}

double When_Close	(Vector3d loc1, Vector3d vel1, Vector3d loc2, Vector3d vel2, double d) {
	Vector3d dloc = loc1 - loc2;
	Vector3d dvel = vel1 - vel2;

	double a = dvel.Dot(dvel);
	double b = dvel.Dot(dloc) * 2;
	double c = dloc.Dot(dloc) - d * d;

	double b2 =		b * b;
	double disc =	b2 - 4 * a * c;

	if (disc < 0 || (b > 0 && disc < b2))
		return -1.0;

	double sqrtdisc =	sqrt(disc);
	double oneover2a =	1.0 / (a + a);
	double result =		(-b - sqrtdisc) * oneover2a;

	if (result > 0.0)
		return result;
	else
		return (-b + sqrtdisc) * oneover2a;
}*/