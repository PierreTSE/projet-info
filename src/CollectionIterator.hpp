#ifndef COLLECTION_ITERATOR_HPP
#define COLLECTION_ITERATOR_HPP

#include <type_traits>
#include <iterator>
#include <memory>

template <typename T>
class IteratorBase
{
	public:
		IteratorBase() = default; //default constructor
		virtual ~IteratorBase() = default; //destructor
	
		bool operator==(const IteratorBase& rhs) const { return typeid(*this) == typeid(rhs) && equal(rhs); } //== comparator
	
		virtual T& operator*() = 0; //dereference operator
		virtual const T& operator*() const = 0; //const dereference operator
		virtual T* operator->() = 0; //arrow operator
		virtual const T* operator->() const = 0; //const arrow operator
	
		virtual void operator++() = 0; //pre-increment operator
		virtual void operator--() = 0; //pre-decrement operator
	
		virtual IteratorBase* clone() const = 0;
	protected:
		virtual bool equal(const IteratorBase& rhs) const = 0;
};

template<typename T>
class CollectionIterator
{	
	friend class IteratorBase<T>;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using const_pointer = const T*;
		using pointer = T* ;
		using const_reference = const T&;
		using reference = T&;
		using difference_type = std::ptrdiff_t;
		using size_type = size_t;
	
		CollectionIterator() = default; //default constructor, using unique_ptr default constructor
		CollectionIterator(const CollectionIterator& itr) : itr_{ itr.itr_->clone() } {} //copy constructor
		CollectionIterator(CollectionIterator&& itr) noexcept : itr_{ std::move(itr.itr_) } {} //move constructor
		//destructed via unique_ptr destructor

		explicit CollectionIterator(IteratorBase<T>* ptr) : itr_{ ptr } {} //actual constructor
	
		//Copy assigment
		CollectionIterator& operator=(const CollectionIterator& rhs) { itr_.reset(rhs.itr_->clone()); return *this; }
	
		//Move assignment
		CollectionIterator& operator=(CollectionIterator&& rhs) { itr_ = std::move(rhs.itr_); return *this; }
	
		//Can be compared for equivalence using the equality/inequality operators
		bool operator==(const CollectionIterator& rhs) const { return *itr_ == *rhs.itr_; }
		bool operator!=(const CollectionIterator& rhs) const { return !(*itr_ == *rhs.itr_); }
	
		//Can be dereferenced as an rvalue
		reference operator*() { return **itr_; }
		const_reference operator*() const {return **const_cast<const IteratorBase<T>*>(itr_); }
		pointer operator->() { return itr_->operator->(); }
		const_pointer operator->() const { return const_cast<const IteratorBase<T>*>(itr_); }
	
		//Can be incremented
		CollectionIterator& operator++() { ++(*itr_); return *this; }
		CollectionIterator operator++(int) { CollectionIterator tempItr = *this; ++(*itr_); return tempItr; }
	
		//Can be decremented
		CollectionIterator& operator--() { --(*itr_); return *this; }
		CollectionIterator operator--(int) { CollectionIterator tempItr = *this; --(*itr_); return tempItr; }
	
		//Lvalues are swappable
			//via use of std::swap with move constructor

	
	private:	
		std::unique_ptr<IteratorBase<T>> itr_;
};

#endif // !COLLECTION_ITERATOR_HPP
