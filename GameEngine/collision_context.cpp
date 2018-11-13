#include "collision_context.h"
#include "game_engine.h"

template<class T, uint n>
CollisionContext<T, n>::CollisionContext(const AxisAlignedBox<T, n>& in_box, uint in_depth) :
	tree(in_box),
	depth(in_depth) 
{}

template<class T, uint n>
CollisionContext<T, n>::~CollisionContext() 
{}

template<class T, uint n>
void CollisionContext<T, n>::Add(CollisionMask<T, n>* in_mask) {
	masksToAdd.push_back(in_mask);
}

template<class T, uint n>
void CollisionContext<T, n>::Remove(CollisionMask<T, n>* in_mask) {
	for (auto it = evaluations.begin(); it < evaluations.end(); it++) {
		if (it->Get_Parent_Mask() == in_mask) {
			evaluations.erase(it);
			it--;
		}
	}
}

template<class T, uint n>
void CollisionContext<T, n>::Update() {
	using Evaluation = BinaryCollisionTree<T, n>::Evaluation;
	std::vector<Evaluation*> filteredEvaluations1;
	std::vector<Evaluation*> filteredEvaluations2;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	Collision<T, n> collision;

	Prepare_Data_Containers();
	Update_Evaluations();

	for (auto it = partnersToTest.begin(); it != partnersToTest.end(); it++) {
		filteredEvaluations1.clear();
		filteredEvaluations2.clear();

		ubyte filter1 = it->first;
		ubyte filter2 = it->second;

		if (it->first == it->second) {
			for (auto jt = evaluations.begin(); jt < evaluations.end(); jt++) {
				if (jt->Get_Parent_Mask()->Has_Filter(filter1)) {
					filteredEvaluations1.push_back(&(*jt));
				}
			}

			Partner_Filtered(filteredEvaluations1);
		}
		else {
			for (auto jt = evaluations.begin(); jt < evaluations.end(); jt++) {
				if (jt->Get_Parent_Mask()->Has_Filter(filter1)) {
					filteredEvaluations1.push_back(&(*jt));
				}
			}

			for (auto jt = evaluations.begin(); jt < evaluations.end(); jt++) {
				if (jt->Get_Parent_Mask()->Has_Filter(filter2)) {
					filteredEvaluations2.push_back(&(*jt));
				}
			}

			Partner_Filtered(filteredEvaluations1, filteredEvaluations2);
		}
	}

	GE.Async().Help_Until_Empty();

	for (auto it = tasks.begin(); it < tasks.end(); it++) {
		(*it)->Wait_For_Finish();
		delete *it;
	}
}

/*
template<class T, uint n>
void CollisionContext<T, n>::Update(CollisionMask<T, n>* in_masks, uint in_nMasks) {}
*/
template<class T, uint n>
uint CollisionContext<T, n>::Get_Total_Partnerings() const {
	return (uint)partnering.size();
}

template<class T, uint n>
std::pair<typename CollisionContext<T, n>::CollisionPartner*, typename CollisionContext<T, n>::CollisionPartner*> CollisionContext<T, n>::Get_Partners(CollisionMask<T, n>* in_mask) {
	auto search = partnering.equal_range(in_mask);
	if (search.first == search.second) {
		return std::pair<CollisionPartner*, CollisionPartner*>(nullptr, nullptr);
	}
	else {
		return std::pair<CollisionPartner*, CollisionPartner*>(&search.first->second, &search.second->second);
	}
}

template<class T, uint n>
void CollisionContext<T, n>::Set_Partner_Test_Activation(std::pair<ubyte, ubyte> in_test, bool in_value) {
	if (in_value) {
		partnersToTest.insert(in_test);
	}
	else {
		partnersToTest.erase(in_test);
	}
}

template<class T, uint n>
bool CollisionContext<T, n>::Get_Partner_Test_Activation(std::pair<ubyte, ubyte> in_test) {
	return partnersToTest.find(in_test) != partnersToTest.end();
}

template<class T, uint n>
void CollisionContext<T, n>::Update_Evaluations() {
	std::vector<PackagedAsyncTask<void>> tasks;
	uint nEvaluations = (uint)evaluations.size();
	uint batchSize = (uint)evaluations.size() / 4 + 1;

	tasks.reserve(nEvaluations / batchSize + 2);

	for (uint i = 0; i < nEvaluations; i += batchSize) {
		auto todo = [this, i, batchSize, nEvaluations]() {
			for (uint j = i; j < i + batchSize && j < nEvaluations; j++) {
				tree.Evaluate(evaluations[j].Get_Parent_Mask(), depth, evaluations[j]);
			}
		};
		tasks.emplace_back(todo);
		GE.Async().Add(&tasks.back());
	}

	GE.Async().Help_Until_Empty();

	for (auto it = tasks.begin(); it < tasks.end(); it++) {
		it->Wait_For_Finish();
	}
	/*
	for (uint i = 0; i < evaluations.size(); i++) {
		tree.Evaluate(evaluations[i].Get_Parent_Mask(), depth, evaluations[i]);
	}
	*/
	for (auto it = masksToAdd.begin(); it < masksToAdd.end(); it++) {
		evaluations.push_back(BinaryCollisionTree<T, n>::Evaluation());
		tree.Evaluate(*it, depth, evaluations.back());
	}
	masksToAdd.clear();
}

