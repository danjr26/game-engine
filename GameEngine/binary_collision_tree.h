#ifndef BINARY_COLLISION_TREE_H
#define BINARY_COLLISION_TREE_H

#include <list>
#include <algorithm>
#include <mutex>
#include "collision_mask.h"
#include "axis_aligned_box.h"
#include "in_place_collision_evaluator.h"

template<class T, uint n>
class BinaryCollisionTree {
public:
	const static int maxDepth = 10;

	class Evaluation {
		friend class BinaryCollisionTree<T, n>;
		friend class Iterator;
	public:
		class Iterator {
			friend class Evaluation;
		private:
			Evaluation* parent;
			uint depths[n];
			uint evals[n];

			Iterator(Evaluation* in_parent) :
				parent(in_parent) {

				for (uint i = 0; i < n; i++) {
					depths[i] = 0;
					evals[i] = 0;
				}
			}
		public:
			Iterator() {}

			bool Go_Left(uint in_dimension) {
				if (!Get_Left(in_dimension) || depths[in_dimension] == parent->depth) {
					return false;
				}
				else {
					if (Get_Right(in_dimension)) {
						evals[in_dimension]++;
						uint skipStack[maxDepth];
						uint skipStackSize = 1;
						skipStack[0] = parent->depth - depths[in_dimension] + 1;
						while (skipStackSize > 0) {
							if (Get_Left(in_dimension) && Get_Right(in_dimension) && skipStack[skipStackSize - 1] > 1) {
								skipStack[skipStackSize] = --skipStack[skipStackSize - 1];
								skipStackSize++;
							}

							if (--skipStack[skipStackSize - 1] == 0) {
								skipStackSize--;
							}

							if (skipStackSize > 0) {
								evals[in_dimension]++;
							}
						}
					}
					else {
						evals[in_dimension]++;
					}
					
					depths[in_dimension]++;
					return true;
				}
			}

			bool Go_Right(uint in_dimension) {
				if (!Get_Right(in_dimension) || depths[in_dimension] == parent->depth) {
					return false;
				}
				else {
					evals[in_dimension]++;
					depths[in_dimension]++;
					return true;
				}
			}

			ubyte Get_Left(uint in_dimension) {
				return parent->data[in_dimension][evals[in_dimension] * 2 + 0];
			}

			ubyte Get_Right(uint in_dimension) {
				return parent->data[in_dimension][evals[in_dimension] * 2 + 1];
			}

			uint Get_Depth(uint in_dimension) {
				return depths[in_dimension];
			}
		};

	private:
		BinaryCollisionTree<T, n>* parentTree;
		CollisionMask<T, n>* parentMask;
		CollisionMask<T, n>* transformedParentMask;
		ubyte* data[n];
		uint dataSizes[n];
		uint depth;

	public:
		Evaluation() :
			parentTree(nullptr),
			parentMask(nullptr),
			transformedParentMask(nullptr),
			depth(UINT_MAX)
		{
			for (uint i = 0; i < n; i++) {
				data[i] = nullptr;
			}
		}

		Iterator Get_Iterator() {
			return Iterator(this);
		}

		BinaryCollisionTree<T, n>* Get_Parent_Tree() {
			return parentTree;
		}

		CollisionMask<T, n>* Get_Parent_Mask() {
			return parentMask;
		}

		CollisionMask<T, n>* Get_Transformed_Parent_Mask() {
			return transformedParentMask;
		}

		void Transform_Parent_Mask() {
			if (transformedParentMask != nullptr) {
				delete transformedParentMask;
			}

			transformedParentMask = parentMask->Clone();
			transformedParentMask->Apply_Transform();
			transformedParentMask->Set_Ignore_Transform(true);
		}
	};

	class GroupingScheme {
		friend class BinaryCollisionTree<T, n>;
	private:
		struct Grouping {
			ubyte path[n][maxDepth];
			std::vector<Evaluation*> evals;
		};

