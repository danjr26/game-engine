#include "collision_context.h"
#include "game_engine.h"
#include "collision_queue.h"

template<class T, uint n>
CollisionContext<T, n>::CollisionContext(const AABox<T, n>& i_box, uint i_depth) :
	mTree(i_box),
	mDepth(i_depth) 
{}

template<class T, uint n>
CollisionContext<T, n>::~CollisionContext() 
{}

template<class T, uint n>
void CollisionContext<T, n>::add(Collider<T, n>* i_mask) {
	if (i_mask == nullptr) {
		throw InvalidArgumentException();
	}
	mMasksToAdd.push_back(i_mask);
}

template<class T, uint n>
void CollisionContext<T, n>::remove(Collider<T, n>* i_mask) {
	for (uint i = 0; i < mEvaluations.size();) {
		if (mEvaluations[i].getParentMask() == i_mask) {
			mEvaluations.erase(mEvaluations.begin() + i);
		}
		else {
			i++;
		}
	}
}

/*
template<class T, uint n>
void CollisionContext<T, n>::check(Collider<T, n>* i_mask, std::vector<CollisionPartner>& o_partners) {
	return;
	std::vector<Evaluation*> filteredEvaluations1;
	std::vector<Evaluation*> filteredEvaluations2;

	Evaluation evaluation;
	mTree.evaluate(i_mask, mDepth, evaluation);
	filteredEvaluations1.push_back(&evaluation);

	Partnering partnering;
	for (auto it = mPartnersToTest.begin(); it != mPartnersToTest.end(); it++) {
		ubyte filter1 = it->first;
		ubyte filter2 = it->second;
		if (i_mask->hasFilter(filter1)) {
			filteredEvaluations2.clear();
			for (auto jt = mEvaluations.begin(); jt != mEvaluations.end(); jt++) {
				if (jt->getParentMask()->hasFilter(filter2)) {
					filteredEvaluations2.push_back(&*jt);
				}
			}
			partnerFiltered(filteredEvaluations1, filteredEvaluations2, partnering);
		}

		if (i_mask->hasFilter(filter2)) {
			filteredEvaluations2.clear();
			for (auto jt = mEvaluations.begin(); jt != mEvaluations.end(); jt++) {
				if (jt->getParentMask()->hasFilter(filter1)) {
					filteredEvaluations2.push_back(&*jt);
				}
			}
			partnerFiltered(filteredEvaluations1, filteredEvaluations2, partnering);
		}
	}

	for (auto it = partnering.begin(); it != partnering.end(); it++) {
		if (it->first == i_mask) {
			o_partners.push_back(it->second);
		}
	}
}
*/

template<class T, uint n>
void CollisionContext<T, n>::check(
	Collider<T, n>* i_mask, 
	std::vector<CollisionPartner<T, n>>& o_partners, 
	std::function<bool(Collider<T, n>*)> i_filter) {

	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	collisionEvaluator.returnPoint(true);
	collisionEvaluator.returnSeparator(true);

	CollisionPartner<T, n> partner;
	Collider<T, n>* transformedMask = i_mask->clone();
	transformedMask->applyTransform();

	for (auto jt = mEvaluations.begin(); jt != mEvaluations.end(); jt++) {
		Collider<T, n>* mask2 = jt->getParentMask();
		if (i_mask != mask2 && i_filter(mask2)) {
			partner.mCollision = collisionEvaluator.evaluate(
				*transformedMask, 
				*jt->getTransformedParentMask()
			);
			if (partner.mCollision.mDid) {
				partner.mMask = mask2;
				o_partners.push_back(partner);
			}
		}
	}
	delete transformedMask;
}