template<class T, uint n>
void CollisionContext<T, n>::Prepare_Data_Containers() {
	partnering.clear();
	tasks.clear();

	groupingSchemes.clear();
	groupingSchemes.reserve(partnersToTest.size());

	pairedGroupingSchemes.clear();
	pairedGroupingSchemes.reserve(partnersToTest.size());
}

template<class T, uint n>
void CollisionContext<T, n>::Partner_Filtered(std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations) {
	if (in_filteredEvaluations.size() > 1) {
		groupingSchemes.push_back(BinaryCollisionTree<T, n>::GroupingScheme());

		tree.Group(
			in_filteredEvaluations.data(),
			(uint)in_filteredEvaluations.size(),
			groupingSchemes.back()
		);

		uint batchSize = 10;
		for (auto kt = groupingSchemes.back().Get_Iterator(); !kt.Is_Done(); kt += batchSize) {
			auto todo = [this, kt, batchSize]() {
				auto itCopy = kt;
				for (uint i = 0; i < batchSize && !itCopy.Is_Done(); i++) {
					Partner_If_Collide_Async(
						itCopy.Get_First()->Get_Transformed_Parent_Mask(),
						itCopy.Get_Second()->Get_Transformed_Parent_Mask(),
						itCopy.Get_First()->Get_Parent_Mask(),
						itCopy.Get_Second()->Get_Parent_Mask(),
						partnering,
						partneringMutex
					);
					itCopy++;
				}
			};

			PackagedAsyncTask<void>* task = new PackagedAsyncTask<void>(todo);
			tasks.push_back(task);
			GE.Async().Add(task);
		}
	}
}

template<class T, uint n>
void CollisionContext<T, n>::Partner_Filtered(std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations1, std::vector<typename BinaryCollisionTree<T, n>::Evaluation*>& in_filteredEvaluations2) {
	if (in_filteredEvaluations1.size() > 1 && in_filteredEvaluations2.size() > 1) {
		pairedGroupingSchemes.push_back(BinaryCollisionTree<T, n>::PairedGroupingScheme());

		tree.Group(
			in_filteredEvaluations1.data(), (uint)in_filteredEvaluations1.size(),
			in_filteredEvaluations2.data(), (uint)in_filteredEvaluations2.size(),
			pairedGroupingSchemes.back()
		);

		uint batchSize = 10;
		for (auto kt = groupingSchemes.back().Get_Iterator(); !kt.Is_Done(); kt += batchSize) {
			auto todo = [this, kt, batchSize]() {
				auto itCopy = kt;
				for (uint i = 0; i < batchSize && !itCopy.Is_Done(); i++) {
					Partner_If_Collide_Async(
						itCopy.Get_First()->Get_Transformed_Parent_Mask(),
						itCopy.Get_Second()->Get_Transformed_Parent_Mask(),
						itCopy.Get_First()->Get_Parent_Mask(),
						itCopy.Get_Second()->Get_Parent_Mask(),
						partnering,
						partneringMutex
					);
					itCopy++;
				}
			};

			PackagedAsyncTask<void>* task = new PackagedAsyncTask<void>(todo);
			tasks.push_back(task);
			GE.Async().Add(task);
		}
	}
}

template<class T, uint n>
void CollisionContext<T, n>::Partner_If_Collide(CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2, Partnering& in_partnering) {
	Collision<T, n> collision;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;
	auto search = in_partnering.equal_range(in_mask1);

	bool found = false;
	for (auto it = search.first; it != search.second; it++) {
		if (it->second.mask == in_mask2) {
			found = true;
			break;
		}
	}

	if (!found && (collision = collisionEvaluator.Evaluate(*in_mask1, *in_mask2)).didCollide) {
		in_partnering.insert(Partnering::value_type(in_mask1, { in_mask2, collision }));
		in_partnering.insert(Partnering::value_type(in_mask2, { in_mask1, collision }));
	}
}

template<class T, uint n>
void CollisionContext<T, n>::Partner_If_Collide_Async(
	CollisionMask<T, n>* in_mask1, CollisionMask<T, n>* in_mask2, 
	CollisionMask<T, n>* in_maskToPartner1, CollisionMask<T, n>* in_maskToPartner2,
	Partnering& in_partnering, std::mutex& in_mutex) {

	Collision<T, n> collision;
	InPlaceCollisionEvaluator<T, n> collisionEvaluator;

	if ((collision = collisionEvaluator.Evaluate(*in_mask1, *in_mask2)).didCollide) {
		std::lock_guard<std::mutex> lock(in_mutex);

		auto range = in_partnering.equal_range(in_mask1);
		for (auto it = range.first; it != range.second; it++) {
			if (it->second.mask == in_mask2) {
				return;
			}
		}

		in_partnering.insert(Partnering::value_type(in_maskToPartner1, { in_maskToPartner2, collision }));
		in_partnering.insert(Partnering::value_type(in_maskToPartner2, { in_maskToPartner1, collision }));
	}
}


template class CollisionContext<float, 2>;
template class CollisionContext<double, 2>;