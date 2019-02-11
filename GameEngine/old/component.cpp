#include "component.h"
#include "framework.h"


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

CollidableComponent3::CollidableComponent3(Identity in_type, SphereTreeCollisionMask * in_mask) :
type		(type),
mask		(mask) {
	GEngine::Get().Collision().Add(this);
}

CollidableComponent3::~CollidableComponent3() {
	GEngine::Get().Collision().Remove(this);
	delete mask;
}

bool CollidableComponent3::Collide(const CollidableComponent3* in_that, const Collision* in_collision) {
	return false;
}

bool CollidableComponent3::Should_Check_Collision(const CollidableComponent3* in_that) const {
	return true;
}

SphereTreeCollisionMask const* CollidableComponent3::Mask() const {
	return mask;
}

Identity CollidableComponent3::Type() const {
	return type;
}

Entity::Entity(Transform3d in_transform, Identity in_type, Allegiance in_allegiance, SphereTree* in_tree, double in_mechanicsStep) :
MechanicalComponent	(&GEngine::Get().Play_Clock(), in_mechanicsStep),
CollidableComponent3	(in_type, new SphereTreeCollisionMask(&transform, &velocity, in_tree)),
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

bool Entity::Collide(const CollidableComponent3* in_that, const Collision* in_collision) {
	return false;
}

bool Entity::Should_Check_Collision(const CollidableComponent3* in_that) const {
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

bool NPC::Collide(const CollidableComponent3* in_that, const Collision* in_collision) {
	return false;
}

bool NPC::Should_Check_Collision(const CollidableComponent3* in_that) const {
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