	public:
		class Iterator {
			friend class GroupingScheme;
		private:
			GroupingScheme* parent;
			typename std::list<Grouping>::iterator it;
			uint i, j;

			Iterator(GroupingScheme* in_parent) :
				parent(in_parent),
				it(in_parent->groupings.begin()),
				i(0),
				j(1)
			{}

		public:
			Evaluation* Get_First() {
				return it->evals[i];
			}

			Evaluation* Get_Second() {
				return it->evals[j];
			}

			void operator++(int a) {
				if (Is_Done()) {
					throw InvalidArgumentException();
				}

				j++;
				if (j >= it->evals.size()) {
					i++;
					j = i + 1;
				}
				if (i >= it->evals.size() - 1) {
					it++;
					i = 0;
					j = 1;
				}
			}

			void operator+=(uint n) {
				for (uint i = 0; i < n; i++) {
					if (Is_Done()) break;
					(*this)++;
				}
			}

			bool Is_Done() {
				return it == parent->groupings.end();
			}
		};
		friend class GroupingScheme::Iterator;

	private:

		BinaryCollisionTree<T, n>* parent;
		std::list<Grouping> groupings;

	public:
		uint Get_Number_Groups() const {
			return groupings.size();
		}

		Iterator Get_Iterator() {
			return Iterator(this);
		}

		void Get_Unique_Pairs(std::vector<std::pair<Evaluation*, Evaluation*>>& in_pairs) const {
			in_pairs.clear();
			in_pairs.reserve(groupings.size());
			for (auto groupIterator = groupings.begin(); groupIterator != groupings.end(); groupIterator++) {
				for (uint i = 0; i < groupIterator->evals.size(); i++) {
					for (uint j = i + 1; j < groupIterator->evals.size(); j++) {
						auto newPair = std::pair<Evaluation*, Evaluation*>(groupIterator->evals[i], groupIterator->evals[j]);
						auto lowerBound = std::lower_bound(in_pairs.begin(), in_pairs.end(), newPair);
						if (*lowerBound != newPair) {
							in_pairs.insert(lowerBound, newPair);
						}
					}
				}
			}
		}
	};

	class PairedGroupingScheme {
		friend class BinaryCollisionTree<T, n>;
	private:
		struct Grouping {
			ubyte path[n][maxDepth];
			std::vector<Evaluation*> evals1;
			std::vector<Evaluation*> evals2;
		};

	public:
		class Iterator {
			friend class GroupingScheme;
		private:
			GroupingScheme* parent;
			typename std::list<Grouping>::iterator it;
			uint i, j;

			Iterator(GroupingScheme* in_parent) :
				parent(in_parent),
				it(in_parent->groupings.begin()),
				i(0),
				j(0) 
			{}

		public:
			Evaluation* Get_First() {
				return it->evals1[i];
			}

			Evaluation* Get_Second() {
				return it->evals2[j];
			}

			Iterator& operator++(int a) {
				if (Is_Done()) {
					throw InvalidArgumentException();
				}

				j++;
				if (j >= it->evals2.size()) {
					i++;
					j = 0;
				}
				if (i >= it->evals1.size()) {
					it++;
					i = 0;
					j = 0;
				}

				return *this;
			}

			void operator+=(uint n) {
				for (uint i = 0; i < n; i++) {
					if (Is_Done()) break;
					(*this)++;
				}
			}

			bool Is_Done() {
				return it == parent->groupings.end();
			}
		};
		friend class PairedGroupingScheme::Iterator;

	private:

		BinaryCollisionTree<T, n>* parent;
		std::list<Grouping> groupings;

	public:
		uint Get_Number_Groups() const {
			return groupings.size();
		}

		Iterator Get_Iterator() {
			return Iterator(this);
		}

