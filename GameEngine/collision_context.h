#ifndef COLLISION_CONTEXT_H
#define COLLISION_CONTEXT_H

#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <mutex>
#include <functional>
#include "basic_collision_mask.h"
#include "binary_collision_tree.h"
#include "in_place_collision_evaluator.h"
#include "packaged_async_task.h"
#include "collision_partner.h"

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

private:
	using Evaluation = typename BinaryCollisionTree<T, n>::Evaluation;
	using GroupingScheme = typename BinaryCollisionTree<T, n>::GroupingScheme;
	using PairedGroupingScheme = typename BinaryCollisionTree<T, n>::PairedGroupingScheme;
	using Partnering = std::unordered_multimap<CollisionMask<T, n>*, CollisionPartner<T, n>>;

private:
	BinaryCollisionTree<T, n> mTree;

	std::vector<CollisionMask<T, n>*> mMasksToAdd;

	std::unordered_map<CollisionMask<T, n>*, Transform<T, n>*> mPrevTransforms;
	std::unordered_multimap<CollisionMask<T, n>*, CollisionMask<T, n>*> mPrevPartnering;

	std::vector<Evaluation> mEvaluations;
	std::vector<GroupingScheme> mGroupingSchemes;
	std::vector<PairedGroupingScheme> mPairedGroupingSchemes;

	Partnering mPartnering;
	std::mutex mPartneringMutex;
	std::vector<PackagedAsyncTask<void>*> mTasks;

	std::set<std::pair<ubyte, ubyte>> mPartnersToTest;

	uint mDepth;

public:
	CollisionContext(const AxisAlignedBox<T, n>& in_box, uint in_depth);
	~CollisionContext();

	void add(CollisionMask<T, n>* in_mask);
	void remove(CollisionMask<T, n>* in_mask);
	//void check(CollisionMask<T, n>* in_mask, std::vector<CollisionPartner>& out_partners);
	void check(CollisionMask<T, n>* in_mask, std::vector<CollisionPartner<T, n>>& out_partners, std::function<bool(CollisionMask<T, n>*)> in_filter);
	void update();
	//void update(CollisionMask<T, n>** in_masks, uint in_nMasks);
	uint getTotalPartnerings() const;
	void getPartners(CollisionMask<T, n>* in_mask, std::vector<CollisionPartner<T, n>*>& out_partners);
	void setPartnerTestActivation(std::pair<ubyte, ubyte> in_test, bool in_value);
	bool getPartnerTestActivation(std::pair<ubyte, ubyte> in_test);
	void getTriggeredTests(
		CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2, 
		std::vector<std::pair<ubyte, ubyte>>& out_tests
	);

	void broadcast();

	static void narrowCollision(
		CollisionMask<T, n>& in_mask1, CollisionMask<T, n>& in_mask2,
		const Transform<T, n>& in_separated1, const Transform<T, n>& in_separated2, 
		Collision<T, n>& inout_collision, uint in_nIterations
	);

private:
	void updateEvaluations();
	//void updateEvaluations(CollisionMask<T, n>** in_masks, uint in_nMasks);

	void prepareDataContainers();
	//void prepareDataContainers(CollisionMask<T, n>** in_masks, uint in_nMasks);
	
	void recordHistory();

	void partnerFiltered(
		std::vector<Evaluation*>& in_filteredEvaluations,
		Partnering& in_partnering
	);
	void partnerFiltered(
		std::vector<Evaluation*>& in_filteredEvaluations1, 
		std::vector<Evaluation*>& in_filteredEvaluations2,
		Partnering& in_partnering
	);

	//static void partnerIfCollide(CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2, Partnering& in_partnering);
	void partnerIfCollideAsync(
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