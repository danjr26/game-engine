#ifndef MECHANICS_H
#define MECHANICS_H
#include "component.h"
#include "trq.h"

template class IndexedArray<MechanicalComponent>;
class MechanicsManager {
public:
	static const uint CAPACITY = 1024;
private:
	IndexedArray<MechanicalComponent> components;

public:
			MechanicsManager	();
			~MechanicsManager	();
	void	Add					(MechanicalComponent* component);
	void	Remove				(MechanicalComponent* component);
};

#endif
