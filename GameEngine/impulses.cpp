#include "impulses.h"
#include "framework.h"

Impulse::Impulse(AIShip* parent) 
: Component(TYPE_IMPULSE),
parent(parent) 
{}

Impulse::~Impulse()
{}

Vector3 Impulse::Get() 
{}

ConstImpulse::ConstImpulse(AIShip* parent, Vector3 v, bool subjective)
: Impulse	(parent),
v			(v),
subjective	(subjective) 
{}

ConstImpulse::~ConstImpulse()
{}

Vector3 ConstImpulse::Get() {
	if(subjective)
		return parent->transform.S_To_O
}