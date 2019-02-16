#include "collision_context.h"
#include "game_engine.h"

template<class T, uint n>
CollisionContext<T, n>::CollisionContext(const AxisAlignedBox<T, n>& in_box, uint in_depth) :
	mTree(in_box),
	mDepth(in_depth) 
{}

template<class T, uint n>
CollisionContext<T, n>::~CollisionContext() 
{}

template<class T, uint n>
void CollisionContext<T, n>::add(CollisionMask<T, n>* in_mask) {
	if (in_mask == nullptr) {
		throw InvalidArgumentException();
	}
	mMasksToAdd.push_back(in_mask);
}

template<class T, uint n>
void CollisionContext<T, n>::remove(CollisionMask<T, n>* in_mask) {
	for (auto it = mEvaluations.begin(); it < mEvaluations.end(); it++) {
		if (it->getParentMask() == in_mask) {
			mEvaluations.erase(it);
			it--;
		}
	}
}

template<class T, uint n>
void CollisionContext<T, n>::update() {
	using Evaluation = BinaryCollisionTree<T, n>::Evaluation;
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


template<class T, uint n>
void CollisionContext<T, n>::update(CollisionMask<T, n>** in_masks, uint in_nMasks) {
	using Evaluation = BinaryCollisionTree<T, n>::Evaluation;
	std::vector<Evaluation*> filteredEvaluations1;
	std::vector<Evaluation*> filteredEvaluations2;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	Collision<T, n> collision;

	prepareDataContainers(in_masks, in_nMasks);
	updateEvaluations(in_masks, in_nMasks);

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

template<class T, uint n>
uint CollisionContext<T, n>::getTotalPartnerings() const {
	return (uint)mPartnering.size();
}

template<class T, uint n>
void CollisionContext<T, n>::getPartners(CollisionMask<T, n>* in_mask, std::vector<CollisionPartner*>& out_partners) {
	out_partners.clear();
	auto search = mPartnering.equal_range(in_mask);
	for (auto it = search.first; it != search.second; it++) {
		out_partners.push_back(&it->second);
	}
}

template<class T, uint n>
void CollisionContext<T, n>::setPartnerTestActivation(std::pair<ubyte, ubyte> in_test, bool in_value) {
	if (in_value) {
		mPartnersToTest.insert(in_test);
	}
	else {
		mPartnersToTest.erase(in_test);
	}
}

template<class T, uint n>
bool CollisionContext<T, n>::getPartnerTestActivation(std::pair<ubyte, ubyte> in_test) {
	return mPartnersToTest.find(in_test) != mPartnersToTest.end();
}

template<class T, uint n>
void CollisionContext<T, n>::getTriggeredTests(CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2, std::vector<std::pair<ubyte, ubyte>>& out_tests) {
	for (auto it = mPartnersToTest.begin(); it != mPartnersToTest.begin(); it++) {
		if (in_mask1->hasFilter(it->first) && in_mask2->hasFilter(it->second)) {
			out_tests.push_back(*it);
		}
		else if (in_mask1->hasFilter(it->second) && in_mask2->hasFilter(it->first)) {
			out_tests.push_back(std::pair<ubyte, ubyte>(it->second, it->first));
		}
	}
}

template<class T, uint n>
void CollisionContext<T, n>::updateEvaluations() {
	std::vector<PackagedAsyncTask<void>> mTasks;
	uint nEvaluations = (uint)mEvaluations.size();
	uint batchSize = (uint)mEvaluations.size() / 4 + 1;

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
	for (auto it = mMasksToAdd.begin(); it < mMasksToAdd.end(); it++) {
		mEvaluations.push_back(BinaryCollisionTree<T, n>::Evaluation());
		mTree.evaluate(*it, mDepth, mEvaluations.back());
	}
	mMasksToAdd.clear();
}

template<class T, uint n>
void CollisionContext<T, n>::updateEvaluations(CollisionMask<T, n>** in_masks, uint in_nMasks) {
	std::vector<PackagedAsyncTask<void>> mTasks;
	uint nEvaluations = in_nMasks;
	uint batchSize = in_nMasks / 4 + 1;

	mTasks.reserve(nEvaluations / batchSize + 2);

	for (uint i = 0; i < nEvaluations; i += batchSize) {
		auto todo = [this, i, batchSize, nEvaluations, in_masks]() {
			for (uint j = i; j < i + batchSize && j < nEvaluations; j++) {
				for (uint k = 0; k < mEvaluations.size(); k++) {
					if (mEvaluations[k].getParentMask() == in_masks[j]) {
						mTree.evaluate(in_masks[j], mDepth, mEvaluations[k]);
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

template<class T, uint n>
void CollisionContext<T, n>::prepareDataContainers() {
	mPartnering.clear();
	mTasks.clear();

	mGroupingSchemes.clear();
	mGroupingSchemes.reserve(mPartnersToTest.size());

	mPairedGroupingSchemes.clear();
	mPairedGroupingSchemes.reserve(mPartnersToTest.size());
}

template<class T, uint n>
void CollisionContext<T, n>::prepareDataContainers(CollisionMask<T, n>** in_masks, uint in_nMasks) {
	mPartnering.clear();
	mTasks.clear();

	mGroupingSchemes.clear();
	mGroupingSchemes.reserve(mPartnersToTest.size());

	mPairedGroupingSchemes.clear();
	mPairedGroupingSchemes.reserve(mPartnersToTest.size());
}

template<class T, uint n>
void CollisionContext<T, n>::partnerFiltered(std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations) {
	if (in_filteredEvaluations.size() > 1) {
		mGroupingSchemes.push_back(BinaryCollisionTree<T, n>::GroupingScheme());

		mTree.group(
			in_filteredEvaluations.data(),
			(uint)in_filteredEvaluations.size(),
			mGroupingSchemes.back()
		);

		uint batchSize = 10;
		for (auto kt = mGroupingSchemes.back().getIterator(); !kt.isDone(); kt += batchSize) {
			auto todo = [this, kt, batchSize]() {
				auto itCopy = kt;
				for (uint i = 0; i < batchSize && !itCopy.isDone(); i++) {
					partnerIfCollideAsync(
						itCopy.getFirst()->getTransformedParentMask(),
						itCopy.getSecond()->getTransformedParentMask(),
						itCopy.getFirst()->getParentMask(),
						itCopy.getSecond()->getParentMask(),
						mPartnering,
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
void CollisionContext<T, n>::partnerFiltered(std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations1, std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations2) {
	if (in_filteredEvaluations1.size() > 0 && in_filteredEvaluations2.size() > 0) {
		mPairedGroupingSchemes.push_back(BinaryCollisionTree<T, n>::PairedGroupingScheme());

		mTree.group(
			in_filteredEvaluations1.data(), (uint)in_filteredEvaluations1.size(),
			in_filteredEvaluations2.data(), (uint)in_filteredEvaluations2.size(),
			mPairedGroupingSchemes.back()
		);

		uint batchSize = 10;
		for (auto kt = mPairedGroupingSchemes.back().getIterator(); !kt.isDone(); kt += batchSize) {
			auto todo = [this, kt, batchSize]() {
				auto itCopy = kt;
				for (uint i = 0; i < batchSize && !itCopy.isDone(); i++) {
					partnerIfCollideAsync(
						itCopy.getFirst()->getTransformedParentMask(),
						itCopy.getSecond()->getTransformedParentMask(),
						itCopy.getFirst()->getParentMask(),
						itCopy.getSecond()->getParentMask(),
						mPartnering,
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
void CollisionContext<T, n>::partnerIfCollide(CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2, Partnering& in_partnering) {
	Collision<T, n> collision;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	auto search = in_partnering.equal_range(in_mask1);

	bool found = false;
	for (auto it = search.first; it != search.second; it++) {
		if (it->second.mMask == in_mask2) {
			found = true;
			break;
		}
	}

	if (!found && (collision = collisionEvaluator.evaluate(*in_mask1, *in_mask2)).mDid) {
		in_partnering.insert(Partnering::value_type(in_mask1, { in_mask2, collision }));
		in_partnering.insert(Partnering::value_type(in_mask2, { in_mask1, collision }));
	}
}

template<class T, uint n>
void CollisionContext<T, n>::partnerIfCollideAsync(
	CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2, 
	CollisionMask<T, n>* in_maskToPartner1, CollisionMask<T, n>* in_maskToPartner2,
	Partnering& in_partnering, std::mutex& in_mutex) {

	if (in_mask1 == nullptr || in_mask2 == nullptr || in_maskToPartner1 == nullptr || in_maskToPartner2 == nullptr) {
		throw InvalidArgumentException();
	}

	Collision<T, n> collision;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;

	if ((collision = collisionEvaluator.evaluate(*in_mask1, *in_mask2)).mDid) {
		std::lock_guard<std::mutex> lock(in_mutex);

		if (collision.mOwner == in_mask1) collision.mOwner = in_maskToPartner1;
		if (collision.mOwner == in_mask2) collision.mOwner = in_maskToPartner2;

		auto range = in_partnering.equal_range(in_maskToPartner1);
		for (auto it = range.first; it != range.second; it++) {
			if (it->second.mMask == in_maskToPartner2) {
				return;
			}
		}

		in_partnering.insert(Partnering::value_type(in_maskToPartner1, { in_maskToPartner2, collision }));
		in_partnering.insert(Partnering::value_type(in_maskToPartner2, { in_maskToPartner1, collision }));
	}
}


template class CollisionContext<float, 2>;
template class CollisionContext<double, 2>;