		void Get_Unique_Pairs(std::vector<std::pair<Evaluation*, Evaluation*>>& in_pairs) const {
			in_pairs.clear();
			in_pairs.reserve(groupings.size());
			for (auto groupIterator = groupings.begin(); groupIterator != groupings.end(); groupIterator++) {
				for (uint i = 0; i < groupIterator->evals1.size(); i++) {
					for (uint j = 0; j < groupIterator->evals2.size(); j++) {
						if (groupIterator->evals1[i] != groupIterator->evals2[j]) {
							in_pairs.push_back(std::pair(groupIterator->evals1[i], groupIterator->evals2[j]));
						}
					}
				}
			}
		}
	};

private:
	AxisAlignedBox<T, n> box;

	struct EvaluationStackElement {
		uint dimension;
		T value;
		bool isPositive;
		uint depth;
	};

	struct IntersectionStackElement {
		typename Evaluation::Iterator it1;
		typename Evaluation::Iterator it2;
	};

	std::list<typename GroupingScheme::Grouping> recycling;
	std::list<typename PairedGroupingScheme::Grouping> pairRecycling;

	template<typename = std::enable_if_t<n == 2>>
	using AxisAlignedHalfSpaceCollisionMask = AxisAlignedHalfSpace2CollisionMask<T>;
	using InPlaceCollisionEvaluator = InPlaceCollisionEvaluator<T, n>;

public:
	BinaryCollisionTree(const AxisAlignedBox<T, n>& in_box) :
		box(in_box)
	{}

