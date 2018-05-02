#ifndef FILTERED_COLLECTION_HPP
#define FILTERED_COLLECTION_HPP

#include <vector>
#include <functional>

using Collection = std::vector<int>;

class FilteredCollection
{
	Collection& collec_;
	using filtre_t = std::function<bool(const Collection::value_type&)>;
	filtre_t filtre_;

public:
	FilteredCollection(Collection& c, const filtre_t& f) : collec_{ c }, filtre_{ f } {};

	Collection::value_type& front() {
		auto it = collec_.begin();
		while (!filtre_(*it))
			++it;
		return *it;
	}
};
#endif // !FILTERED_COLLECTION_HPP

