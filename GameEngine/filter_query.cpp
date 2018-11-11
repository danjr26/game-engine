#include "filter_query.h"

FilterQuery::FilterQuery(Type in_type) :
	type(in_type),
	filters(),
	query1(nullptr),
	query2(nullptr)
{}

FilterQuery::FilterQuery(Type in_type, std::vector<ubyte> in_filters) :
type(in_type),
filters(in_filters),
query1(nullptr),
query2(nullptr)
{}

FilterQuery::FilterQuery(Type in_type, const FilterQuery* in_query1, const FilterQuery* in_query2) :
type(in_type),
filters(),
query1(in_query1),
query2(in_query2)
{}

FilterQuery::FilterQuery(const FilterQuery& in_source) :
	type(in_source.type),
	filters(in_source.filters),
	query1(in_source.query1),
	query2(in_source.query2)
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
	switch (type) {
	case any:
		return true;
	case fit_one:
		return in_object.filter & (1ull << filters[0]);
	case fit_one_of:
		for (uint i = 0; i < filters.size(); i++) {
			if (in_object.filter & (1ull << filters[i])) {
				return true;
			}
		}
		return false;
	case fit_all_of:
		for (uint i = 0; i < filters.size(); i++) {
			if (!(in_object.filter & (1ull << filters[i]))) {
				return false;
			}
		}
		return true;
	case combine_and:
		return query1->Evaluate(in_object) && query2->Evaluate(in_object);
	case combine_or:
		return query1->Evaluate(in_object) || query2->Evaluate(in_object);
	case combine_xor:
		return query1->Evaluate(in_object) ^ query2->Evaluate(in_object);
	case combine_not:
		return !query1->Evaluate(in_object);
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