template<class T, uint n>
void CollisionContext<T, n>::update() {
	std::vector<Evaluation*> filteredEvaluations1;
	std::vector<Evaluation*> filteredEvaluations2;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	Collision<T, n> collision;

	prepareDataContainers();
	updateEvaluations();

	for (auto it = mPartnersToTest.begin(); it != mPartnersToTest.end(); it++) {
		filteredEvaluations1.clear();
		filteredEvaluations2.clear();

		ubyte filter1 = it->first;
		ubyte filter2 = it->second;

		if (filter1 == filter2) {
			for (auto jt = mEvaluations.begin(); jt < mEvaluations.end(); jt++) {
				if (jt->getParentMask()->hasFilter(filter1)) {
					filteredEvaluations1.push_back(&(*jt));
				}
			}
			partnerFiltered(filteredEvaluations1, mPartnering);
		}
		else {
			for (auto jt = mEvaluations.begin(); jt < mEvaluations.end(); jt++) {
				if (jt->getParentMask()->hasFilter(filter1)) {
					filteredEvaluations1.push_back(&(*jt));
				}
			}

			for (auto jt = mEvaluations.begin(); jt < mEvaluations.end(); jt++) {
				if (jt->getParentMask()->hasFilter(filter2)) {
					filteredEvaluations2.push_back(&(*jt));
				}
			}
			partnerFiltered(filteredEvaluations1, filteredEvaluations2, mPartnering);
		}
	}

	GE.async().helpUntilEmpty();

	for (auto it = mTasks.begin(); it < mTasks.end(); it++) {
		(*it)->waitForFinish();
		delete *it;
	}

	recordHistory();
}

/*
template<class T, uint n>
void CollisionContext<T, n>::update(Collider<T, n>** i_masks, uint i_nMasks) {
	using Evaluation = BinaryCollisionTree<T, n>::Evaluation;
	std::vector<Evaluation*> filteredEvaluations1;
	std::vector<Evaluation*> filteredEvaluations2;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	Collision<T, n> collision;

	prepareDataContainers(i_masks, i_nMasks);
	updateEvaluations(i_masks, i_nMasks);

	for (auto it = mPartnersToTest.begin(); it != mPartnersToTest.end(); it++) {
		filteredEvaluations1.clear();
		filteredEvaluations2.clear();

		ubyte filter1 = it->first;
		ubyte filter2 = it->second;

		if (filter1 == filter2) {
			for (auto jt = mEvaluations.begin(); jt < mEvaluations.end(); jt++) {
				if (jt->getParentMask()->hasFilter(filter1)) {
					filteredEvaluations1.push_back(&(*jt));
				}
			}

			partnerFiltered(filteredEvaluations1);
		}
		else {
			for (auto jt = mEvaluations.begin(); jt < mEvaluations.end(); jt++) {
				if (jt->getParentMask()->hasFilter(filter1)) {
					filteredEvaluations1.push_back(&(*jt));
				}
			}

			for (auto jt = mEvaluations.begin(); jt < mEvaluations.end(); jt++) {
				if (jt->getParentMask()->hasFilter(filter2)) {
					filteredEvaluations2.push_back(&(*jt));
				}
			}

			partnerFiltered(filteredEvaluations1, filteredEvaluations2);
		}
	}

	GE.async().helpUntilEmpty();

	for (auto it = mTasks.begin(); it < mTasks.end(); it++) {
		(*it)->waitForFinish();
		delete *it;
	}
}
*/

template<class T, uint n>
uint CollisionContext<T, n>::getTotalPartnerings() const {
	return (uint)mPartnering.size();
}

template<class T, uint n>
void CollisionContext<T, n>::getPartners(Collider<T, n>* i_mask, std::vector<CollisionPartner<T, n>*>& o_partners) {
	o_partners.clear();
	auto search = mPartnering.equal_range(i_mask);
	for (auto it = search.first; it != search.second; it++) {
		o_partners.push_back(&it->second);
	}
}

template<class T, uint n>
void CollisionContext<T, n>::setPartnerTestActivation(std::pair<ubyte, ubyte> i_test, bool i_value) {
	if (i_value) {
		mPartnersToTest.insert(i_test);
	}
	else {
		mPartnersToTest.erase(i_test);
	}
}

template<class T, uint n>
bool CollisionContext<T, n>::getPartnerTestActivation(std::pair<ubyte, ubyte> i_test) {
	return mPartnersToTest.find(i_test) != mPartnersToTest.end();
}

template<class T, uint n>
void CollisionContext<T, n>::getTriggeredTests(Collider<T, n>* i_mask1, Collider<T, n>* i_mask2, std::vector<std::pair<ubyte, ubyte>>& o_tests) {
	for (auto it = mPartnersToTest.begin(); it != mPartnersToTest.begin(); it++) {
		if (i_mask1->hasFilter(it->first) && i_mask2->hasFilter(it->second)) {
			o_tests.push_back(*it);
		}
		else if (i_mask1->hasFilter(it->second) && i_mask2->hasFilter(it->first)) {
			o_tests.push_back(std::pair<ubyte, ubyte>(it->second, it->first));
		}
	}
}

