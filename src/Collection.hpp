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

	Collection() = default;
	virtual ~Collection() = default;

	//Assign
	virtual void assign(size_type count, const T& value) = 0;
	virtual void assign(iterator first, iterator last) = 0;
	virtual void assign(std::initializer_list<T> ilist) = 0;

	//Element access
	virtual reference at(size_type pos) = 0;
	virtual const_reference at(size_type pos) const = 0;
	virtual reference front() = 0;
	virtual const_reference front() const = 0;
	virtual reference back() = 0;
	virtual const_reference back() const = 0;

	//Iterators
	virtual iterator begin() = 0;
	virtual const_iterator cbegin() const = 0;
	virtual reverse_iterator rbegin() = 0;
	virtual const_reverse_iterator rbegin() const = 0;
	virtual const_reverse_iterator crbegin() const = 0;
	virtual iterator end() = 0;
	virtual const_iterator cend() const = 0;
	virtual reverse_iterator rend() = 0;
	virtual const_reverse_iterator rend() const = 0;
	virtual const_reverse_iterator crend() const = 0;

	//Capacity
	virtual bool empty() const = 0;
	virtual size_type size() const = 0;
	virtual size_type max_size() const = 0;
	virtual void reserve(size_type new_cap) = 0;
	virtual size_type capacity() const = 0;
	virtual void shrink_to_fit() = 0;

	//Modifiers
	virtual void clear() = 0;
	virtual iterator insert(iterator pos, const T& value) = 0;
	virtual iterator insert(const_iterator pos, const T& value) = 0;
	virtual iterator insert(const_iterator pos, T&& value) = 0;
	virtual void insert(iterator pos, size_type count, const T& value) = 0;
	virtual iterator insert(const_iterator pos, size_type count, const T& value) = 0;
	template <class InputIt>
	virtual void insert(iterator pos, InputIt first, InputIt last) = 0;
	template <class InputIt>
	virtual iterator insert(const_iterator pos, InputIt first, InputIt last) = 0;
	virtual iterator insert(const_iterator pos, std::initializer_list<T> ilist) = 0;
	template <class... Args>
	virtual iterator emplace(const_iterator pos, Args&&... args) = 0;
	virtual iterator erase(iterator pos) = 0;
	virtual iterator erase(const_iterator pos) = 0;
	virtual iterator erase(iterator first, iterator last) = 0;
	virtual iterator erase(const_iterator first, const_iterator last) = 0;
	virtual void push_back(const T& value) = 0;
	virtual void push_back(T&& value) = 0;
	template <class... Args>
	virtual void emplace_back(Args&&... args) = 0;
	template <class... Args>
	virtual reference emplace_back(Args&&... args) = 0;
	virtual void pop_back() = 0;
	virtual void resize(size_type count, T value = T()) = 0;
	virtual void resize(size_type count) = 0;
	virtual void resize(size_type count, const value_type& value) = 0;
};

#endif // !COLLECTION_HPP
