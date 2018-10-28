#ifndef BINARY_COLLISION_TREE_H
#define BINARY_COLLISION_TREE_H

#include <list>
#include "collision_mask.h"
#include "axis_aligned_box.h"
#include "in_place_collision_evaluator2.h"

template<uint n>
class BinaryCollisionTree {
public:
	const static int maxDepth = 10;

	class Evaluation {
		friend class BinaryCollisionTree<n>;
		friend class Iterator;
	public:
		class Iterator {
			friend class Evaluation;
		private:
			Evaluation* parent;
			uint depths[n];
			uint indices[n];

			Iterator(Evaluation* in_parent) :
				parent(in_parent) {

				for (uint i = 0; i < n; i++) {
					depths[i] = 0;
					indices[i] = 0;
				}
			}
		public:
			Iterator() {}

			bool Go_Left(uint in_dimension) {
				if (!Get_Left(in_dimension) || depths[in_dimension] == parent->depth) {
					return false;
				}
				else {
					indices[in_dimension]++;
					if (Get_Right(in_dimension)) {
						uint* skipStack = parent->parent->skipStack;
						uint skipStackSize = 0;
						skipStack[0] = parent->depth - depths[in_dimension] + 1;
						while (skipStackSize > 0) {
							if (Get_Left(in_dimension) && Get_Right(in_dimension) && skipStack[skipStackSize - 1] > 1) {
								skipStack[skipStackSize] = skipStack[skipStackSize - 1] - 1;
								skipStackSize++;
							}

							if (--skipStack[skipStackSize - 1] == 0) {
								skipStackSize--;
							}

							if (skipStackSize > 0) {
								indices[in_dimension]++;
							}
						}
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
					indices[in_dimension]++;
					depths[in_dimension]++;
					return true;
				}
			}

			ubyte Get_Left(uint in_dimension) {
				return parent->data[in_dimension][indices[in_dimension] * 2 + 0];
			}

			ubyte Get_Right(uint in_dimension) {
				return parent->data[in_dimension][indices[in_dimension] * 2 + 1];
			}

			uint Get_Depth(uint in_dimension) {
				return depths[in_dimension];
			}
		};

	private:
		BinaryCollisionTree<n>* parent;
		ubyte* data[n];
		uint dataSizes[n];
		uint depth;

	public:
		Evaluation() :
			parent(nullptr),
			depth(UINT_MAX)
		{}
		Iterator Get_Iterator() {
			return Iterator(this);
		}
	};

	class GroupingScheme {
		friend class BinaryCollisionTree<n>;
	private:
		struct Grouping {
			ubyte path[n][maxDepth];
			std::vector<uint> indices;
		};

		BinaryCollisionTree<n>* parent;
		std::list<Grouping> groupings;

	public:
		uint Get_Number_Groups() const {
			return groupings.size();
		}
		void Get_Unique_Pairs(std::vector<std::pair<uint, uint>>& in_pairs) const {
			in_pairs.clear();
			in_pairs.reserve(groupings.size());
			for (auto groupIterator = groupings.begin(); groupIterator != groupings.end(); groupIterator++) {
				for (uint i = 0; i < groupIterator->indices.size(); i++) {
					for (uint j = i + 1; j < groupIterator->indices.size(); j++) {
						bool isCopy = false;
						for (uint k = 0; k < in_pairs.size(); k++) {
							if (in_pairs[k].first == groupIterator->indices[i] && in_pairs[k].second == groupIterator->indices[j]) {
								isCopy = true;
								break;
							}
						}
						if (!isCopy) {
							in_pairs.push_back(std::pair<uint, uint>(groupIterator->indices[i], groupIterator->indices[j]));
						}
					}
				}
			}
		}
	};

private:
	AxisAlignedRectangled box;

	struct EvaluationStackElement {
		uint dimension;
		double value;
		bool isPositive;
		uint depth;
	};

	struct IntersectionStackElement {
		typename Evaluation::Iterator it1;
		typename Evaluation::Iterator it2;
	};