template<class T, uint n>
void CollisionContext<T, n>::broadcast() {
	for (auto it = mPartnering.begin(); it != mPartnering.end(); it++) {
		if (it->first->getQueue()) it->first->getQueue()->push_back(it->second);
	}
}

template<class T, uint n>
void CollisionContext<T, n>::updateEvaluations() {
	std::vector<PackagedAsyncTask<void>> mTasks;
	uint nEvaluations = (uint)mEvaluations.size();
	uint batchSize = geutil::max(nEvaluations, 1u);// / 4 + 1;

	mTasks.reserve(nEvaluations / batchSize + 2);

	for (uint i = 0; i < nEvaluations; i += batchSize) {
		auto todo = [this, i, batchSize, nEvaluations]() {
			for (uint j = i; j < i + batchSize && j < nEvaluations; j++) {
				mTree.evaluate(mEvaluations[j].getParentMask(), mDepth, mEvaluations[j]);
			}
		};
		mTasks.emplace_back(todo);
		GE.async().add(&mTasks.back());
	}

	GE.async().helpUntilEmpty();

	for (auto it = mTasks.begin(); it < mTasks.end(); it++) {
		it->waitForFinish();
	}
	/*
	for (uint i = 0; i < mEvaluations.size(); i++) {
		mTree.evaluate(mEvaluations[i].getParentMask(), mDepth, mEvaluations[i]);
	}
	*/
	mEvaluations.shrink_to_fit();
	for (auto it = mMasksToAdd.begin(); it < mMasksToAdd.end(); it++) {
		mEvaluations.shrink_to_fit();
		mEvaluations.push_back(BinaryCollisionTree<T, n>::Evaluation());
		mTree.evaluate(*it, mDepth, mEvaluations.back());
	}
	mMasksToAdd.clear();
}

/*
template<class T, uint n>
void CollisionContext<T, n>::updateEvaluations(Collider<T, n>** i_masks, uint i_nMasks) {
	std::vector<PackagedAsyncTask<void>> mTasks;
	uint nEvaluations = i_nMasks;
	uint batchSize = i_nMasks / 4 + 1;

	mTasks.reserve(nEvaluations / batchSize + 2);

	for (uint i = 0; i < nEvaluations; i += batchSize) {
		auto todo = [this, i, batchSize, nEvaluations, i_masks]() {
			for (uint j = i; j < i + batchSize && j < nEvaluations; j++) {
				for (uint k = 0; k < mEvaluations.size(); k++) {
					if (mEvaluations[k].getParentMask() == i_masks[j]) {
						mTree.evaluate(i_masks[j], mDepth, mEvaluations[k]);
						break;
					}
				}
			}
		};
		mTasks.emplace_back(todo);
		GE.async().add(&mTasks.back());
	}

	GE.async().helpUntilEmpty();

	for (auto it = mTasks.begin(); it < mTasks.end(); it++) {
		it->waitForFinish();
	}
}
*/

template<class T, uint n>
void CollisionContext<T, n>::prepareDataContainers() {
	mPartnering.clear();
	mTasks.clear();

	mGroupingSchemes.clear();
	mGroupingSchemes.reserve(mPartnersToTest.size());

	mPairedGroupingSchemes.clear();
	mPairedGroupingSchemes.reserve(mPartnersToTest.size());
}

/*
template<class T, uint n>
void CollisionContext<T, n>::prepareDataContainers(Collider<T, n>** i_masks, uint i_nMasks) {
	mPartnering.clear();
	mTasks.clear();

	mGroupingSchemes.clear();
	mGroupingSchemes.reserve(mPartnersToTest.size());

	mPairedGroupingSchemes.clear();
	mPairedGroupingSchemes.reserve(mPartnersToTest.size());
}
*/

template<class T, uint n>
void CollisionContext<T, n>::recordHistory() {
	for (auto it = mPrevTransforms.begin(); it != mPrevTransforms.end(); it++) {
		it->second->deleteChainParents();
		delete it->second;
	}
	mPrevTransforms.clear();

	for (auto it = mEvaluations.begin(); it != mEvaluations.end(); it++) {
		mPrevTransforms[it->getParentMask()] = it->getParentMask()->getTransform().cloneChain();
	}

	mPrevPartnering.clear();
	for (auto it = mPartnering.begin(); it != mPartnering.end(); it++) {
		mPrevPartnering.insert(std::pair<Collider<T, n>*, Collider<T, n>*>(it->first, it->second.mMask));
	}
}

