#ifndef COLLISION_CONTEXT_H
#define COLLISION_CONTEXT_H

#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <mutex>
#include <functional>
#include "collider.h"
#include "binary_collision_tree.h"
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
	using Partnering = std::unordered_multimap<Collider<T, n>*, CollisionPartner<T, n>>;

private:
	BinaryCollisionTree<T, n> mTree;

	std::vector<Collider<T, n>*> mMasksToAdd;

	std::unordered_map<Collider<T, n>*, Transform<T, n>*> mPrevTransforms;
	std::unordered_multimap<Collider<T, n>*, Collider<T, n>*> mPrevPartnering;

	std::vector<Evaluation> mEvaluations;
	std::vector<GroupingScheme> mGroupingSchemes;
	std::vector<PairedGroupingScheme> mPairedGroupingSchemes;

	Partnering mPartnering;
	std::mutex mPartneringMutex;
	std::vector<PackagedAsyncTask<void>*> mTasks;

	std::set<std::pair<ubyte, ubyte>> mPartnersToTest;

	uint mDepth;

public:
	CollisionContext(const AABox<T, n>& i_box, uint i_depth);
	~CollisionContext();

	void add(Collider<T, n>* i_mask);
	void remove(Collider<T, n>* i_mask);
	//void check(Collider<T, n>* i_mask, std::vector<CollisionPartner>& o_partners);
	void check(Collider<T, n>* i_mask, std::vector<CollisionPartner<T, n>>& o_partners, std::function<bool(Collider<T, n>*)> i_filter);
	void update();
	//void update(Collider<T, n>** i_masks, uint i_nMasks);
	uint getTotalPartnerings() const;
	void getPartners(Collider<T, n>* i_mask, std::vector<CollisionPartner<T, n>*>& o_partners);
	void setPartnerTestActivation(std::pair<ubyte, ubyte> i_test, bool i_value);
	bool getPartnerTestActivation(std::pair<ubyte, ubyte> i_test);
	void getTriggeredTests(
		Collider<T, n>* i_mask1, Collider<T, n>* i_mask2, 
		std::vector<std::pair<ubyte, ubyte>>& o_tests
	);

	void broadcast();

	static void narrowCollision(
		Collider<T, n>& i_mask1, Collider<T, n>& i_mask2,
		const Transform<T, n>& i_separated1, const Transform<T, n>& i_separated2, 
		Collision<T, n>& io_collision, uint i_nIterations
	);

private:
	void updateEvaluations();
	//void updateEvaluations(Collider<T, n>** i_masks, uint i_nMasks);

	void prepareDataContainers();
	//void prepareDataContainers(Collider<T, n>** i_masks, uint i_nMasks);
	
	void recordHistory();

	void partnerFiltered(
		std::vector<Evaluation*>& i_filteredEvaluations,
		Partnering& i_partnering
	);
	void partnerFiltered(
		std::vector<Evaluation*>& i_filteredEvaluations1, 
		std::vector<Evaluation*>& i_filteredEvaluations2,
		Partnering& i_partnering
	);

	//static void partnerIfCollide(Collider<T, n>* i_mask1, Collider<T, n>* i_mask2, Partnering& i_partnering);
	void partnerIfCollideAsync(
		Collider<T, n>* i_mask1, Collider<T, n>* i_mask2,
		Collider<T, n>* i_maskToPartner1, Collider<T, n>* i_maskToPartner2,
		Partnering& i_partnering, std::mutex& i_mutex
	);
};

using CollisionContext2f = CollisionContext<float, 2>;
using CollisionContext2d = CollisionContext<double, 2>;
using CollisionContext3f = CollisionContext<float, 3>;
using CollisionContext3d = CollisionContext<double, 3>;

#endif