	IntersectionStackElement stacks[n][1 << (maxDepth + 1)];
	uint skipStack[maxDepth];
	std::list<typename GroupingScheme::Grouping> recycling;

	template<typename = std::enable_if_t<n == 2>>
	using AxisAlignedHalfSpaceCollisionMask = AxisAlignedHalfSpace2CollisionMask;

	template<typename = std::enable_if_t<n == 2>>
	using InPlaceCollisionEvaluator = InPlaceCollisionEvaluator2;

public:
	BinaryCollisionTree(const AxisAlignedBox<double, n>& in_box) :
		box(in_box)
	{}

	template<class T>
	void Evaluate(T& in_mask, uint in_depth, Evaluation& out_evaluation) {
		using StackElement = EvaluationStackElement;
		if (in_depth > maxDepth) {
			throw InvalidArgumentException();
		}

		out_evaluation.parent = this;

		for (uint i = 0; i < n; i++) {
			if (out_evaluation.depth != in_depth) {
				if (out_evaluation.data[i] != nullptr) {
					delete[] out_evaluation.data[i];
				}
				out_evaluation.data[i] = new ubyte[(1 << (in_depth + 2))];
			}
			out_evaluation.dataSizes[i] = 0;
		}
		out_evaluation.depth = in_depth;

		T transformedMask = in_mask;
		transformedMask.Apply_Transform();
		transformedMask.Set_Ignore_Transform(true);

		InPlaceCollisionEvaluator<> evaluator;
		evaluator.Return_Point(false);

		StackElement* stacks[3];
		uint stackSizes[3];
		for (uint i = 0; i < n; i++) {
			stacks[i] = (StackElement*)this->stacks[i];
			stackSizes[i] = 1;
			stacks[i][0] = { i, box.Get_Center()[i], false, 0 };
		}

		AxisAlignedHalfSpace<double, n> halfSpace1 = AxisAlignedHalfSpace<double, n>::From_Dimension_Value(0, 0.0, false);
		AxisAlignedHalfSpace<double, n> halfSpace2 = AxisAlignedHalfSpace<double, n>::From_Dimension_Value(0, 0.0, false);
		AxisAlignedHalfSpaceCollisionMask<> mask1 = AxisAlignedHalfSpaceCollisionMask<>(halfSpace1, true);
		AxisAlignedHalfSpaceCollisionMask<> mask2 = AxisAlignedHalfSpaceCollisionMask<>(halfSpace2, true);

		for (bool keepGoing = true; keepGoing;) {
			keepGoing = false;
			for (uint i = 0; i < n; i++) {
				if (stackSizes[i] == 0) continue;
				StackElement& back = stacks[i][--stackSizes[i]];

				halfSpace1 = AxisAlignedHalfSpace<double, n>::From_Dimension_Value(
					back.dimension, back.value, back.isPositive);
				halfSpace2 = AxisAlignedHalfSpace<double, n>::From_Dimension_Value(
					back.dimension, back.value, !back.isPositive);

				mask1.Get_Half_Space() = halfSpace1;
				mask2.Get_Half_Space() = halfSpace2;

				bool didCollide1 = evaluator.Evaluate(transformedMask, mask1).didCollide;
				bool didCollide2 = evaluator.Evaluate(transformedMask, mask2).didCollide;

				out_evaluation.data[i][out_evaluation.dataSizes[i]++] = didCollide1;
				out_evaluation.data[i][out_evaluation.dataSizes[i]++] = didCollide2;

				double nextOffset = box.Get_Dimensions()[halfSpace1.Get_Dimension()] / (1 << (back.depth + 2));
				uint lastDepth = back.depth;

				if (lastDepth < in_depth) {
					if (didCollide1) {
						stacks[i][stackSizes[i]++] = {
							halfSpace1.Get_Dimension(),
							halfSpace1.Get_Value() + nextOffset,
							false,
							lastDepth + 1
						};
					}

					if (didCollide2) {
						stacks[i][stackSizes[i]++] = {
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

		uint stackSizes[n];
		bool didIntersect[n];
		ubyte* data[2][n];

		uint skipStackSize = 0;

		for (uint i = 0; i < n; i++) {
			stackSizes[i] = 1;
			stacks[i][0] = { in_evaluation1.Get_Iterator(), in_evaluation2.Get_Iterator() };
			didIntersect[i] = false;
			data[0][i] = &in_evaluation1.data[i][0];
			data[1][i] = &in_evaluation2.data[i][0];
		}
		
		for (bool keepGoing = true; keepGoing;) {
			keepGoing = false;
			for (uint i = 0; i < n; i++) {
				if (stackSizes[i] == 0) continue;

				StackElement element = stacks[i][--stackSizes[i]];

				if (element.it1.Get_Left(i) && element.it2.Get_Left(i)) {
					StackElement& newElement = stacks[i][stackSizes[i]++];
					newElement.it1 = element.it1;
					newElement.it2 = element.it2;
					if (!newElement.it1.Go_Left(i) || !newElement.it2.Go_Left(i)) {
						stackSizes[i] = 0;
						didIntersect[i] = true;
						continue;
					}
				}

				if (element.it1.Get_Right(i) && element.it2.Get_Right(i)) {
					StackElement& newElement = stacks[i][stackSizes[i]++];
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
		if (recycling.size() < 1) {
			recycling.resize(1);
		}
		GroupingScheme::Grouping& startingGrouping = recycling.front();
		startingGrouping.indices.resize(in_nElements);
		uint i = 0;
		for (auto it = startingGrouping.indices.begin(); it != startingGrouping.indices.end(); it++) {
			*it = i++;
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

		for(uint currentDepth = 0; currentDepth < in_evaluations[0]->depth; currentDepth++) {
			// for each dimension
			for (uint i = 0; i < n; i++) {
				uint nGroups = out_groupingScheme.groupings.size();
				auto groupIterator = out_groupingScheme.groupings.begin();

				// for each grouping
				for (uint j = 0; j < nGroups; j++) {
					while (recycling.size() < 2) {
						recycling.push_back(GroupingScheme::Grouping());
					}

					GroupingScheme::Grouping& parentGrouping = *groupIterator;
					GroupingScheme::Grouping& leftGrouping = recycling.front();
					GroupingScheme::Grouping& rightGrouping = recycling.back();
					leftGrouping.indices.clear();
					rightGrouping.indices.clear();
					leftGrouping.indices.reserve(in_nElements);
					rightGrouping.indices.reserve(in_nElements);

					// for each evaluation in that grouping 
					for (auto indexIterator = parentGrouping.indices.begin(); indexIterator != parentGrouping.indices.end(); indexIterator++) {
						auto evaluationIterator = in_evaluations[*indexIterator]->Get_Iterator();

						for (uint k = 0; k < currentDepth; k++) {
							if (!((parentGrouping.path[i][k]) ? evaluationIterator.Go_Right(i) : evaluationIterator.Go_Left(i))) {
								throw ProcessFailureException();
							}
						}

						if (evaluationIterator.Get_Left(i)) {
							leftGrouping.indices.push_back(*indexIterator);
						}

						if (evaluationIterator.Get_Right(i)) {
							rightGrouping.indices.push_back(*indexIterator);
						}
					}

					if (leftGrouping.indices.size() > 1) {
						for (uint k = 0; k < n; k++) {
							for (uint m = 0; m < maxDepth; m++) {
								leftGrouping.path[k][m] = parentGrouping.path[k][m];
							}
						}
						leftGrouping.path[i][currentDepth] = 0;

						out_groupingScheme.groupings.splice(out_groupingScheme.groupings.end(), recycling, recycling.begin());
					}

					if (rightGrouping.indices.size() > 1) {
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

	friend class Evaluation::Iterator;
};

using QuadTree = BinaryCollisionTree<2>;

#endif

