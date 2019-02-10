#include "binary_collision_tree.h"
#include "basic_collision_mask.h"

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::Iterator::Iterator(Evaluation* in_parent) :
	mParent(in_parent) {

	for (uint i = 0; i < n; i++) {
		mDepths[i] = 0;
		mEvals[i] = 0;
	}
}

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::Iterator::Iterator() 
{}

template<class T, uint n>
bool BinaryCollisionTree<T, n>::Evaluation::Iterator::goLeft(uint in_dimension) {
	if (!getLeft(in_dimension) || mDepths[in_dimension] == mParent->mDepth) {
		return false;
	}
	else {
		if (getRight(in_dimension)) {
			mEvals[in_dimension]++;
			uint skipStack[maxDepth];
			uint skipStackSize = 1;
			skipStack[0] = mParent->mDepth - mDepths[in_dimension] + 1;
			while (skipStackSize > 0) {
				if (getLeft(in_dimension) && getRight(in_dimension) && skipStack[skipStackSize - 1] > 1) {
					skipStack[skipStackSize] = --skipStack[skipStackSize - 1];
					skipStackSize++;
				}

				if (--skipStack[skipStackSize - 1] == 0) {
					skipStackSize--;
				}

				if (skipStackSize > 0) {
					mEvals[in_dimension]++;
				}
			}
		}
		else {
			mEvals[in_dimension]++;
		}

		mDepths[in_dimension]++;
		return true;
	}
}

template<class T, uint n>
bool BinaryCollisionTree<T, n>::Evaluation::Iterator::goRight(uint in_dimension) {
	if (!getRight(in_dimension) || mDepths[in_dimension] == mParent->mDepth) {
		return false;
	}
	else {
		mEvals[in_dimension]++;
		mDepths[in_dimension]++;
		return true;
	}
}

template<class T, uint n>
ubyte BinaryCollisionTree<T, n>::Evaluation::Iterator::getLeft(uint in_dimension) {
	return mParent->mData[in_dimension][mEvals[in_dimension] * 2 + 0];
}

template<class T, uint n>
ubyte BinaryCollisionTree<T, n>::Evaluation::Iterator::getRight(uint in_dimension) {
	return mParent->mData[in_dimension][mEvals[in_dimension] * 2 + 1];
}

template<class T, uint n>
uint BinaryCollisionTree<T, n>::Evaluation::Iterator::getDepth(uint in_dimension) {
	return mDepths[in_dimension];
}

