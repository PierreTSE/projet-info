#ifndef COLLECTION_ITERATOR_HPP
#define COLLECTION_ITERATOR_HPP

#include <type_traits>
#include <iterator>
#include <memory>


/**
 * @class IteratorBase
 * @brief Classe abstraite qui gère un itérateur personnalisé
 * @tparam T Type de l'objet référencé par l'itérateur
 * 
 * Cette classe est la véritable classe mère dont les itérateurs personnalisés héritent.
 * Elle est implémentée comme une classe abstraite quelconque.
 * Avec la classe @class CollectionIterator, elle permet d'utiliser un itérateur abstrait,
 * en utilisant la sémantique des itérateurs standards.
 */
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
	
		/** TODO doc
		*/
		virtual IteratorBase* clone() const = 0;
	protected:
		virtual bool equal(const IteratorBase& rhs) const = 0;
};

/**
 * @class CollectionIterator
 * @brief Wrapper autour d'un @c IteratorBase permettant le polymorphisme
 * @tparam T Type de l'objet référencé par l'itérateur
 *
 * Cette classe contient un pointeur vers un @c IteratorBase (qui sera hérité) afin de 
 * permettre un comportement polymorphique à cet itérateur. 
 * L'itérateur utilise la sémantique d'un itérateur standard 
 * (principalement l'utilisation fréquente de copie de l'itérateur, explicitement ou en passage par valeur,
 * ce qui est incompatible avec le polymorphisme usuel qui utilise des références ou des pointeurs).
 * Cette classe est implémentée comme un BidirectionalIterator et redéfinit donc tous les alias et méthodes
 * qui la rendent conforme à ce concept.
 */
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
		CollectionIterator& operator=(CollectionIterator&& rhs) noexcept
		{ itr_ = std::move(rhs.itr_); return *this; }
	
		//Can be compared for equivalence using the equality/inequality operators
		bool operator==(const CollectionIterator& rhs) const { return *itr_ == *rhs.itr_; }
		bool operator!=(const CollectionIterator& rhs) const { return !(*itr_ == *rhs.itr_); }
	
		//Can be dereferenced as an rvalue
		reference operator*() { return **itr_; }
		const_reference operator*() const {return **const_cast<const IteratorBase<T>*>(itr_.get()); }
		pointer operator->() { return itr_->operator->(); }
		const_pointer operator->() const { return const_cast<const IteratorBase<T>*>(itr_.get())->operator->(); }
	
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

/** TODO doc
 *
 */
template <typename T>
class ConstIteratorBase
{
public:
	ConstIteratorBase() = default; //default constructor
	virtual ~ConstIteratorBase() = default; //destructor

	bool operator==(const ConstIteratorBase& rhs) const { return typeid(*this) == typeid(rhs) && equal(rhs); } //== comparator

	virtual const T& operator*() const = 0; //const dereference operator
	virtual const T* operator->() const = 0; //const arrow operator

	virtual void operator++() = 0; //pre-increment operator
	virtual void operator--() = 0; //pre-decrement operator

	virtual ConstIteratorBase* clone() const = 0;
protected:
	virtual bool equal(const ConstIteratorBase& rhs) const = 0;
};

/**
 * TODO doc
 */
template<typename T>
class ConstCollectionIterator
{
	friend class ConstIteratorBase<T>;

public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = T;
	using const_pointer = const T*;
	using const_reference = const T&;
	using difference_type = std::ptrdiff_t;
	using size_type = size_t;

	ConstCollectionIterator() = default; //default constructor, using unique_ptr default constructor
	ConstCollectionIterator(const ConstCollectionIterator& itr) : itr_{ itr.itr_->clone() } {} //copy constructor
	ConstCollectionIterator(ConstCollectionIterator&& itr) noexcept : itr_{ std::move(itr.itr_) } {} //move constructor
	//destructed via unique_ptr destructor

	explicit ConstCollectionIterator(ConstIteratorBase<T>* ptr) : itr_{ ptr } {} //actual constructor

    //Copy assigment
	ConstCollectionIterator& operator=(const ConstCollectionIterator& rhs) { itr_.reset(rhs.itr_->clone()); return *this; }

	//Move assignment
	ConstCollectionIterator& operator=(ConstCollectionIterator&& rhs) noexcept
	{ itr_ = std::move(rhs.itr_); return *this; }

	//Can be compared for equivalence using the equality/inequality operators
	bool operator==(const ConstCollectionIterator& rhs) const { return *itr_ == *rhs.itr_; }
	bool operator!=(const ConstCollectionIterator& rhs) const { return !(*itr_ == *rhs.itr_); }

	//Can be dereferenced as an rvalue
	const_reference operator*() const { return **const_cast<const ConstIteratorBase<T>*>(itr_.get()); }
	const_pointer operator->() const { return const_cast<const ConstIteratorBase<T>*>(itr_.get())->operator->(); }

	//Can be incremented
	ConstCollectionIterator& operator++() { ++(*itr_); return *this; }
	ConstCollectionIterator operator++(int) { ConstCollectionIterator tempItr = *this; ++(*itr_); return tempItr; }

	//Can be decremented
	ConstCollectionIterator& operator--() { --(*itr_); return *this; }
	ConstCollectionIterator operator--(int) { ConstCollectionIterator tempItr = *this; --(*itr_); return tempItr; }

	//Lvalues are swappable
	    //via use of std::swap with move constructor

private:
	std::unique_ptr<ConstIteratorBase<T>> itr_;
};


#endif // !COLLECTION_ITERATOR_HPP

