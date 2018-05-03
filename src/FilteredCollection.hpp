#ifndef FILTERED_COLLECTION_HPP
#define FILTERED_COLLECTION_HPP

#include "Collection.hpp"
#include <vector>
#include <functional>

template<typename T>
class FilteredCollection : public Collection<T>
{
public:

	using value_type = typename Collection<T>::value_type;
	using allocator_type = typename Collection<T>::allocator_type;
	using reference = typename Collection<T>::reference;
	using const_reference = typename Collection<T>::const_reference;
	using pointer = typename Collection<T>::pointer;
	using const_pointer = typename Collection<T>::const_pointer;
	using iterator = typename Collection<T>::iterator;
	using const_iterator = typename Collection<T>::const_iterator;
	using reverse_iterator = typename Collection<T>::reverse_iterator;
	using const_reverse_iterator = typename Collection<T>::const_reverse_iterator;
	using difference_type = typename Collection<T>::difference_type;
	using size_type = typename Collection<T>::size_type;

private:
	using filtre_t = std::function<bool(const_reference)>;

public:
	FilteredCollection(Collection& c, const filtre_t& f) : collection_{ c }, filtre_{ f } {};

	reference front() {
		auto it = collec_.begin();
		while (!filtre_(*it))
			++it;
		return *it;
	}

private:
	Collection<T>& collection_;
	filtre_t filtre_;
};
#endif // !FILTERED_COLLECTION_HPP