	void Evaluate(CollisionMask<T, n>* in_mask, uint in_depth, Evaluation& out_evaluation) {
		using StackElement = EvaluationStackElement;
		if (in_depth > maxDepth) {
			throw InvalidArgumentException();
		}

		out_evaluation.parentTree = this;
		out_evaluation.parentMask = in_mask;
		out_evaluation.Transform_Parent_Mask();

		for (uint i = 0; i < n; i++) {
			if (out_evaluation.depth != in_depth) {
				if (out_evaluation.data[i] != nullptr) {
					delete[] out_evaluation.data[i];
				}
				out_evaluation.data[i] = new ubyte[(1ull << (in_depth + 2))];
			} 
			out_evaluation.dataSizes[i] = 0;
		}
		out_evaluation.depth = in_depth;

		InPlaceCollisionEvaluator evaluator;
		evaluator.Return_Point(false);

		EvaluationStackElement evaluationStacks[n][1 << (maxDepth + 1)];
		uint stackSizes[3];
		for (uint i = 0; i < n; i++) {
			stackSizes[i] = 1;
			evaluationStacks[i][0] = { i, box.Get_Center()[i], false, 0 };
		}

		AxisAlignedHalfSpace<T, n> halfSpace1 = AxisAlignedHalfSpace<T, n>::From_Dimension_Value(0, 0.0, false);
		AxisAlignedHalfSpace<T, n> halfSpace2 = AxisAlignedHalfSpace<T, n>::From_Dimension_Value(0, 0.0, false);
		AxisAlignedHalfSpaceCollisionMask<> mask1 = AxisAlignedHalfSpaceCollisionMask<>(halfSpace1, true);
		AxisAlignedHalfSpaceCollisionMask<> mask2 = AxisAlignedHalfSpaceCollisionMask<>(halfSpace2, true);

		for (bool keepGoing = true; keepGoing;) {
			keepGoing = false;
			for (uint i = 0; i < n; i++) {
				if (stackSizes[i] == 0) continue;
				StackElement& back = evaluationStacks[i][--stackSizes[i]];

				halfSpace1 = AxisAlignedHalfSpace<T, n>::From_Dimension_Value(
					back.dimension, back.value, back.isPositive);
				halfSpace2 = AxisAlignedHalfSpace<T, n>::From_Dimension_Value(
					back.dimension, back.value, !back.isPositive);

				mask1.Get_Half_Space() = halfSpace1;
				mask2.Get_Half_Space() = halfSpace2;

				bool didCollide1 = evaluator.Evaluate(*out_evaluation.Get_Transformed_Parent_Mask(), mask1).didCollide;
				bool didCollide2 = evaluator.Evaluate(*out_evaluation.Get_Transformed_Parent_Mask(), mask2).didCollide;

				out_evaluation.data[i][out_evaluation.dataSizes[i]++] = didCollide1;
				out_evaluation.data[i][out_evaluation.dataSizes[i]++] = didCollide2;

				T nextOffset = box.Get_Dimensions()[halfSpace1.Get_Dimension()] / (1 << (back.depth + 2));
				uint lastDepth = back.depth;

				if (lastDepth < in_depth) {
					if (didCollide1) {
						evaluationStacks[i][stackSizes[i]++] = {
							halfSpace1.Get_Dimension(),
							halfSpace1.Get_Value() + nextOffset,
							false,
							lastDepth + 1
						};
					}

					if (didCollide2) {
						evaluationStacks[i][stackSizes[i]++] = {
							halfSpace2.Get_Dimension(),
							halfSpace2.Get_Value() - nextOffset,
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

	bool Intersection(Evaluation& in_evaluation1, Evaluation& in_evaluation2) {
		using StackElement = IntersectionStackElement;

		IntersectionStackElement intersectionStacks[n][1 << (maxDepth + 1)];
		uint stackSizes[n];
		bool didIntersect[n];
		ubyte* data[2][n];

		uint skipStackSize = 0;

		for (uint i = 0; i < n; i++) {
			stackSizes[i] = 1;
			intersectionStacks[i][0] = { in_evaluation1.Get_Iterator(), in_evaluation2.Get_Iterator() };
			didIntersect[i] = false;
			data[0][i] = &in_evaluation1.data[i][0];
			data[1][i] = &in_evaluation2.data[i][0];
		}
		
		for (bool keepGoing = true; keepGoing;) {
			keepGoing = false;
			for (uint i = 0; i < n; i++) {
				if (stackSizes[i] == 0) continue;

				StackElement element = intersectionStacks[i][--stackSizes[i]];

				if (element.it1.Get_Left(i) && element.it2.Get_Left(i)) {
					StackElement& newElement = intersectionStacks[i][stackSizes[i]++];
					newElement.it1 = element.it1;
					newElement.it2 = element.it2;
					if (!newElement.it1.Go_Left(i) || !newElement.it2.Go_Left(i)) {
						stackSizes[i] = 0;
						didIntersect[i] = true;
						continue;
					}
				}

				if (element.it1.Get_Right(i) && element.it2.Get_Right(i)) {
					StackElement& newElement = intersectionStacks[i][stackSizes[i]++];
					newElement.it1 = element.it1;
					newElement.it2 = element.it2;
					if (!newElement.it1.Go_Right(i) || !newElement.it2.Go_Right(i)) {
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

	void Group(Evaluation** in_evaluations, uint in_nElements, GroupingScheme& out_groupingScheme) {
		using Grouping = GroupingScheme::Grouping;

		if (recycling.size() < 1) {
			recycling.resize(1);
		}

		Grouping& startingGrouping = recycling.front();
		startingGrouping.evals.resize(in_nElements);

		uint i = 0;
		for (auto it = startingGrouping.evals.begin(); it != startingGrouping.evals.end(); it++) {
			*it = in_evaluations[i++];
		}

		for (uint i = 0; i < n; i++) {
			for (uint j = 0; j < maxDepth; j++) {
				startingGrouping.path[i][j] = 255;
			}
		}

		out_groupingScheme.parent = this;
		recycling.splice(recycling.end(), out_groupingScheme.groupings);
		out_groupingScheme.groupings.splice(out_groupingScheme.groupings.begin(), recycling, recycling.begin());

		if (recycling.size() > 100) {
			recycling.resize(50);
		}

		for(uint currentDepth = 0; currentDepth <= in_evaluations[0]->depth; currentDepth++) {
			// for each dimension
			for (uint i = 0; i < n; i++) {
				uint nGroups = (uint)out_groupingScheme.groupings.size();
				auto groupIterator = out_groupingScheme.groupings.begin();

				// for each grouping
				for (uint j = 0; j < nGroups; j++) {
					while (recycling.size() < 2) {
						recycling.push_back(Grouping());
					}

					Grouping& parentGrouping = *groupIterator;
					Grouping& leftGrouping = recycling.front();
					Grouping& rightGrouping = recycling.back();
					leftGrouping.evals.clear();
					rightGrouping.evals.clear();
					leftGrouping.evals.reserve(in_nElements);
					rightGrouping.evals.reserve(in_nElements);

					// for each evaluation in that grouping 
					for (auto indexIterator = parentGrouping.evals.begin(); indexIterator != parentGrouping.evals.end(); indexIterator++) {
						auto evaluationIterator = (*indexIterator)->Get_Iterator();

						for (uint k = 0; k < currentDepth; k++) {
							if (!((parentGrouping.path[i][k]) ? evaluationIterator.Go_Right(i) : evaluationIterator.Go_Left(i))) {
								throw ProcessFailureException();
							}
						}

						if (evaluationIterator.Get_Left(i)) {
							leftGrouping.evals.push_back(*indexIterator);
						}

						if (evaluationIterator.Get_Right(i)) {
							rightGrouping.evals.push_back(*indexIterator);
						}
					}

					if (leftGrouping.evals.size() > 1) {
						for (uint k = 0; k < n; k++) {
							for (uint m = 0; m < maxDepth; m++) {
								leftGrouping.path[k][m] = parentGrouping.path[k][m];
							}
						}
						leftGrouping.path[i][currentDepth] = 0;

						out_groupingScheme.groupings.splice(out_groupingScheme.groupings.end(), recycling, recycling.begin());
					}

					if (rightGrouping.evals.size() > 1) {
						for (uint k = 0; k < n; k++) {
							for (uint m = 0; m < maxDepth; m++) {
								rightGrouping.path[k][m] = parentGrouping.path[k][m];
							}
						}
						rightGrouping.path[i][currentDepth] = 1;

						out_groupingScheme.groupings.splice(out_groupingScheme.groupings.end(), recycling, --recycling.end());
					}
					groupIterator++;
				}
				auto begin = out_groupingScheme.groupings.begin();
				auto end = out_groupingScheme.groupings.begin();
				std::advance(end, nGroups);
				recycling.splice(recycling.end(), out_groupingScheme.groupings, begin, end);

				if (out_groupingScheme.groupings.size() == 0) {
					currentDepth = in_evaluations[0]->depth;
					break;
				}
			}
		}
	}

	void Group(Evaluation** in_evaluations1, uint in_nElements1, Evaluation** in_evaluations2, uint in_nElements2, PairedGroupingScheme& out_groupingScheme) {
		using Grouping = PairedGroupingScheme::Grouping;

		if (pairRecycling.size() < 1) {
			pairRecycling.resize(1);
		}

		Grouping& startingGrouping = pairRecycling.front();
		startingGrouping.evals1.resize(in_nElements1);
		startingGrouping.evals2.resize(in_nElements2);

		uint i = 0;
		for (auto it = startingGrouping.evals1.begin(); it != startingGrouping.evals1.end(); it++) {
			*it = in_evaluations1[i++];
		}

		i = 0;
		for (auto it = startingGrouping.evals2.begin(); it != startingGrouping.evals2.end(); it++) {
			*it = in_evaluations2[i++];
		}

		for (uint i = 0; i < n; i++) {
			for (uint j = 0; j < maxDepth; j++) {
				startingGrouping.path[i][j] = 255;
			}
		}

		out_groupingScheme.parent = this;
		pairRecycling.splice(pairRecycling.end(), out_groupingScheme.groupings);
		out_groupingScheme.groupings.splice(out_groupingScheme.groupings.begin(), pairRecycling, pairRecycling.begin());

		if (pairRecycling.size() > 100) {
			pairRecycling.resize(50);
		}

		for (uint currentDepth = 0; currentDepth < in_evaluations1[0]->depth; currentDepth++) {
			// for each dimension
			for (uint i = 0; i < n; i++) {
				uint nGroups = (uint) out_groupingScheme.groupings.size();
				auto groupIterator = out_groupingScheme.groupings.begin();

				// for each grouping
				for (uint j = 0; j < nGroups; j++) {
					while (pairRecycling.size() < 2) {
						pairRecycling.push_back(Grouping());
					}

					Grouping& parentGrouping = *groupIterator;

					Grouping& leftGrouping = pairRecycling.front();
					leftGrouping.evals1.clear();
					leftGrouping.evals1.reserve(in_nElements1);
					leftGrouping.evals2.clear();
					leftGrouping.evals2.reserve(in_nElements2);

					Grouping& rightGrouping = pairRecycling.back();
					rightGrouping.evals1.clear();
					rightGrouping.evals1.reserve(in_nElements1);
					rightGrouping.evals2.clear();
					rightGrouping.evals2.reserve(in_nElements2);

					for (auto indexIterator = parentGrouping.evals1.begin(); indexIterator != parentGrouping.evals1.end(); indexIterator++) {
						auto evaluationIterator = (*indexIterator)->Get_Iterator();

						for (uint k = 0; k < currentDepth; k++) {
							if (!((parentGrouping.path[i][k]) ? evaluationIterator.Go_Right(i) : evaluationIterator.Go_Left(i))) {
								throw ProcessFailureException();
							}
						}

						if (evaluationIterator.Get_Left(i)) {
							leftGrouping.evals1.push_back(*indexIterator);
						}

						if (evaluationIterator.Get_Right(i)) {
							rightGrouping.evals1.push_back(*indexIterator);
						}
					}

					for (auto indexIterator = parentGrouping.evals2.begin(); indexIterator != parentGrouping.evals2.end(); indexIterator++) {
						auto evaluationIterator = (*indexIterator)->Get_Iterator();

						for (uint k = 0; k < currentDepth; k++) {
							if (!((parentGrouping.path[i][k]) ? evaluationIterator.Go_Right(i) : evaluationIterator.Go_Left(i))) {
								throw ProcessFailureException();
							}
						}

						if (evaluationIterator.Get_Left(i)) {
							leftGrouping.evals2.push_back(*indexIterator);
						}

						if (evaluationIterator.Get_Right(i)) {
							rightGrouping.evals2.push_back(*indexIterator);
						}
					}

					if (leftGrouping.evals1.size() > 1 && leftGrouping.evals2.size() > 1) {
						for (uint k = 0; k < n; k++) {
							for (uint m = 0; m < maxDepth; m++) {
								leftGrouping.path[k][m] = parentGrouping.path[k][m];
							}
						}
						leftGrouping.path[i][currentDepth] = 0;

						out_groupingScheme.groupings.splice(out_groupingScheme.groupings.end(), pairRecycling, pairRecycling.begin());
					}

					if (rightGrouping.evals1.size() > 1 && leftGrouping.evals2.size() > 1) {
						for (uint k = 0; k < n; k++) {
							for (uint m = 0; m < maxDepth; m++) {
								rightGrouping.path[k][m] = parentGrouping.path[k][m];
							}
						}
						rightGrouping.path[i][currentDepth] = 1;

						out_groupingScheme.groupings.splice(out_groupingScheme.groupings.end(), pairRecycling, --pairRecycling.end());
					}
					
					groupIterator++;
				}
				auto begin = out_groupingScheme.groupings.begin();
				auto end = out_groupingScheme.groupings.begin();
				std::advance(end, nGroups);
				pairRecycling.splice(pairRecycling.end(), out_groupingScheme.groupings, begin, end);

				if (out_groupingScheme.groupings.size() == 0) {
					currentDepth = in_evaluations1[0]->depth;
					break;
				}
			}
		}
	}

	friend class Evaluation::Iterator;
};

using QuadTreef = BinaryCollisionTree<float, 2>;
using QuadTreed = BinaryCollisionTree<double, 2>;
using OctTreef = BinaryCollisionTree<float, 3>;
using OctTreed = BinaryCollisionTree<double, 3>;

#endif

