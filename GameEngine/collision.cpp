#include "collision.h"
#include "framework.h"

CollisionManager::CollisionManager() :
components	(512, offsetof(CollidableComponent, collisionManagerArrayIndex), true),
octTree		(2),
timeRequest	(&GEngine::Get().Play_Clock(), 1.0 / 100.0) 
{}

CollisionManager::~CollisionManager() {
	while (components.Size())
		delete components[0];
}

void CollisionManager::Check_Collisions() {
	octTree.Refresh();

	CollidableComponent* deletions[512];
	uint nDeletions = 0;

	uint count = 0;
	for (uint i = 0; i < components.Size(); i++) {
		for (int j = octTree.length - 1; j > 0; j--) {
			if (octTree.structure[j].data == components[i]) {
				OctInfo* node = nullptr;
				int nodei;
				for (int k = j - 1; k >= 0; k--) {
					if (octTree.structure[k].node.nodecode == 0xf0f0f0f0) {
						node = &octTree.structure[k];
						nodei = k;
						break;
					}
				}
				if (node == nullptr)
					Die("CollisiomManager.CheckCollisions: OctTree node not found");
				for (int k = nodei + 1; k < nodei + node->node.ncomponents + 1; k++) {
					if (k != j) {
						count++;
						Collision collision = (octTree.structure[j].data->Should_Check_Collision(octTree.structure[k].data)) ? 
							octTree.structure[j].data->mask->Is_Colliding(octTree.structure[k].data->mask) :
							Collision{ false, Vector3d() };
						if (collision.didCollide) {
							if (octTree.structure[j].data->Collide(octTree.structure[k].data, &collision)) {
								bool alreadyDeleted = false;
								for (uint i = 0; i < nDeletions; i++) {
									if (deletions[i] == octTree.structure[j].data) {
										alreadyDeleted = true;
										break;
									}
								}
								if (!alreadyDeleted) {
									deletions[nDeletions] = octTree.structure[j].data;
									nDeletions++;
								}
							}
							if (octTree.structure[k].data->Collide(octTree.structure[j].data, &collision)) {
								bool alreadyDeleted = false;
								for (uint i = 0; i < nDeletions; i++) {
									if (deletions[i] == octTree.structure[k].data) {
										alreadyDeleted = true;
										break;
									}
								}
								if (!alreadyDeleted) {
									deletions[nDeletions] = octTree.structure[k].data;
									nDeletions++;
								}
							}
						}
					}
				}
				break;
			}
		}
	}

	for (uint i = 0; i < nDeletions; i++)
		delete deletions[i];
}

void CollisionManager::Add(CollidableComponent* in_component) {
	components.Add(in_component);
}

void CollisionManager::Remove(CollidableComponent* in_component) {
	components.Remove(in_component);
}

