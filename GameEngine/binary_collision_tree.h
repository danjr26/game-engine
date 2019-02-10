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
			Evaluation* mParent;
			uint mDepths[n];
			uint mEvals[n];

		private:
			Iterator(Evaluation* in_parent);

		public:
			Iterator();

			bool Go_Left(uint in_dimension);
			bool Go_Right(uint in_dimension);
			ubyte Get_Left(uint in_dimension);
			ubyte Get_Right(uint in_dimension);
			uint Get_Depth(uint in_dimension);
		};

	private:
		BinaryCollisionTree<T, n>* mParentTree;
		CollisionMask<T, n>* mParentMask;
		CollisionMask<T, n>* mTransformedParentMask;
		ubyte* mData[n];
		uint mDataSizes[n];
		uint mDepth;

	public:
		Evaluation();

		Iterator Get_Iterator();
		BinaryCollisionTree<T, n>* Get_Parent_Tree();
		CollisionMask<T, n>* Get_Parent_Mask();
		CollisionMask<T, n>* Get_Transformed_Parent_Mask();
		void Transform_Parent_Mask();
	};

	class GroupingScheme {
		friend class BinaryCollisionTree<T, n>;
	private:
		struct Grouping {
			ubyte mPath[n][maxDepth];
			std::vector<Evaluation*> mEvals;
		};

	public:
		class Iterator {
			friend class GroupingScheme;
		private:
			GroupingScheme* mParent;
			typename std::list<Grouping>::iterator mIt;
			uint mI, mJ;

		private:
			Iterator(GroupingScheme* in_parent);

		public:
			Evaluation* Get_First();
			Evaluation* Get_Second();
			void operator++(int a);
			void operator+=(uint n);
			bool Is_Done();
		};
		friend class GroupingScheme::Iterator;

	private:
		BinaryCollisionTree<T, n>* mParent;
		std::list<Grouping> mGroupings;

	public:
		uint Get_Number_Groups() const;
		Iterator Get_Iterator();
		void Remove(const Evaluation* in_eval);
		void Get_Unique_Pairs(std::vector<std::pair<Evaluation*, Evaluation*>>& in_pairs) const;
	};

	class PairedGroupingScheme {
		friend class BinaryCollisionTree<T, n>;
	private:
		struct Grouping {
			ubyte mPath[n][maxDepth];
			std::vector<Evaluation*> mEvals1;
			std::vector<Evaluation*> mEvals2;
		};

	public:
		class Iterator {
			friend class PairedGroupingScheme;
		private:
			PairedGroupingScheme* mParent;
			typename std::list<Grouping>::iterator mIt;
			uint mI, mJ;

		private:
			Iterator(PairedGroupingScheme* in_parent);

		public:
			Evaluation* Get_First();
			Evaluation* Get_Second();
			void operator++(int a);
			void operator+=(uint n);
			bool Is_Done();
		};
		friend class PairedGroupingScheme::Iterator;

	private:
		BinaryCollisionTree<T, n>* mParent;
		std::list<Grouping> mGroupings;

	public:
		uint Get_Number_Groups() const;
		Iterator Get_Iterator();
		void Get_Unique_Pairs(std::vector<std::pair<Evaluation*, Evaluation*>>& in_pairs) const;
	};

private:
	AxisAlignedBox<T, n> mBox;

	struct EvaluationStackElement {
		uint mDimension;
		T mValue;
		bool mIsPositive;
		uint mDepth;
	};

	struct IntersectionStackElement {
		typename Evaluation::Iterator mIt1;
		typename Evaluation::Iterator mIt2;
	};

	std::list<typename GroupingScheme::Grouping> mRecycling;
	std::list<typename PairedGroupingScheme::Grouping> mPairRecycling;

	template<typename = std::enable_if_t<n == 2>>
	using AxisAlignedHalfSpaceCollisionMask = AAHalfSpace2CollisionMask<T>;
	using InPlaceCollisionEvaluator = InPlaceCollisionEvaluator<T, n>;

public:
	BinaryCollisionTree(const AxisAlignedBox<T, n>& in_box);

	void Evaluate(CollisionMask<T, n>* in_mask, uint in_depth, Evaluation& out_evaluation);
	bool Intersection(Evaluation& in_evaluation1, Evaluation& in_evaluation2);
	void Group(Evaluation** in_evaluations, uint in_nElements, GroupingScheme& out_groupingScheme);
	void Group(Evaluation** in_evaluations1, uint in_nElements1, Evaluation** in_evaluations2, uint in_nElements2, PairedGroupingScheme& out_groupingScheme);

	friend class Evaluation::Iterator;
};

using QuadTreef = BinaryCollisionTree<float, 2>;
using QuadTreed = BinaryCollisionTree<double, 2>;
using OctTreef = BinaryCollisionTree<float, 3>;
using OctTreed = BinaryCollisionTree<double, 3>;

#endif


