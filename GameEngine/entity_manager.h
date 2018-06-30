#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityManager : public MutexProtected {
private:
	ResponsibleIndexedArray<Entity> entities;

public:
	EntityManager	();
	~EntityManager	();
	void	Add				(Entity* in_entity);
	void	Remove			(Entity* in_entity);

	IndexedArray<Entity> const& List() const;
};

#endif