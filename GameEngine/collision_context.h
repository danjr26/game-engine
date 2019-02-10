#ifndef COLLISION_CONTEXT_H
#define COLLISION_CONTEXT_H

#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <mutex>
#include "basic_collision_mask.h"
#include "binary_collision_tree.h"
#include "in_place_collision_evaluator.h"
#include "packaged_async_task.h"

template<class T, uint n>
class CollisionContext {
public:
	enum Filters {
		rigid_body,
		rigid_body_unstoppable,
		force_field,
		user_defined
	};

	static const ubyte maxFilters = FilteredObject::maxFilters;
	struct CollisionPartner {
		CollisionMask<T, n>* mMask;
		Collision<T, n> mCollision;

		bool operator==(const CollisionPartner& in_other) {
			return mMask == in_other.mMask;
		}
	};

private:
	BinaryCollisionTree<T, n> mTree;

	std::vector<CollisionMask<T, n>*> mMasksToAdd;

	std::vector<typename BinaryCollisionTree<T, n>::Evaluation> mEvaluations;
	std::vector<typename BinaryCollisionTree<T, n>::GroupingScheme> mGroupingSchemes;
	std::vector<typename BinaryCollisionTree<T, n>::PairedGroupingScheme> mPairedGroupingSchemes;

	using Partnering = std::unordered_multimap<CollisionMask<T, n>*, CollisionPartner>;
	Partnering mPartnering;
	std::mutex mPartneringMutex;
	std::vector<PackagedAsyncTask<void>*> mTasks;

	std::set<std::pair<ubyte, ubyte>> mPartnersToTest;

	uint mDepth;

public:
	CollisionContext(const AxisAlignedBox<T, n>& in_box, uint in_depth);
	~CollisionContext();

	void Add(CollisionMask<T, n>* in_mask);
	void Remove(CollisionMask<T, n>* in_mask);
	void Update();
	void Update(CollisionMask<T, n>** in_masks, uint in_nMasks);
	uint Get_Total_Partnerings() const;
	void Get_Partners(CollisionMask<T, n>* in_mask, std::vector<CollisionPartner*>& out_partners);
	void Set_Partner_Test_Activation(std::pair<ubyte, ubyte> in_test, bool in_value);
	bool Get_Partner_Test_Activation(std::pair<ubyte, ubyte> in_test);

private:
	void Update_Evaluations();
	void Update_Evaluations(CollisionMask<T, n>** in_masks, uint in_nMasks);

	void Prepare_Data_Containers();
	void Prepare_Data_Containers(CollisionMask<T, n>** in_masks, uint in_nMasks);

	void Partner_Filtered(
		std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations
	);
	void Partner_Filtered(
		std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations1, 
		std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations2
	);

	static void Partner_If_Collide(CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2, Partnering& in_partnering);
	static void Partner_If_Collide_Async(
		CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2,
		CollisionMask<T, n>* in_maskToPartner1, CollisionMask<T, n>* in_maskToPartner2,
		Partnering& in_partnering, std::mutex& in_mutex
	);
};

using CollisionContext2f = CollisionContext<float, 2>;
using CollisionContext2d = CollisionContext<double, 2>;
using CollisionContext3f = CollisionContext<float, 3>;
using CollisionContext3d = CollisionContext<double, 3>;

#endif