template<class T, uint n>
BinaryCollisionTree<T, n>::Evaluation::Evaluation() :
	mParentTree(nullptr),
	mParentMask(nullptr),
	mTransformedParentMask(nullptr),
	mDepth(UINT_MAX) {
	for (uint i = 0; i < n; i++) {
		mData[i] = nullptr;
	}
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
CollisionMask<T, n>* BinaryCollisionTree<T, n>::Evaluation::getParentMask() {
	return mParentMask;
}

template<class T, uint n>
CollisionMask<T, n>* BinaryCollisionTree<T, n>::Evaluation::getTransformedParentMask() {
	return mTransformedParentMask;
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::Evaluation::transformParentMask() {
	if (mTransformedParentMask != nullptr) {
		delete mTransformedParentMask;
	}

	mTransformedParentMask = mParentMask->clone();
	mTransformedParentMask->applyTransform();
	mTransformedParentMask->setIgnoreTransform(true);
}

template<class T, uint n>
BinaryCollisionTree<T, n>::GroupingScheme::Iterator::Iterator(GroupingScheme* in_parent) :
	mParent(in_parent),
	mIt(in_parent->mGroupings.begin()),
	mI(0),
	mJ(1) {}

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
void BinaryCollisionTree<T, n>::GroupingScheme::remove(const Evaluation * in_eval) {
	for (auto it = mGroupings.begin(); it != mGroupings.end(); it++) {
		auto search = std::find(it->mEvals.begin(), it->mEvals.end(), in_eval);
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
void BinaryCollisionTree<T, n>::GroupingScheme::getUniquePairs(std::vector<std::pair<Evaluation*, Evaluation*>>& in_pairs) const {
	in_pairs.clear();
	in_pairs.reserve(mGroupings.size());
	for (auto groupIterator = mGroupings.begin(); groupIterator != mGroupings.end(); groupIterator++) {
		for (uint i = 0; i < groupIterator->mEvals.size(); i++) {
			for (uint j = i + 1; j < groupIterator->mEvals.size(); j++) {
				auto newPair = std::pair<Evaluation*, Evaluation*>(groupIterator->mEvals[i], groupIterator->mEvals[j]);
				auto lowerBound = std::lower_bound(in_pairs.begin(), in_pairs.end(), newPair);
				if (*lowerBound != newPair) {
					in_pairs.insert(lowerBound, newPair);
				}
			}
		}
	}
}

template<class T, uint n>
BinaryCollisionTree<T, n>::PairedGroupingScheme::Iterator::Iterator(PairedGroupingScheme * in_parent) :
	mParent(in_parent),
	mIt(in_parent->mGroupings.begin()),
	mI(0),
	mJ(0) {}

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
void BinaryCollisionTree<T, n>::PairedGroupingScheme::getUniquePairs(std::vector<std::pair<Evaluation*, Evaluation*>>& in_pairs) const {
	in_pairs.clear();
	in_pairs.reserve(mGroupings.size());
	for (auto groupIterator = mGroupings.begin(); groupIterator != mGroupings.end(); groupIterator++) {
		for (uint i = 0; i < groupIterator->mEvals1.size(); i++) {
			for (uint j = 0; j < groupIterator->mEvals2.size(); j++) {
				if (groupIterator->mEvals1[i] != groupIterator->mEvals2[j]) {
					in_pairs.push_back(std::pair(groupIterator->mEvals1[i], groupIterator->mEvals2[j]));
				}
			}
		}
	}
}

template<class T, uint n>
BinaryCollisionTree<T, n>::BinaryCollisionTree(const AxisAlignedBox<T, n>& in_box) :
	mBox(in_box) {}

template<class T, uint n>
void BinaryCollisionTree<T, n>::evaluate(CollisionMask<T, n>* in_mask, uint in_depth, Evaluation & out_evaluation) {
	using StackElement = EvaluationStackElement;
	if (in_depth > maxDepth) {
		throw InvalidArgumentException();
	}

	out_evaluation.mParentTree = this;
	out_evaluation.mParentMask = in_mask;
	out_evaluation.transformParentMask();

	for (uint i = 0; i < n; i++) {
		if (out_evaluation.mDepth != in_depth) {
			if (out_evaluation.mData[i] != nullptr) {
				delete[] out_evaluation.mData[i];
			}
			out_evaluation.mData[i] = new ubyte[(1ull << (in_depth + 2))];
		}
		out_evaluation.mDataSizes[i] = 0;
	}
	out_evaluation.mDepth = in_depth;

	InPlaceCollisionEvaluator evaluator;
	evaluator.returnPoint(false);

	EvaluationStackElement evaluationStacks[n][1 << (maxDepth + 1)];
	uint stackSizes[3];
	for (uint i = 0; i < n; i++) {
		stackSizes[i] = 1;
		evaluationStacks[i][0] = { i, mBox.getCenter()[i], false, 0 };
	}

	AxisAlignedHalfSpace<T, n> halfSpace1 = AxisAlignedHalfSpace<T, n>::fromDimensionValue(0, 0.0, false);
	AxisAlignedHalfSpace<T, n> halfSpace2 = AxisAlignedHalfSpace<T, n>::fromDimensionValue(0, 0.0, false);
	typename BinaryCollisionTree<T, n>::AxisAlignedHalfSpaceCollisionMask<> mask1(halfSpace1, true);
	typename BinaryCollisionTree<T, n>::AxisAlignedHalfSpaceCollisionMask<> mask2(halfSpace2, true);

	for (bool keepGoing = true; keepGoing;) {
		keepGoing = false;
		for (uint i = 0; i < n; i++) {
			if (stackSizes[i] == 0) continue;
			StackElement& back = evaluationStacks[i][--stackSizes[i]];

			halfSpace1 = AxisAlignedHalfSpace<T, n>::fromDimensionValue(
				back.mDimension, back.mValue, back.mIsPositive);
			halfSpace2 = AxisAlignedHalfSpace<T, n>::fromDimensionValue(
				back.mDimension, back.mValue, !back.mIsPositive);

			mask1.getBasis() = halfSpace1;
			mask2.getBasis() = halfSpace2;

			bool didCollide1 = evaluator.evaluate(*out_evaluation.getTransformedParentMask(), mask1).mDid;
			bool didCollide2 = evaluator.evaluate(*out_evaluation.getTransformedParentMask(), mask2).mDid;

			out_evaluation.mData[i][out_evaluation.mDataSizes[i]++] = didCollide1;
			out_evaluation.mData[i][out_evaluation.mDataSizes[i]++] = didCollide2;

			T nextOffset = mBox.getDimensions()[halfSpace1.getDimension()] / (1 << (back.mDepth + 2));
			uint lastDepth = back.mDepth;

			if (lastDepth < in_depth) {
				if (didCollide1) {
					evaluationStacks[i][stackSizes[i]++] = {
						halfSpace1.getDimension(),
						halfSpace1.getValue() + nextOffset,
						false,
						lastDepth + 1
					};
				}

				if (didCollide2) {
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
bool BinaryCollisionTree<T, n>::intersection(Evaluation & in_evaluation1, Evaluation & in_evaluation2) {
	using StackElement = IntersectionStackElement;

	IntersectionStackElement intersectionStacks[n][1 << (maxDepth + 1)];
	uint stackSizes[n];
	bool didIntersect[n];
	ubyte* mData[2][n];

	uint skipStackSize = 0;

	for (uint i = 0; i < n; i++) {
		stackSizes[i] = 1;
		intersectionStacks[i][0] = { in_evaluation1.getIterator(), in_evaluation2.getIterator() };
		didIntersect[i] = false;
		mData[0][i] = &in_evaluation1.mData[i][0];
		mData[1][i] = &in_evaluation2.mData[i][0];
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
void BinaryCollisionTree<T, n>::group(Evaluation ** in_evaluations, uint in_nElements, GroupingScheme & out_groupingScheme) {
	using Grouping = GroupingScheme::Grouping;

	if (mRecycling.size() < 1) {
		mRecycling.resize(1);
	}

	Grouping& startingGrouping = mRecycling.front();
	startingGrouping.mEvals.resize(in_nElements);

	uint i = 0;
	for (auto it = startingGrouping.mEvals.begin(); it != startingGrouping.mEvals.end(); it++) {
		*it = in_evaluations[i++];
	}

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < maxDepth; j++) {
			startingGrouping.mPath[i][j] = 255;
		}
	}

	out_groupingScheme.mParent = this;
	mRecycling.splice(mRecycling.end(), out_groupingScheme.mGroupings);
	out_groupingScheme.mGroupings.splice(out_groupingScheme.mGroupings.begin(), mRecycling, mRecycling.begin());

	if (mRecycling.size() > 100) {
		mRecycling.resize(50);
	}

	for (uint currentDepth = 0; currentDepth <= in_evaluations[0]->mDepth; currentDepth++) {
		// for each dimension
		for (uint i = 0; i < n; i++) {
			uint nGroups = (uint)out_groupingScheme.mGroupings.size();
			auto groupIterator = out_groupingScheme.mGroupings.begin();

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
				leftGrouping.mEvals.reserve(in_nElements);
				rightGrouping.mEvals.reserve(in_nElements);

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

					out_groupingScheme.mGroupings.splice(out_groupingScheme.mGroupings.end(), mRecycling, mRecycling.begin());
				}

				if (rightGrouping.mEvals.size() > 1) {
					for (uint k = 0; k < n; k++) {
						for (uint m = 0; m < maxDepth; m++) {
							rightGrouping.mPath[k][m] = parentGrouping.mPath[k][m];
						}
					}
					rightGrouping.mPath[i][currentDepth] = 1;

					out_groupingScheme.mGroupings.splice(out_groupingScheme.mGroupings.end(), mRecycling, --mRecycling.end());
				}
				groupIterator++;
			}
			auto begin = out_groupingScheme.mGroupings.begin();
			auto end = out_groupingScheme.mGroupings.begin();
			std::advance(end, nGroups);
			mRecycling.splice(mRecycling.end(), out_groupingScheme.mGroupings, begin, end);

			if (out_groupingScheme.mGroupings.size() == 0) {
				currentDepth = in_evaluations[0]->mDepth + 1;
				break;
			}
		}
	}
}

template<class T, uint n>
void BinaryCollisionTree<T, n>::group(Evaluation ** in_evaluations1, uint in_nElements1, Evaluation ** in_evaluations2, uint in_nElements2, PairedGroupingScheme & out_groupingScheme) {
	using Grouping = PairedGroupingScheme::Grouping;

	if (mPairRecycling.size() < 1) {
		mPairRecycling.resize(1);
	}

	Grouping& startingGrouping = mPairRecycling.front();
	startingGrouping.mEvals1.resize(in_nElements1);
	startingGrouping.mEvals2.resize(in_nElements2);

	uint i = 0;
	for (auto it = startingGrouping.mEvals1.begin(); it != startingGrouping.mEvals1.end(); it++) {
		*it = in_evaluations1[i++];
	}

	i = 0;
	for (auto it = startingGrouping.mEvals2.begin(); it != startingGrouping.mEvals2.end(); it++) {
		*it = in_evaluations2[i++];
	}

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < maxDepth; j++) {
			startingGrouping.mPath[i][j] = 255;
		}
	}

	out_groupingScheme.mParent = this;
	mPairRecycling.splice(mPairRecycling.end(), out_groupingScheme.mGroupings);
	out_groupingScheme.mGroupings.splice(out_groupingScheme.mGroupings.begin(), mPairRecycling, mPairRecycling.begin());

	if (mPairRecycling.size() > 100) {
		mPairRecycling.resize(50);
	}

	for (uint currentDepth = 0; currentDepth <= in_evaluations1[0]->mDepth; currentDepth++) {
		// for each dimension
		for (uint i = 0; i < n; i++) {
			uint nGroups = (uint)out_groupingScheme.mGroupings.size();
			auto groupIterator = out_groupingScheme.mGroupings.begin();

			// for each grouping
			for (uint j = 0; j < nGroups; j++) {
				while (mPairRecycling.size() < 2) {
					mPairRecycling.push_back(Grouping());
				}

				Grouping& parentGrouping = *groupIterator;

				Grouping& leftGrouping = mPairRecycling.front();
				leftGrouping.mEvals1.clear();
				leftGrouping.mEvals1.reserve(in_nElements1);
				leftGrouping.mEvals2.clear();
				leftGrouping.mEvals2.reserve(in_nElements2);

				Grouping& rightGrouping = mPairRecycling.back();
				rightGrouping.mEvals1.clear();
				rightGrouping.mEvals1.reserve(in_nElements1);
				rightGrouping.mEvals2.clear();
				rightGrouping.mEvals2.reserve(in_nElements2);

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

					out_groupingScheme.mGroupings.splice(out_groupingScheme.mGroupings.end(), mPairRecycling, mPairRecycling.begin());
				}

				if (rightGrouping.mEvals1.size() > 0 && rightGrouping.mEvals2.size() > 0) {
					for (uint k = 0; k < n; k++) {
						for (uint m = 0; m < maxDepth; m++) {
							rightGrouping.mPath[k][m] = parentGrouping.mPath[k][m];
						}
					}
					rightGrouping.mPath[i][currentDepth] = 1;

					out_groupingScheme.mGroupings.splice(out_groupingScheme.mGroupings.end(), mPairRecycling, --mPairRecycling.end());
				}

				groupIterator++;
			}
			auto begin = out_groupingScheme.mGroupings.begin();
			auto end = out_groupingScheme.mGroupings.begin();
			std::advance(end, nGroups);
			mPairRecycling.splice(mPairRecycling.end(), out_groupingScheme.mGroupings, begin, end);

			if (out_groupingScheme.mGroupings.size() == 0) {
				currentDepth = in_evaluations1[0]->mDepth + 1;
				break;
			}
		}
	}
}

template class BinaryCollisionTree<float, 2>;
template class BinaryCollisionTree<double, 2>;
