#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include "CollectionIterator.hpp"
#include <iterator>

template <typename T>
class Collection
{
public:
	using value_type = T;
	using allocator_type = std::allocator<T>;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = CollectionIterator<T, false>;
	using const_iterator = CollectionIterator<T, true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using difference_type = std::ptrdiff_t;
	using size_type = size_t;


	virtual reference front() = 0;
	virtual const_reference front() const = 0;
	virtual reference back() = 0;
	virtual const_reference back() const = 0;

};

#endif // !COLLECTION_HPP
