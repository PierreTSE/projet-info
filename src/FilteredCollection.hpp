#ifndef FILTERED_COLLECTION_HPP
#define FILTERED_COLLECTION_HPP

#include "Collection.hpp"
#include <vector>
#include <functional>

template<typename T>
class FilteredIterator;

template<typename T>
class FilteredCollection : public Collection<T>
{
	friend class FilteredIterator<T>;

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

	private:
		using filtre_t = std::function<bool(const_reference)>;

	public:
		FilteredCollection(Collection& c, const filtre_t& f) : collection_{ c }, filtre_{ f } {};

		iterator begin() {
			auto it = collection_.begin();
			while (!filtre_(*it))
				++it;
			return FilteredIterator(*this, it);
		}
		iterator end()
		{
			return FilteredIterator(*this, collection_.end());
		}

	private:
		Collection<T>& collection_;
		filtre_t filtre_;
};

template<typename T>
class FilteredIterator : public IteratorBase<T>
{
public:
	FilteredCollection(FilteredCollection<T>& f, CollectionIterator<T> itr) : filter_reference_{ f }, itr_{ itr } {}

	T & operator*() override //dereference operator
	{
		return *itr_;
	}
	const T& operator*() const override //const dereference operator
	{
		return *itr_;
	}
	T* operator->() override //arrow operator
	{
		return itr_;
	}
	const T* operator->() const override //const arrow operator
	{
		return itr_;
	}
	void operator++() override //pre-increment operator
	{
		if (itr_ != filter_reference_.container_.end())
		{
			do
			{
				++itr_;
			} while (!filter_reference_.filtre_(*itr_) and itr_ != filter_reference_.collection_.end());
		}
	}
	void operator--() override //pre-decrement operator
	{
		if (itr_ != --filter_reference_.container_.begin())
		{
			do
			{
				--itr_;
			} while (!filter_reference_.filtre_(*itr_) and itr_ != --filter_reference_.collection_.begin());
		}
	}
	IteratorBase<T>* clone() const override
	{
		return new FilteredIterator(filter_reference_, itr_);
	}
	bool equal(const IteratorBase<T>& rhs) const override
	{
		return itr_ == dynamic_cast<const FilteredIterator&>(rhs).itr_;
	}
private:
	FilteredCollection<T>& filter_reference_;
	CollectionIterator<T> itr_;
};


#endif // !FILTERED_COLLECTION_HPP

