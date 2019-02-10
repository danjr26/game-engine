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
	FilterQuery and(const FilterQuery& in_query) const;
	FilterQuery or(const FilterQuery& in_query) const;
	FilterQuery xor(const FilterQuery& in_query) const;
	FilterQuery not() const;

	bool evaluate(const FilteredObject& in_object) const;

	static FilterQuery everything();

	static FilterQuery fits(ubyte in_filter);

	template<typename... Args>
	static FilterQuery fitsOneOf(Args... in_filters) {
		uint n = sizeof...(Args);
		ubyte filters[n] = { in_filters... };
		return FilterQuery(fit_one_of, std::vector<ubyte>(filters, filters + n));
	}

	template<typename... Args>
	static FilterQuery fitsAllOf(Args... in_filters) {
		uint n = sizeof...(Args);
		ubyte filters[n] = { in_filters... };
		return FilterQuery(fit_all_of, std::vector<ubyte>(filters, filters + n));
	}
};

#endif