#ifndef BINARY_COLLISION_TREE_H
#define BINARY_COLLISION_TREE_H

#include <list>
#include <algorithm>
#include <mutex>
#include <stack>
#include "collision_mask.h"
#include "axis_aligned_box.h"
#include "in_place_collision_evaluator.h"
#include "lazy_provider.h"

template<class T, uint n>
class BinaryCollisionTree {
public:
	const static int maxDepth = 12;

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

			bool goLeft(uint in_dimension);
			bool goRight(uint in_dimension);
			ubyte getLeft(uint in_dimension);
			ubyte getRight(uint in_dimension);
			uint getDepth(uint in_dimension);
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
		Evaluation(const Evaluation& in_other);
		Evaluation(Evaluation&& in_other) noexcept;
		~Evaluation();

		Evaluation& operator=(const Evaluation& in_other);

		Iterator getIterator();
		BinaryCollisionTree<T, n>* getParentTree();
		CollisionMask<T, n>* getParentMask();
		CollisionMask<T, n>* getTransformedParentMask();
		void transformParentMask();
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
			Evaluation* getFirst();
			Evaluation* getSecond();
			void operator++(int a);
			void operator+=(uint n);
			bool isDone();
		};
		friend class GroupingScheme::Iterator;

	private:
		BinaryCollisionTree<T, n>* mParent;
		std::list<Grouping> mGroupings;

	public:
		uint getNumberGroups() const;
		Iterator getIterator();
		void remove(const Evaluation* in_eval);
		void getUniquePairs(std::vector<std::pair<Evaluation*, Evaluation*>>& in_pairs) const;
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
			Evaluation* getFirst();
			Evaluation* getSecond();
			void operator++(int a);
			void operator+=(uint n);
			bool isDone();
		};
		friend class PairedGroupingScheme::Iterator;

	private:
		BinaryCollisionTree<T, n>* mParent;
		std::list<Grouping> mGroupings;

	public:
		uint getNumberGroups() const;
		Iterator getIterator();
		void getUniquePairs(std::vector<std::pair<Evaluation*, Evaluation*>>& in_pairs) const;
	};

private:
	template<typename = std::enable_if_t<n == 2>>
	using AAHalfSpaceCollisionMask = AAHalfSpace2CollisionMask<T>;
	using InPlaceCollisionEvaluator = InPlaceCollisionEvaluator<T, n>;

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

	// member variables

	AxisAlignedBox<T, n> mBox;



	std::list<typename GroupingScheme::Grouping> mRecycling;
	std::list<typename PairedGroupingScheme::Grouping> mPairRecycling;

public:
	BinaryCollisionTree(const AxisAlignedBox<T, n>& in_box);

	void evaluate(CollisionMask<T, n>* in_mask, uint in_depth, Evaluation& out_evaluation);
	bool intersection(Evaluation& in_evaluation1, Evaluation& in_evaluation2);
	void group(
		Evaluation** in_evaluations, uint in_nElements, 
		GroupingScheme& out_groupingScheme
	);
	void group(
		Evaluation** in_evaluations1, uint in_nElements1, 
		Evaluation** in_evaluations2, uint in_nElements2, 
		PairedGroupingScheme& out_groupingScheme
	);

	friend class Evaluation::Iterator;
};

using QuadTreef = BinaryCollisionTree<float, 2>;
using QuadTreed = BinaryCollisionTree<double, 2>;
using OctTreef = BinaryCollisionTree<float, 3>;
using OctTreed = BinaryCollisionTree<double, 3>;

#endif


