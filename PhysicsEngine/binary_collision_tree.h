#ifndef BINARY_COLLISION_TREE_H
#define BINARY_COLLISION_TREE_H

#include <list>
#include <algorithm>
#include <mutex>
#include <stack>
#include "collider.h"
#include "axis_aligned_box.h"
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
			Iterator(Evaluation* i_parent);

		public:
			Iterator();

			bool goLeft(uint i_dimension);
			bool goRight(uint i_dimension);
			ubyte getLeft(uint i_dimension);
			ubyte getRight(uint i_dimension);
			uint getDepth(uint i_dimension);
		};

	private:
		BinaryCollisionTree<T, n>* mParentTree;
		Collider<T, n>* mParentMask;
		Collider<T, n>* mTransformedParentMask;
		ubyte* mData[n];
		uint mDataSizes[n];
		uint mDepth;

	public:
		Evaluation();
		Evaluation(const Evaluation& i_other);
		Evaluation(Evaluation&& i_other) noexcept;
		~Evaluation();

		Evaluation& operator=(const Evaluation& i_other);

		Iterator getIterator();
		BinaryCollisionTree<T, n>* getParentTree();
		Collider<T, n>* getParentMask();
		Collider<T, n>* getTransformedParentMask();
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
			Iterator(GroupingScheme* i_parent);

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
		void remove(const Evaluation* i_eval);
		void getUniquePairs(std::vector<std::pair<Evaluation*, Evaluation*>>& i_pairs) const;
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
			Iterator(PairedGroupingScheme* i_parent);

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
		void getUniquePairs(std::vector<std::pair<Evaluation*, Evaluation*>>& i_pairs) const;
	};

private:
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

	AABox<T, n> mBox;

	std::list<typename GroupingScheme::Grouping> mRecycling;
	std::list<typename PairedGroupingScheme::Grouping> mPairRecycling;

public:
	BinaryCollisionTree(const AABox<T, n>& i_box);

	void evaluate(Collider<T, n>* i_mask, uint i_depth, Evaluation& o_evaluation);
	bool intersect(Evaluation& i_evaluation1, Evaluation& i_evaluation2);
	void group(
		Evaluation** i_evaluations, uint i_nElements, 
		GroupingScheme& o_groupingScheme
	);
	void group(
		Evaluation** i_evaluations1, uint i_nElements1, 
		Evaluation** i_evaluations2, uint i_nElements2, 
		PairedGroupingScheme& o_groupingScheme
	);

	friend class Evaluation::Iterator;
};

using QuadTreef = BinaryCollisionTree<float, 2>;
using QuadTreed = BinaryCollisionTree<double, 2>;
using OctTreef = BinaryCollisionTree<float, 3>;
using OctTreed = BinaryCollisionTree<double, 3>;

#endif


