#ifndef COLLECTION_POOL_HPP
#define COLLECTION_POOL_HPP

#include "Collection.hpp"
#include "CollectionIterator.hpp"
#include <vector>
#include <iterator>

template<typename T>
class PoolIterator;

template<typename T>
class CollectionPool : public Collection<T>
{
    public:
		using value_type = typename Collection<T>::value_type;
		using allocator_type = typename Collection<T>::allocator_type;
		using reference = typename Collection<T>::reference;
		using const_reference = typename Collection<T>::const_reference;
		using pointer = typename Collection<T>::pointer;
		using const_pointer = typename Collection<T>::const_pointer;
		using iterator = typename Collection<T>::iterator;
		//using const_iterator = typename Collection<T>::const_iterator;
		using reverse_iterator = typename Collection<T>::reverse_iterator;
		//using const_reverse_iterator = typename Collection<T>::const_reverse_iterator;
		using difference_type = typename Collection<T>::difference_type;
		using size_type = typename Collection<T>::size_type;
		
        CollectionPool() = default;
        explicit CollectionPool(size_type n, const value_type& v = value_type{}) : pool_(n, v) {}
        CollectionPool(std::initializer_list<value_type> list) : pool_{list} {}

        //Capacity
        bool empty() const { return pool_.empty(); }
        size_type size() const { return pool_.size(); }
        size_type max_size() const { return pool_.max_size(); }
        void reserve(size_type new_cap) { pool_.reserve(new_cap); }
        size_type capacity() const { return pool_.capacity(); }
        void shrink_to_fit() { pool_.shrink_to_fit(); }
    /*
        //Modifiers
        void clear() { pool_.clear(); }
        iterator insert(const_iterator pos, const T& value) { return pool_.insert(pos, value); }
        iterator insert(const_iterator pos, T&& value);
        void insert(iterator pos, size_type count, const T& value);
        iterator insert(const_iterator pos, size_type count, const T& value);
        template <class InputIt>
        void insert(iterator pos, InputIt first, InputIt last);
        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last);
        iterator insert(const_iterator pos, std::initializer_list<T> ilist);
        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args);
        iterator erase(iterator pos);
        iterator erase(const_iterator pos);
        iterator erase(iterator first, iterator last);
        iterator erase(const_iterator first, const_iterator last);
        void push_back(const T& value);
        void push_back(T&& value);
        template <class... Args>
        void emplace_back(Args&&... args);
        template <class... Args>
        reference emplace_back(Args&&... args);
        void pop_back();
        void resize(size_type count, T value = T());
        void resize(size_type count);
        void resize(size_type count, const value_type& value);
        */

		iterator begin() override
		{
			return iterator(new PoolIterator<value_type>(&pool_.front()));
		}

		iterator end() override
		{
			return iterator(new PoolIterator<value_type>(&pool_.back()+1));
		}
    
    private:
        std::vector<value_type> pool_;
};

template<typename T>
class PoolIterator : public IteratorBase<T>
{
public: 
	PoolIterator(T* ptr) : ptr_{ptr} {}

	T& operator*() override //dereference operator
	{
		return *ptr_;
	}
	const T& operator*() const override //const dereference operator
	{
		return *ptr_;
	}
	T* operator->() override //arrow operator
	{
		return ptr_;
	}
	const T* operator->() const override //const arrow operator
	{
		return ptr_;
	}
	void operator++() override //pre-increment operator
	{
		++ptr_;
	}
	void operator--() override //pre-decrement operator
	{
		--ptr_;
	}
	IteratorBase<T>* clone() const override
	{
		return new PoolIterator(ptr_);
	}
	bool equal(const IteratorBase<T>& rhs) const override
	{
		return ptr_ == dynamic_cast<const PoolIterator&>(rhs).ptr_;
	}
private:
	T* ptr_;
};
#endif // !COLLECTION_POOL_HPP