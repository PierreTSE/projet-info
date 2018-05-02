#ifndef COLLECTION_ITERATOR_HPP
#define COLLECTION_ITERATOR_HPP

#include <type_traits>
#include <iterator>

template <typename T, bool is_const = false>
class IteratorBase
{
public:
	IteratorBase() = default;//default constructor
	virtual IteratorBase(const IteratorBase<T, false>&) = default;//copy constructor
	virtual explicit IteratorBase(IteratorBase<T, false>*) = default;
	virtual IteratorBase(IteratorBase<T, false>&&) noexcept = default;//move constructor
	virtual ~IteratorBase = default;//destructor

	virtual void operator=(const IteratorBase<T, false>&) = 0;//copy assignment
	virtual void operator=(IteratorBase<T, false>&&) = 0;//move assignment

	bool operator==(const IteratorBase<T, true>& rhs) const { return typeid(*this) == typeid(rhs) && equal(rhs); }//== comparator
	bool operator!=(const IteratorBase<T, true>& rhs) { return typeid(*this) != typeid(rhs) && !(equal(rhs)); }//!= comparator

	virtual T& operator*() = 0;//dereference operator
	virtual const T& operator*() const = 0;//const dereference operator
	virtual T* operator->() = 0;//arrow operator
	virtual const T* operator->() const = 0;//const arrow operator

	virtual void operator++() = 0;//pre-increment operator
	virtual void operator++(int) = 0;//post-increment operator

	virtual IteratorBase* clone() const = 0;
protected:
	virtual bool equal(const IteratorBase& rhs) const = 0;
};

template<typename T, bool is_const = false>
class CollectionIterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using const_pointer = const T*;
	using pointer = std::conditional_t<is_const, const_pointer, T*>;
	using const_reference = const T&;
	using reference = std::conditional_t<is_const, const_reference, T&>;
	using difference_type = std::ptrdiff_t;

	//Is default-constructible, copy-constructible, copy-assignable and destructible
	CollectionIterator() : itr_{ nullptr } {}
	CollectionIterator(const CollectionIterator<T, false>& itr) : itr_{ itr.itr_->clone() } {}
	CollectionIterator& operator=(const CollectionIterator<T, false>& rhs) { itr_ = rhs.itr_->clone(); return *this; }
	~CollectionIterator() { delete itr_; }

	//Move constructor, move assignment
	CollectionIterator(CollectionIterator<T, false>&& itr) noexcept : itr_{ itr.itr_ } { itr.itr_ = nullptr; }
	CollectionIterator& operator=(CollectionIterator<T, false>&& rhs) { itr_ = rhs.itr_; rhs.itr_ = nullptr; return *this; }

	//Can be compared for equivalence using the equality/inequality operators
	bool operator==(const CollectionIterator<T, true>& rhs) const { return itr_ == rhs.itr_; }
	bool operator!=(const CollectionIterator<T, true>& rhs) const { return itr_ != rhs.itr_; }

	//Can be dereferenced as an rvalue
	reference operator*() { return *itr_; }
	const_reference operator*() const {return *const_cast<const IteratorBase<T>*>(itr_); }
	pointer operator->() { return itr_->operator->(); }
	const_pointer operator->() const { return const_cast<const IteratorBase<T>*>(itr_)->operator->(); }

	//Can be incremented
	CollectionIterator& operator++() { ++(*itr); return *this; }
	CollectionIterator operator++(int) { CollectionIterator tempItr = *this; ++(*itr); return tempItr; }

	//Lvalues are swappable


public:
	explicit CollectionIterator(IteratorBase<T>* ptr) : itr_{ ptr } {}

	IteratorBase<T>* itr_;
};

#endif // !COLLECTION_ITERATOR_HPP
