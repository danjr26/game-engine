#include "binary_collision_tree.h"
#include "collision_evaluation.h"
#include "log.h"

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::Iterator::Iterator(Evaluation* i_parent) :
	mParent(i_parent),
	mDepths{ 0 },
	mEvals{ 0 } 
{}

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::Iterator::Iterator() :
	mParent(nullptr),
	mDepths{ 0 },
	mEvals{ 0 }
{}

template<class T, uint n>
bool BinaryCollisionTree<T, n>::Evaluation::Iterator::goLeft(uint i_dimension) {
	if (!getLeft(i_dimension) || mDepths[i_dimension] == mParent->mDepth) {
		return false;
	}
	else {
		if (getRight(i_dimension)) {
			mEvals[i_dimension]++;
			uint skipStack[maxDepth];
			uint skipStackSize = 1;
			skipStack[0] = mParent->mDepth - mDepths[i_dimension] + 1;
			while (skipStackSize > 0) {
				if (getLeft(i_dimension) && getRight(i_dimension) && skipStack[skipStackSize - 1] > 1) {
					skipStack[skipStackSize] = --skipStack[skipStackSize - 1];
					skipStackSize++;
				}

				if (--skipStack[skipStackSize - 1] == 0) {
					skipStackSize--;
				}

				if (skipStackSize > 0) {
					mEvals[i_dimension]++;
				}
			}
		}
		else {
			mEvals[i_dimension]++;
		}

		mDepths[i_dimension]++;
		return true;
	}
}

template<class T, uint n>
bool BinaryCollisionTree<T, n>::Evaluation::Iterator::goRight(uint i_dimension) {
	if (!getRight(i_dimension) || mDepths[i_dimension] == mParent->mDepth) {
		return false;
	}
	else {
		mEvals[i_dimension]++;
		mDepths[i_dimension]++;
		return true;
	}
}

template<class T, uint n>
ubyte BinaryCollisionTree<T, n>::Evaluation::Iterator::getLeft(uint i_dimension) {
	return mParent->mData[i_dimension][mEvals[i_dimension] * 2 + 0];
}

template<class T, uint n>
ubyte BinaryCollisionTree<T, n>::Evaluation::Iterator::getRight(uint i_dimension) {
	return mParent->mData[i_dimension][mEvals[i_dimension] * 2 + 1];
}

template<class T, uint n>
uint BinaryCollisionTree<T, n>::Evaluation::Iterator::getDepth(uint i_dimension) {
	return mDepths[i_dimension];
}

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::Evaluation() :
	mParentTree(nullptr),
	mParentMask(nullptr),
	mTransformedParentMask(nullptr),
	mDepth(UINT_MAX),
	mData{ 0 },
	mDataSizes{ 0 } 
{}

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::Evaluation(const Evaluation& i_other) :
	mParentTree(nullptr),
	mParentMask(nullptr),
	mTransformedParentMask(nullptr),
	mDepth(UINT_MAX),
	mData{ 0 },
	mDataSizes{ 0 } {

	*this = i_other;
}

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::Evaluation(Evaluation&& i_other) noexcept :
	mParentTree(i_other.mParentTree),
	mParentMask(i_other.mParentMask),
	mTransformedParentMask(i_other.mTransformedParentMask),
	mData{},
	mDataSizes{},
	mDepth(i_other.mDepth) {
	
	i_other.mParentTree = nullptr;
	i_other.mParentMask = nullptr;
	i_other.mTransformedParentMask = nullptr;

	for (uint i = 0; i < n; i++) {
		mData[i] = i_other.mData[i];
		mDataSizes[i] = i_other.mDataSizes[i];
		i_other.mData[i] = nullptr;
		i_other.mDataSizes[i] = 0;
	}

}

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::~Evaluation() {
	for (uint i = 0; i < n; i++) {
		if (mData[i] != nullptr) {
			delete[] mData[i];
			mData[i] = nullptr;
		}
	}
	if (mTransformedParentMask != nullptr) {
		delete mTransformedParentMask;
		mTransformedParentMask = nullptr;
	}
}