template<class T, uint n>
void CollisionContext<T, n>::partnerFiltered(
	std::vector<Evaluation*>& i_filteredEvaluations, 
	Partnering& i_partnering) {

	if (i_filteredEvaluations.size() > 1) {
		mGroupingSchemes.push_back(BinaryCollisionTree<T, n>::GroupingScheme());

		mTree.group(
			i_filteredEvaluations.data(),
			(uint)i_filteredEvaluations.size(),
			mGroupingSchemes.back()
		);

		uint batchSize = 10;
		for (auto kt = mGroupingSchemes.back().getIterator(); !kt.isDone(); kt += batchSize) {
			auto todo = [this, kt, batchSize, &i_partnering]() {
				auto itCopy = kt;
				for (uint i = 0; i < batchSize && !itCopy.isDone(); i++) {
					if (itCopy.getFirst()->getParentMask() == itCopy.getSecond()->getParentMask()) continue;
					partnerIfCollideAsync(
						itCopy.getFirst()->getTransformedParentMask(),
						itCopy.getSecond()->getTransformedParentMask(),
						itCopy.getFirst()->getParentMask(),
						itCopy.getSecond()->getParentMask(),
						i_partnering,
						mPartneringMutex
					);
					itCopy++;
				}
			};

			PackagedAsyncTask<void>* task = new PackagedAsyncTask<void>(todo);
			mTasks.push_back(task);
			GE.async().add(task);
		}
	}
}

template<class T, uint n>
void CollisionContext<T, n>::partnerFiltered(
	std::vector<Evaluation*>& i_filteredEvaluations1, 
	std::vector<Evaluation*>& i_filteredEvaluations2, 
	Partnering& i_partnering) {

	if (i_filteredEvaluations1.size() > 0 && i_filteredEvaluations2.size() > 0) {
		mPairedGroupingSchemes.push_back(BinaryCollisionTree<T, n>::PairedGroupingScheme());

		mTree.group(
			i_filteredEvaluations1.data(), (uint)i_filteredEvaluations1.size(),
			i_filteredEvaluations2.data(), (uint)i_filteredEvaluations2.size(),
			mPairedGroupingSchemes.back()
		);

		uint batchSize = 10;
		for (auto kt = mPairedGroupingSchemes.back().getIterator(); !kt.isDone(); kt += batchSize) {
			auto todo = [this, kt, batchSize, &i_partnering]() {
				auto itCopy = kt;
				for (uint i = 0; i < batchSize && !itCopy.isDone(); i++) {
					if (itCopy.getFirst()->getParentMask() == itCopy.getSecond()->getParentMask()) continue;
					partnerIfCollideAsync(
						itCopy.getFirst()->getTransformedParentMask(),
						itCopy.getSecond()->getTransformedParentMask(),
						itCopy.getFirst()->getParentMask(),
						itCopy.getSecond()->getParentMask(),
						i_partnering,
						mPartneringMutex
					);
					itCopy++;
				}
			};

			PackagedAsyncTask<void>* task = new PackagedAsyncTask<void>(todo);
			mTasks.push_back(task);
			GE.async().add(task);
		}
	}
}

