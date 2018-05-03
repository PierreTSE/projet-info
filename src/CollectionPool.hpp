#ifndef COLLECTION_POOL_HPP
#define COLLECTION_POOL_HPP

#include "Collection.hpp"
#include "CollectionIterator.hpp"
#include <vector>
#include <iterator>

template<typename T, bool is_const = false>
class PoolIterator;

template<typename T>
class CollectionPool : public Collection<T>
{
    public:
        CollectionPool() = default;
        CollectionPool(size_type n, const value_type& v = value_type{}) : pool_(n, v) {}
        CollectionPool(std::initializer_list<value_type> list) : pool_{list} {}
        
    
    private:
        std::vector<value_type> pool_;
};

template<typename T, bool is_const = false>
class PoolIterator : public IteratorBase
{
public: 
	PoolIterator(T* ptr) : ptr_{ptr} {}

	T& operator*() override //dereference operator
		return *ptr_;
	const T& operator*() const override //const dereference operator
		return *ptr_;
	T* operator->() override //arrow operator
		return ptr_;
	const T* operator->() const override //const arrow operator
		return ptr_;
	void operator++() override //pre-increment operator
		++ptr_;
	void operator--() override //pre-decrement operator
		--ptr_;
	IteratorBase* clone() const override
		return new PoolIterator(ptr_);
	bool equal(const IteratorBase& rhs) const override
		return ptr_ == dynamic_cast<const PoolIterator&>(rhs).ptr_;
private:
	T* ptr_;
};
#endif // !COLLECTION_POOL_HPP