template<class T, uint n>
typename BinaryCollisionTree<T, n>::Evaluation& BinaryCollisionTree<T, n>::Evaluation::operator=(const Evaluation& i_other) {
	const ullong maxDataSize = 1ull << (i_other.mDepth + 2);

	if (&i_other == this) {
		return *this;
	}

	mParentTree = i_other.mParentTree;
	mParentMask = i_other.mParentMask;

	if (mTransformedParentMask != nullptr) { 
		delete mTransformedParentMask;
		mTransformedParentMask = nullptr;
	}
	if (i_other.mTransformedParentMask != nullptr) {
		mTransformedParentMask = i_other.mTransformedParentMask->clone();
	}

	for (uint i = 0; i < n; i++) {
		mDataSizes[i] = i_other.mDataSizes[i];
		if (mData[i] != nullptr) {
			delete[] mData[i];
		}
		
		mData[i] = new ubyte[maxDataSize];
		memcpy(mData[i], i_other.mData[i], mDataSizes[i]);
	}

	mDepth = i_other.mDepth;

	return *this;
}

template<class T, uint n>
typename BinaryCollisionTree<T, n>::Evaluation::Iterator BinaryCollisionTree<T, n>::Evaluation::getIterator() {
	return Iterator(this);
}

template<class T, uint n>
BinaryCollisionTree<T, n>* BinaryCollisionTree<T, n>::Evaluation::getParentTree() {
	return mParentTree;
}

template<class T, uint n>
Collider<T, n>* BinaryCollisionTree<T, n>::Evaluation::getParentMask() {
	return mParentMask;
}

