#ifndef BINARY_COLLISION_TREE_H
#define BINARY_COLLISION_TREE_H

#include "collision_mask.h"
#include "axis_aligned_box.h"
#include "in_place_collision_evaluator2.h"

template<uint n>
class BinaryCollisionTree {
private:
	AxisAlignedRectangled box;

	template<typename = std::enable_if_t<n == 2>>
	using AxisAlignedHalfSpaceCollisionMask = AxisAlignedHalfSpace2CollisionMask;

	template<typename = std::enable_if_t<n == 2>>
	using InPlaceCollisionEvaluator = InPlaceCollisionEvaluator2;

public:
	class Evaluation {
		friend class BinaryCollisionTree<n>;
	private:
		BinaryCollisionTree<n>* parent;
		std::vector<ubyte> data;
		uint depth;

	public:
		bool Intersection(const Evaluation& in_other) {
			struct StackElement {
				uint depth;
				uint i1;
				uint i2;
			};
			std::vector<StackElement> stack;
			stack.reserve(Min(depth * depth, in_other.depth * in_other.depth));
			stack.push_back({ 0, 0, 0 });
			while (stack.size() > 0) {
				StackElement element = stack.back();
				stack.pop_back();
				if (data[element.i1 * 2 + 0] && data[element.i2 * 2 + 0]) {
					if (element.depth == depth || element.depth == in_other.depth) {
						return true;
					}
					stack.push_back({
						element.depth + 1u, 
						element.i1 + 1u, 
						element.i2 + 1u
						});
				}
				if (data[element.i1 * 2 + 1] && data[element.i2 * 2 + 1]) {
					if (element.depth == depth) {
						return true;
					}
					stack.push_back({ 
						element.depth + 1, 
						element.i1 + (data[element.i1 * 2 + 0]) ? 2u : 1u, 
						element.i2 + (data[element.i2 * 2 + 0]) ? 2u : 1u 
						});
				}
			}
			return false;
		}
	};

	BinaryCollisionTree(const AxisAlignedBox<double, n>& in_box) :
		box(in_box)
	{}

	void Evaluate(CollisionMask<n>& in_mask, uint in_depth, Evaluation& out_evaluation) {
		out_evaluation.parent = this;
		out_evaluation.depth = in_depth;
		out_evaluation.data.clear();

		InPlaceCollisionEvaluator<> evaluator;
		evaluator.Return_Point(false);

		struct StackElement {
			AxisAlignedHalfSpace<double, n> halfSpace;
			uint depth;
		};

		std::vector<StackElement> stack;
		for (uint i = 0; i < n; i++) {
			stack.push_back(StackElement({ AxisAlignedHalfSpace<double, n>::From_Dimension_Value(i, box.Get_Center()[i], true), 0 }));
		}

		while (stack.size() > 0) {
			AxisAlignedHalfSpace<double, n> halfSpace1 = stack.back().halfSpace;
			AxisAlignedHalfSpace<double, n> halfSpace2 = AxisAlignedHalfSpace<double, n>::From_Inverse(halfSpace1);
			AxisAlignedHalfSpaceCollisionMask<> mask1 = AxisAlignedHalfSpaceCollisionMask<>(halfSpace1, true);
			AxisAlignedHalfSpaceCollisionMask<> mask2 = AxisAlignedHalfSpaceCollisionMask<>(halfSpace2, true);
			bool didCollide1 = evaluator.Evaluate(in_mask, mask1).didCollide;
			bool didCollide2 = evaluator.Evaluate(in_mask, mask2).didCollide;
			out_evaluation.data.push_back(didCollide1);
			out_evaluation.data.push_back(didCollide2);

			double nextOffset = box.Get_Dimensions()[halfSpace1.Get_Dimension()] / (1 << (stack.back().depth + 1));
			uint lastDepth = stack.back().depth;

			stack.pop_back();

			if (lastDepth < in_depth) {
				if (didCollide1) {
					stack.push_back({ 
						AxisAlignedHalfSpace<double, n>::From_Dimension_Value(
							halfSpace1.Get_Dimension(), 
							halfSpace1.Get_Value() + nextOffset, 
							true), 
						lastDepth + 1
						});
				}

				if (didCollide2) {
					stack.push_back({ 
						AxisAlignedHalfSpace<double, n>::From_Dimension_Value(
							halfSpace2.Get_Dimension(), 
							halfSpace2.Get_Value() - nextOffset, 
							true), 
						lastDepth + 1});
				}
			}
		}
	}
};

using QuadTree = BinaryCollisionTree<2>;

#endif

