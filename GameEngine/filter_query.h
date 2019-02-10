#ifndef FILTER_QUERY_H
#define FILTER_QUERY_H

#include "filtered_object.h"

class FilterQuery {
private:
	enum Type : ubyte {
		any,
		fit_one,
		fit_one_of,
		fit_all_of,
		combine_and,
		combine_or,
		combine_xor,
		combine_not
	} mType;

	std::vector<ubyte> mFilters;
	const FilterQuery* mQuery1;
	const FilterQuery* mQuery2;

	FilterQuery(Type in_type);
	FilterQuery(Type in_type, std::vector<ubyte> in_filters);
	FilterQuery(Type in_type, const FilterQuery* in_query1, const FilterQuery* in_query2);

public:
	FilterQuery(const FilterQuery& in_source);
	~FilterQuery();
	FilterQuery And(const FilterQuery& in_query) const;
	FilterQuery Or(const FilterQuery& in_query) const;
	FilterQuery XOr(const FilterQuery& in_query) const;
	FilterQuery Not() const;

	bool Evaluate(const FilteredObject& in_object) const;

	static FilterQuery Any();

	static FilterQuery Fits(ubyte in_filter);

	template<typename... Args>
	static FilterQuery Fits_One_Of(Args... in_filters) {
		uint n = sizeof...(Args);
		ubyte filters[n] = { in_filters... };
		return FilterQuery(fit_one_of, std::vector<ubyte>(filters, filters + n));
	}

	template<typename... Args>
	static FilterQuery Fits_All_Of(Args... in_filters) {
		uint n = sizeof...(Args);
		ubyte filters[n] = { in_filters... };
		return FilterQuery(fit_all_of, std::vector<ubyte>(filters, filters + n));
	}
};

#endif