template<class T, uint n>
void CollisionContext<T, n>::narrowCollision(
	Collider<T, n>& i_mask1, Collider<T, n>& i_mask2,
	const Transform<T, n>& i_separated1, const Transform<T, n>& i_separated2,
	Collision<T, n>& io_collision, uint i_nIterations) {

	Transform<T, n> original1 = i_mask1.getTransform();
	Transform<T, n> original2 = i_mask2.getTransform();
	Transform<T, n> hit1 = i_mask1.getTransform();
	Transform<T, n> hit2 = i_mask2.getTransform();
	Transform<T, n> miss1 = i_separated1;
	Transform<T, n> miss2 = i_separated2;

	Collision<T, n> collision;

	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	collisionEvaluator.returnPoint(true);
	collisionEvaluator.returnSeparator(true);

	i_mask1.getTransform() = miss1;
	i_mask2.getTransform() = miss2;

	collision = collisionEvaluator.evaluate(i_mask1, i_mask2);
	if (collision.mDid) {
		i_mask1.getTransform() = original1;
		i_mask2.getTransform() = original2;
		return;
	}
	io_collision.mOwner = collision.mOwner;
	io_collision.mSeparator = collision.mSeparator;

	Transform<T, n>* test1;
	Transform<T, n>* test2;
	std::vector<Transform<T, n>*> tests1, tests2;
	for (uint i = 0; i < i_nIterations; i++) {
		test1 = miss1.lerpChain(hit1, 0.5);
		test2 = miss2.lerpChain(hit2, 0.5);
		tests1.push_back(test1);
		tests2.push_back(test2);

		i_mask1.getTransform() = *test1;
		i_mask2.getTransform() = *test2;

		collision = collisionEvaluator.evaluate(i_mask1, i_mask2);
		if (collision.mDid) {
			hit1 = *test1;
			hit2 = *test2;
			io_collision.mDid = collision.mDid;
			io_collision.mPoint = collision.mPoint;
		}
		else {
			miss1 = *test1;
			miss2 = *test2;
			io_collision.mSeparator = collision.mSeparator;
			io_collision.mOwner = collision.mOwner;
		}
	}

	for (auto it = tests1.begin(); it != tests1.end(); it++) {
		(*it)->deleteChainParents();
		delete (*it);
	}
	for (auto it = tests2.begin(); it != tests2.end(); it++) {
		(*it)->deleteChainParents();
		delete (*it);
	}

	i_mask1.getTransform() = original1;
	i_mask2.getTransform() = original2;
}

/*
template<class T, uint n>
void CollisionContext<T, n>::partnerIfCollide(Collider<T, n>* i_mask1, Collider<T, n>* i_mask2, Partnering& i_partnering) {
	Collision<T, n> collision;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	auto search = i_partnering.equal_range(i_mask1);

	bool found = false;
	for (auto it = search.first; it != search.second; it++) {
		if (it->second.mMask == i_mask2) {
			found = true;
			break;
		}
	}

	if (!found && (collision = collisionEvaluator.evaluate(*i_mask1, *i_mask2)).mDid) {
		i_partnering.insert(Partnering::value_type(i_mask1, { i_mask2, collision }));
		i_partnering.insert(Partnering::value_type(i_mask2, { i_mask1, collision }));
	}
}

*/

template<class T, uint n>
void CollisionContext<T, n>::partnerIfCollideAsync(
	Collider<T, n>* i_mask1, Collider<T, n>* i_mask2, 
	Collider<T, n>* i_maskToPartner1, Collider<T, n>* i_maskToPartner2,
	Partnering& i_partnering, std::mutex& i_mutex) {

	if (i_mask1 == nullptr || i_mask2 == nullptr || i_maskToPartner1 == nullptr || i_maskToPartner2 == nullptr) {
		throw InvalidArgumentException();
	}

	Collision<T, n> collision;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;

	if ((collision = collisionEvaluator.evaluate(*i_mask1, *i_mask2)).mDid) {
		std::lock_guard<std::mutex> lock(i_mutex);

		bool firstHit = mPrevTransforms.count(i_maskToPartner1) && mPrevTransforms.count(i_maskToPartner2);
		if (firstHit) {
			auto prevRange = mPrevPartnering.equal_range(i_maskToPartner1);
			for (auto it = prevRange.first; it != prevRange.second; it++) {
				if (it->second == i_maskToPartner2) {
					firstHit = false;
					break;
				}
			}
		}

		if (firstHit) {
			narrowCollision(
				*i_maskToPartner1, *i_maskToPartner2, 
				*mPrevTransforms[i_maskToPartner1], *mPrevTransforms[i_maskToPartner2], 
				collision, 5
			);
		}

		if (collision.mOwner == i_mask1) collision.mOwner = i_maskToPartner1;
		if (collision.mOwner == i_mask2) collision.mOwner = i_maskToPartner2;

		auto range = i_partnering.equal_range(i_maskToPartner1);
		for (auto it = range.first; it != range.second; it++) {
			if (it->second.mMask == i_maskToPartner2) {
				return;
			}
		}

		i_partnering.insert(Partnering::value_type(i_maskToPartner1, { i_maskToPartner2, collision }));
		i_partnering.insert(Partnering::value_type(i_maskToPartner2, { i_maskToPartner1, collision }));
	}
}


template class CollisionContext<float, 2>;
template class CollisionContext<double, 2>;