#include "filter_query.h"

FilterQuery::FilterQuery(Type in_type) :
	mType(in_type),
	mFilters(),
	mQuery1(nullptr),
	mQuery2(nullptr)
{}

FilterQuery::FilterQuery(Type in_type, std::vector<ubyte> in_filters) :
mType(in_type),
mFilters(in_filters),
mQuery1(nullptr),
mQuery2(nullptr)
{}

FilterQuery::FilterQuery(Type in_type, const FilterQuery* in_query1, const FilterQuery* in_query2) :
mType(in_type),
mFilters(),
mQuery1(in_query1),
mQuery2(in_query2)
{}

FilterQuery::FilterQuery(const FilterQuery& in_source) :
	mType(in_source.mType),
	mFilters(in_source.mFilters),
	mQuery1(in_source.mQuery1),
	mQuery2(in_source.mQuery2)
{}

FilterQuery::~FilterQuery()
{}

FilterQuery FilterQuery::And(const FilterQuery& in_query) const {
	return FilterQuery(combine_and, this, &in_query);
}

FilterQuery FilterQuery::Or(const FilterQuery& in_query) const {
	return FilterQuery(combine_or, this, &in_query);
}

FilterQuery FilterQuery::XOr(const FilterQuery& in_query) const {
	return FilterQuery(combine_xor, this, &in_query);
}

FilterQuery FilterQuery::Not() const {
	return FilterQuery(combine_xor, this, nullptr);
}

bool FilterQuery::Evaluate(const FilteredObject& in_object) const {
	switch (mType) {
	case any:
		return true;
	case fit_one:
		return in_object.mFilter & (1ull << mFilters[0]);
	case fit_one_of:
		for (uint i = 0; i < mFilters.size(); i++) {
			if (in_object.mFilter & (1ull << mFilters[i])) {
				return true;
			}
		}
		return false;
	case fit_all_of:
		for (uint i = 0; i < mFilters.size(); i++) {
			if (!(in_object.mFilter & (1ull << mFilters[i]))) {
				return false;
			}
		}
		return true;
	case combine_and:
		return mQuery1->Evaluate(in_object) && mQuery2->Evaluate(in_object);
	case combine_or:
		return mQuery1->Evaluate(in_object) || mQuery2->Evaluate(in_object);
	case combine_xor:
		return mQuery1->Evaluate(in_object) ^ mQuery2->Evaluate(in_object);
	case combine_not:
		return !mQuery1->Evaluate(in_object);
	}
	return false;
}

FilterQuery FilterQuery::Any() {
	return FilterQuery(any);
}

FilterQuery FilterQuery::Fits(ubyte in_filter) {
	std::vector<ubyte> filters(1);
	filters[0] = in_filter;
	return FilterQuery(fit_one, filters);
}