template<class T, uint n>
Collider<T, n>* BinaryCollisionTree<T, n>::Evaluation::getTransformedParentMask() {
	return mTransformedParentMask;
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::Evaluation::transformParentMask() {
	if (mTransformedParentMask != nullptr) {
		delete mTransformedParentMask;
		mTransformedParentMask = nullptr;
	}

	mTransformedParentMask = mParentMask->clone();
	mTransformedParentMask->applyTransform();
	mTransformedParentMask->setAsIs(true);
}

template<class T, uint n>
BinaryCollisionTree<T, n>::GroupingScheme::Iterator::Iterator(GroupingScheme* i_parent) :
	mParent(i_parent),
	mIt(i_parent->mGroupings.begin()),
	mI(0),
	mJ(1) 
{}

template<class T, uint n>
typename BinaryCollisionTree<T, n>::Evaluation* BinaryCollisionTree<T, n>::GroupingScheme::Iterator::getFirst() {
	return mIt->mEvals[mI];
}

template<class T, uint n>
typename BinaryCollisionTree<T, n>::Evaluation* BinaryCollisionTree<T, n>::GroupingScheme::Iterator::getSecond() {
	return mIt->mEvals[mJ];
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::GroupingScheme::Iterator::operator++(int a) {
	if (isDone()) {
		throw InvalidArgumentException();
	}

	mJ++;
	if (mJ >= mIt->mEvals.size()) {
		mI++;
		mJ = mI + 1;
	}
	if (mI >= mIt->mEvals.size() - 1) {
		mIt++;
		mI = 0;
		mJ = 1;
	}
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::GroupingScheme::Iterator::operator+=(uint n) {
	for (uint i = 0; i < n; i++) {
		if (isDone()) break;
		(*this)++;
	}
}

template<class T, uint n>
bool BinaryCollisionTree<T, n>::GroupingScheme::Iterator::isDone() {
	return mIt == mParent->mGroupings.end();
}

template<class T, uint n>
uint BinaryCollisionTree<T, n>::GroupingScheme::getNumberGroups() const {
	return (uint)mGroupings.size();
}

template<class T, uint n>
typename BinaryCollisionTree<T, n>::GroupingScheme::Iterator BinaryCollisionTree<T, n>::GroupingScheme::getIterator() {
	return Iterator(this);
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::GroupingScheme::remove(const Evaluation * i_eval) {
	for (auto it = mGroupings.begin(); it != mGroupings.end(); it++) {
		auto search = std::find(it->mEvals.begin(), it->mEvals.end(), i_eval);
		if (search != it->mEvals.end()) {
			it->mEvals.erase(search);
		}
		if (it->mEvals.size() == 1) {
			mGroupings.erase(it);
			it = mGroupings.begin();
		}
	}
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::GroupingScheme::getUniquePairs(std::vector<std::pair<Evaluation*, Evaluation*>>& i_pairs) const {
	i_pairs.clear();
	i_pairs.reserve(mGroupings.size());
	for (auto groupIterator = mGroupings.begin(); groupIterator != mGroupings.end(); groupIterator++) {
		for (uint i = 0; i < groupIterator->mEvals.size(); i++) {
			for (uint j = i + 1; j < groupIterator->mEvals.size(); j++) {
				auto newPair = std::pair<Evaluation*, Evaluation*>(groupIterator->mEvals[i], groupIterator->mEvals[j]);
				auto lowerBound = std::lower_bound(i_pairs.begin(), i_pairs.end(), newPair);
				if (*lowerBound != newPair) {
					i_pairs.insert(lowerBound, newPair);
				}
			}
		}
	}
}

template<class T, uint n>
BinaryCollisionTree<T, n>::PairedGroupingScheme::Iterator::Iterator(PairedGroupingScheme * i_parent) :
	mParent(i_parent),
	mIt(i_parent->mGroupings.begin()),
	mI(0),
	mJ(0) 
{}

template<class T, uint n>
typename BinaryCollisionTree<T, n>::Evaluation* BinaryCollisionTree<T, n>::PairedGroupingScheme::Iterator::getFirst() {
	return mIt->mEvals1[mI];
}

template<class T, uint n>
typename BinaryCollisionTree<T, n>::Evaluation* BinaryCollisionTree<T, n>::PairedGroupingScheme::Iterator::getSecond() {
	return mIt->mEvals2[mJ];
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::PairedGroupingScheme::Iterator::operator++(int a) {
	if (isDone()) {
		throw InvalidArgumentException();
	}

	mJ++;
	if (mJ >= mIt->mEvals2.size()) {
		mI++;
		mJ = 0;
	}
	if (mI >= mIt->mEvals1.size()) {
		mIt++;
		mI = 0;
		mJ = 0;
	}
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::PairedGroupingScheme::Iterator::operator+=(uint n) {
	for (uint i = 0; i < n; i++) {
		if (isDone()) break;
		(*this)++;
	}
}

template<class T, uint n>
bool BinaryCollisionTree<T, n>::PairedGroupingScheme::Iterator::isDone() {
	return mIt == mParent->mGroupings.end();
}

template<class T, uint n>
uint BinaryCollisionTree<T, n>::PairedGroupingScheme::getNumberGroups() const {
	return (uint)mGroupings.size();
}

template<class T, uint n>
typename BinaryCollisionTree<T, n>::PairedGroupingScheme::Iterator BinaryCollisionTree<T, n>::PairedGroupingScheme::getIterator() {
	return Iterator(this);
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::PairedGroupingScheme::getUniquePairs(std::vector<std::pair<Evaluation*, Evaluation*>>& i_pairs) const {
	i_pairs.clear();
	i_pairs.reserve(mGroupings.size());
	for (auto groupIterator = mGroupings.begin(); groupIterator != mGroupings.end(); groupIterator++) {
		for (uint i = 0; i < groupIterator->mEvals1.size(); i++) {
			for (uint j = 0; j < groupIterator->mEvals2.size(); j++) {
				if (groupIterator->mEvals1[i] != groupIterator->mEvals2[j]) {
					i_pairs.push_back(std::pair(groupIterator->mEvals1[i], groupIterator->mEvals2[j]));
				}
			}
		}
	}
}

template<class T, uint n>
BinaryCollisionTree<T, n>::BinaryCollisionTree(const AABox<T, n>& i_box) :
	mBox(i_box) {}

template<class T, uint n>
void BinaryCollisionTree<T, n>::evaluate(Collider<T, n>* i_mask, uint i_depth, Evaluation& o_evaluation) {
	using StackElement = EvaluationStackElement;
	if (i_depth > maxDepth) {
		throw InvalidArgumentException();
	}

	const ullong maxDataSize = 1ull << (i_depth + 2);
	const ullong maxStackSize = 1ull << (maxDepth + 1);

	o_evaluation.mParentTree = this;
	o_evaluation.mParentMask = i_mask;
	o_evaluation.transformParentMask();

	for (uint i = 0; i < n; i++) {
		if (o_evaluation.mDepth != i_depth) {
			if (o_evaluation.mData[i] != nullptr) {
				delete[] o_evaluation.mData[i];
			}
			o_evaluation.mData[i] = new ubyte[maxDataSize];
		}
		o_evaluation.mDataSizes[i] = 0;
	}
	o_evaluation.mDepth = i_depth;

	StackElement* evaluationStacks[n];
	uint stackSizes[n];
	for (uint i = 0; i < n; i++) {
		stackSizes[i] = 1;
		evaluationStacks[i] = new EvaluationStackElement[maxStackSize];
		evaluationStacks[i][0] = { i, mBox.getCenter()[i], false, 0 };
	}

	AAHalfSpace<T, n> halfSpace1 = AAHalfSpace<T, n>::fromDimensionValue(0, 0.0, false);
	AAHalfSpace<T, n> halfSpace2 = AAHalfSpace<T, n>::fromDimensionValue(0, 0.0, false);
	AAHalfSpaceCollider<T, n> mask1(halfSpace1, true);
	AAHalfSpaceCollider<T, n> mask2(halfSpace2, true);
	Collision<T, n> collision1, collision2;

	for (bool keepGoing = true; keepGoing;) {
		keepGoing = false;
		for (uint i = 0; i < n; i++) {
			if (stackSizes[i] == 0) continue;
			StackElement& back = evaluationStacks[i][--stackSizes[i]];

			halfSpace1 = AAHalfSpace<T, n>::fromDimensionValue(
				back.mDimension, back.mValue, back.mIsPositive);
			halfSpace2 = AAHalfSpace<T, n>::fromDimensionValue(
				back.mDimension, back.mValue, !back.mIsPositive);

			mask1.getBasis() = halfSpace1;
			mask2.getBasis() = halfSpace2;

			getCollision(*o_evaluation.getTransformedParentMask(), mask1, collision1);
			getCollision(*o_evaluation.getTransformedParentMask(), mask2, collision2);

			o_evaluation.mData[i][o_evaluation.mDataSizes[i]++] = (bool)collision1;
			o_evaluation.mData[i][o_evaluation.mDataSizes[i]++] = (bool)collision2;

			T nextOffset = mBox.getDimensions()[halfSpace1.getDimension()] / (1 << (ullong)(back.mDepth + 2));
			uint lastDepth = back.mDepth;

			if (lastDepth < i_depth) {
				if (collision1) {
					evaluationStacks[i][stackSizes[i]++] = {
						halfSpace1.getDimension(),
						halfSpace1.getValue() + nextOffset,
						false,
						lastDepth + 1
					};
				}

				if (collision2) {
					evaluationStacks[i][stackSizes[i]++] = {
						halfSpace2.getDimension(),
						halfSpace2.getValue() - nextOffset,
						false,
						lastDepth + 1
					};
				}
			}

			if (stackSizes[i] > 0) {
				keepGoing = true;
			}
		}
	}
}

template<class T, uint n>
bool BinaryCollisionTree<T, n>::intersect(Evaluation & i_evaluation1, Evaluation & i_evaluation2) {
	using StackElement = IntersectionStackElement;

	IntersectionStackElement intersectionStacks[n][1 << (maxDepth + 1)];
	uint stackSizes[n];
	bool didIntersect[n];
	ubyte* mData[2][n];

	uint skipStackSize = 0;

	for (uint i = 0; i < n; i++) {
		stackSizes[i] = 1;
		intersectionStacks[i][0] = { i_evaluation1.getIterator(), i_evaluation2.getIterator() };
		didIntersect[i] = false;
		mData[0][i] = &i_evaluation1.mData[i][0];
		mData[1][i] = &i_evaluation2.mData[i][0];
	}

	for (bool keepGoing = true; keepGoing;) {
		keepGoing = false;
		for (uint i = 0; i < n; i++) {
			if (stackSizes[i] == 0) continue;

			StackElement element = intersectionStacks[i][--stackSizes[i]];

			if (element.mIt1.getLeft(i) && element.mIt2.getLeft(i)) {
				StackElement& newElement = intersectionStacks[i][stackSizes[i]++];
				newElement.mIt1 = element.mIt1;
				newElement.mIt2 = element.mIt2;
				if (!newElement.mIt1.goLeft(i) || !newElement.mIt2.goLeft(i)) {
					stackSizes[i] = 0;
					didIntersect[i] = true;
					continue;
				}
			}

			if (element.mIt1.getRight(i) && element.mIt2.getRight(i)) {
				StackElement& newElement = intersectionStacks[i][stackSizes[i]++];
				newElement.mIt1 = element.mIt1;
				newElement.mIt2 = element.mIt2;
				if (!newElement.mIt1.goRight(i) || !newElement.mIt2.goRight(i)) {
					stackSizes[i] = 0;
					didIntersect[i] = true;
					continue;
				}
			}

			if (stackSizes[i] > 0) {
				keepGoing = true;
			}
			else if (!didIntersect[i]) {
				keepGoing = false;
				break;
			}
		}
	}

	bool out = true;
	for (uint i = 0; i < n; i++) {
		if (!didIntersect[i]) out = false;
	}
	return out;
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::group(Evaluation ** i_evaluations, uint i_nElements, GroupingScheme & o_groupingScheme) {
	using Grouping = GroupingScheme::Grouping;

	if (mRecycling.size() < 1) {
		mRecycling.resize(1);
	}

	Grouping& startingGrouping = mRecycling.front();
	startingGrouping.mEvals.resize(i_nElements);

	uint i = 0;
	for (auto it = startingGrouping.mEvals.begin(); it != startingGrouping.mEvals.end(); it++) {
		*it = i_evaluations[i++];
	}

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < maxDepth; j++) {
			startingGrouping.mPath[i][j] = 255;
		}
	}

	o_groupingScheme.mParent = this;
	mRecycling.splice(mRecycling.end(), o_groupingScheme.mGroupings);
	o_groupingScheme.mGroupings.splice(o_groupingScheme.mGroupings.begin(), mRecycling, mRecycling.begin());

	if (mRecycling.size() > 100) {
		mRecycling.resize(50);
	}

	for (uint currentDepth = 0; currentDepth <= i_evaluations[0]->mDepth; currentDepth++) {
		// for each dimension
		for (uint i = 0; i < n; i++) {
			uint nGroups = (uint)o_groupingScheme.mGroupings.size();
			auto groupIterator = o_groupingScheme.mGroupings.begin();

			// for each grouping
			for (uint j = 0; j < nGroups; j++) {
				while (mRecycling.size() < 2) {
					mRecycling.push_back(Grouping());
				}

				Grouping& parentGrouping = *groupIterator;
				Grouping& leftGrouping = mRecycling.front();
				Grouping& rightGrouping = mRecycling.back();
				leftGrouping.mEvals.clear();
				rightGrouping.mEvals.clear();
				leftGrouping.mEvals.reserve(i_nElements);
				rightGrouping.mEvals.reserve(i_nElements);

				// for each evaluation in that grouping 
				for (auto indexIterator = parentGrouping.mEvals.begin(); indexIterator != parentGrouping.mEvals.end(); indexIterator++) {
					auto evaluationIterator = (*indexIterator)->getIterator();

					for (uint k = 0; k < currentDepth; k++) {
						if (!((parentGrouping.mPath[i][k]) ? evaluationIterator.goRight(i) : evaluationIterator.goLeft(i))) {
							throw ProcessFailureException();
						}
					}

					if (evaluationIterator.getLeft(i)) {
						leftGrouping.mEvals.push_back(*indexIterator);
					}

					if (evaluationIterator.getRight(i)) {
						rightGrouping.mEvals.push_back(*indexIterator);
					}
				}

				if (leftGrouping.mEvals.size() > 1) {
					for (uint k = 0; k < n; k++) {
						for (uint m = 0; m < maxDepth; m++) {
							leftGrouping.mPath[k][m] = parentGrouping.mPath[k][m];
						}
					}
					leftGrouping.mPath[i][currentDepth] = 0;

					o_groupingScheme.mGroupings.splice(o_groupingScheme.mGroupings.end(), mRecycling, mRecycling.begin());
				}

				if (rightGrouping.mEvals.size() > 1) {
					for (uint k = 0; k < n; k++) {
						for (uint m = 0; m < maxDepth; m++) {
							rightGrouping.mPath[k][m] = parentGrouping.mPath[k][m];
						}
					}
					rightGrouping.mPath[i][currentDepth] = 1;

					o_groupingScheme.mGroupings.splice(o_groupingScheme.mGroupings.end(), mRecycling, --mRecycling.end());
				}
				groupIterator++;
			}
			auto begin = o_groupingScheme.mGroupings.begin();
			auto end = o_groupingScheme.mGroupings.begin();
			std::advance(end, nGroups);
			mRecycling.splice(mRecycling.end(), o_groupingScheme.mGroupings, begin, end);

			if (o_groupingScheme.mGroupings.size() == 0) {
				currentDepth = i_evaluations[0]->mDepth + 1;
				break;
			}
		}
	}
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::group(Evaluation ** i_evaluations1, uint i_nElements1, Evaluation ** i_evaluations2, uint i_nElements2, PairedGroupingScheme & o_groupingScheme) {
	using Grouping = PairedGroupingScheme::Grouping;

	if (mPairRecycling.size() < 1) {
		mPairRecycling.resize(1);
	}

	Grouping& startingGrouping = mPairRecycling.front();
	startingGrouping.mEvals1.resize(i_nElements1);
	startingGrouping.mEvals2.resize(i_nElements2);

	uint i = 0;
	for (auto it = startingGrouping.mEvals1.begin(); it != startingGrouping.mEvals1.end(); it++) {
		*it = i_evaluations1[i++];
	}

	i = 0;
	for (auto it = startingGrouping.mEvals2.begin(); it != startingGrouping.mEvals2.end(); it++) {
		*it = i_evaluations2[i++];
	}

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < maxDepth; j++) {
			startingGrouping.mPath[i][j] = 255;
		}
	}

	o_groupingScheme.mParent = this;
	mPairRecycling.splice(mPairRecycling.end(), o_groupingScheme.mGroupings);
	o_groupingScheme.mGroupings.splice(o_groupingScheme.mGroupings.begin(), mPairRecycling, mPairRecycling.begin());

	if (mPairRecycling.size() > 100) {
		mPairRecycling.resize(50);
	}

	for (uint currentDepth = 0; currentDepth <= i_evaluations1[0]->mDepth; currentDepth++) {
		// for each dimension
		for (uint i = 0; i < n; i++) {
			uint nGroups = (uint)o_groupingScheme.mGroupings.size();
			auto groupIterator = o_groupingScheme.mGroupings.begin();

			// for each grouping
			for (uint j = 0; j < nGroups; j++) {
				while (mPairRecycling.size() < 2) {
					mPairRecycling.push_back(Grouping());
				}

				Grouping& parentGrouping = *groupIterator;

				Grouping& leftGrouping = mPairRecycling.front();
				leftGrouping.mEvals1.clear();
				leftGrouping.mEvals1.reserve(i_nElements1);
				leftGrouping.mEvals2.clear();
				leftGrouping.mEvals2.reserve(i_nElements2);

				Grouping& rightGrouping = mPairRecycling.back();
				rightGrouping.mEvals1.clear();
				rightGrouping.mEvals1.reserve(i_nElements1);
				rightGrouping.mEvals2.clear();
				rightGrouping.mEvals2.reserve(i_nElements2);

				for (auto indexIterator = parentGrouping.mEvals1.begin(); indexIterator != parentGrouping.mEvals1.end(); indexIterator++) {
					auto evaluationIterator = (*indexIterator)->getIterator();

					for (uint k = 0; k < currentDepth; k++) {
						if (!((parentGrouping.mPath[i][k]) ? evaluationIterator.goRight(i) : evaluationIterator.goLeft(i))) {
							throw ProcessFailureException();
						}
					}

					if (evaluationIterator.getLeft(i)) {
						leftGrouping.mEvals1.push_back(*indexIterator);
					}

					if (evaluationIterator.getRight(i)) {
						rightGrouping.mEvals1.push_back(*indexIterator);
					}
				}

				for (auto indexIterator = parentGrouping.mEvals2.begin(); indexIterator != parentGrouping.mEvals2.end(); indexIterator++) {
					auto evaluationIterator = (*indexIterator)->getIterator();

					for (uint k = 0; k < currentDepth; k++) {
						if (!((parentGrouping.mPath[i][k]) ? evaluationIterator.goRight(i) : evaluationIterator.goLeft(i))) {
							throw ProcessFailureException();
						}
					}

					if (evaluationIterator.getLeft(i)) {
						leftGrouping.mEvals2.push_back(*indexIterator);
					}

					if (evaluationIterator.getRight(i)) {
						rightGrouping.mEvals2.push_back(*indexIterator);
					}
				}

				if (leftGrouping.mEvals1.size() > 0 && leftGrouping.mEvals2.size() > 0) {
					for (uint k = 0; k < n; k++) {
						for (uint m = 0; m < maxDepth; m++) {
							leftGrouping.mPath[k][m] = parentGrouping.mPath[k][m];
						}
					}
					leftGrouping.mPath[i][currentDepth] = 0;

					o_groupingScheme.mGroupings.splice(o_groupingScheme.mGroupings.end(), mPairRecycling, mPairRecycling.begin());
				}

				if (rightGrouping.mEvals1.size() > 0 && rightGrouping.mEvals2.size() > 0) {
					for (uint k = 0; k < n; k++) {
						for (uint m = 0; m < maxDepth; m++) {
							rightGrouping.mPath[k][m] = parentGrouping.mPath[k][m];
						}
					}
					rightGrouping.mPath[i][currentDepth] = 1;

					o_groupingScheme.mGroupings.splice(o_groupingScheme.mGroupings.end(), mPairRecycling, --mPairRecycling.end());
				}

				groupIterator++;
			}
			auto begin = o_groupingScheme.mGroupings.begin();
			auto end = o_groupingScheme.mGroupings.begin();
			std::advance(end, nGroups);
			mPairRecycling.splice(mPairRecycling.end(), o_groupingScheme.mGroupings, begin, end);

			if (o_groupingScheme.mGroupings.size() == 0) {
				currentDepth = i_evaluations1[0]->mDepth + 1;
				break;
			}
		}
	}
}

template class BinaryCollisionTree<float, 2>;
template class BinaryCollisionTree<double, 2>;
