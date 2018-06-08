#ifndef FILTERED_COLLECTION_HPP
#define FILTERED_COLLECTION_HPP

#include "Collection.hpp"
#include <functional>

template<typename T>
class FilteredIterator;

template<typename T>
class ConstFilteredIterator;

template<typename T>
class FilteredCollection : public Collection<T>
{
    friend class FilteredIterator<T>;
	friend class ConstFilteredIterator<T>;

    public:
        using value_type = typename Collection<T>::value_type;
        using allocator_type = typename Collection<T>::allocator_type;
        using reference = typename Collection<T>::reference;
        using const_reference = typename Collection<T>::const_reference;
        using pointer = typename Collection<T>::pointer;
        using const_pointer = typename Collection<T>::const_pointer;
        using iterator = typename Collection<T>::iterator;
        using const_iterator = typename Collection<T>::const_iterator;
        //using reverse_iterator = typename Collection<T>::reverse_iterator;
        //using const_reverse_iterator = typename Collection<T>::const_reverse_iterator;
        using difference_type = typename Collection<T>::difference_type;
        using size_type = typename Collection<T>::size_type;

    private:
        using filtre_t = std::function<bool(const_reference)>;

    public:
        FilteredCollection(Collection<T>& c, const filtre_t& f) : collection_{&c},
                                                                  const_collection_{&c},
		                                                          is_const{ false },
                                                                  filtre_{f} {};

        FilteredCollection(const Collection<T>& c, const filtre_t& f) : collection_{nullptr},
                                                                        const_collection_{&c},
                                                                        is_const{true},
                                                                        filtre_{f} {};

        iterator begin() override
        {
			if (is_const)
				throw std::runtime_error("Wrong method called on const FilteredCollection.");

            auto first = collection_->begin();
            auto last = collection_->end();
            // Gestion du cas vide
            if(first == last)
                return end();
            
            CollectionIterator<T> it(new FilteredIterator<T>(*this, first)); 
            // si le premier élément passe le filtre, on le retourne
            if(filtre_(*it))
                return it;
            // sinon on envoie le suivant (qui passe le filtre)
            return ++it;
        }

		const_iterator begin() const override
		{
			return cbegin();
		}

		const_iterator cbegin() const override
		{
			auto first = const_collection_->cbegin();
			auto last = const_collection_->cend();
			// Gestion du cas vide
			if (first == last)
				return end();

			ConstCollectionIterator<T> it(new ConstFilteredIterator<T>(*this, first));
			// si le premier élément passe le filtre, on le retourne
			if (filtre_(*it))
				return it;
			// sinon on envoie le suivant (qui passe le filtre)
			return ++it;
		}

        iterator end() override
        {
			if (is_const)
				throw std::runtime_error("Wrong method called on const FilteredCollection.");

            return iterator(new FilteredIterator<T>(*this, collection_->end()));
        }

        const_iterator end() const override
        {
			return cend();
        }

		const_iterator cend() const override
		{
			return const_iterator(new ConstFilteredIterator<T>(*this, const_collection_->cend()));
		}

    private:
		bool is_const = false;
        Collection<T>* collection_ = nullptr;
        const Collection<T>* const_collection_ = nullptr;
        filtre_t filtre_;
};

template<typename T>
class FilteredIterator : public IteratorBase<T>
{
    public:
        FilteredIterator(FilteredCollection<T>& f, CollectionIterator<T> itr, bool valid = true) : 
            filter_reference_{ f }, itr_{ itr }, valid_{valid} {}
    
        T & operator*() override //dereference operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator*() on an iterator in invalid state");
            if(itr_ == filter_reference_.collection_->end())
                throw std::out_of_range("Error: call operator*() on an iterator past the end of his internal container");
            return *itr_;
        }
    
        const T& operator*() const override //const dereference operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator*() on an iterator in invalid state");
            if(itr_ == filter_reference_.collection_->end())
                throw std::out_of_range("Error: call operator*() on an iterator past the end of his internal container");
            return *itr_;
        }
    
        T* operator->() override //arrow operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator->() on an iterator in invalid state");
            if(itr_ == filter_reference_.collection_->end())
                throw std::out_of_range("Error: call operator->() on an iterator past the end of his internal container");
            return itr_.operator->();
        }
    
        const T* operator->() const override //const arrow operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator->() on an iterator in invalid state");
            if(itr_ == filter_reference_.collection_->end())
                throw std::out_of_range("Error: call operator->() on an iterator past the end of his internal container");
            return itr_.operator->();
        }
    
        void operator++() override //pre-increment operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator++() on an iterator in invalid state");
            while(itr_ != filter_reference_.collection_->end())
            {
                ++itr_;
                if(itr_ == filter_reference_.collection_->end() || filter_reference_.filtre_(*itr_))
                    return;
            }
            valid_ = false;
        }
    
        void operator--() override //pre-decrement operator
        {
            if(!valid_)
                throw std::out_of_range("Error: call operator--() on an iterator in invalid state");
            while(itr_ != filter_reference_.collection_->begin())
            {
                --itr_;
                if(filter_reference_.filtre_(*itr_))
                    return;
            }
            valid_ = false;
        }
    
        IteratorBase<T>* clone() const override
        {
            return new FilteredIterator(filter_reference_, itr_, valid_);
        }
    
        bool equal(const IteratorBase<T>& rhs) const override
        {
            auto& rhs_ref = dynamic_cast<const FilteredIterator&>(rhs);
            if(!valid_ && !rhs_ref.valid_)
                return true;
            if(!valid_ || !rhs_ref.valid_)
                return false;
            return itr_ == rhs_ref.itr_;
        }
    
    private:
        FilteredCollection<T>& filter_reference_;
        CollectionIterator<T> itr_;
        bool valid_;
};

template<typename T>
class ConstFilteredIterator : public ConstIteratorBase<T>
{
public:
	ConstFilteredIterator(const FilteredCollection<T>& f, ConstCollectionIterator<T> itr, bool valid = true) :
		filter_reference_{ f }, itr_{ itr }, valid_{ valid } {}

	const T& operator*() const override //const dereference operator
	{
		if (!valid_)
			throw std::out_of_range("Error: call operator*() on an iterator in invalid state");
		if (itr_ == filter_reference_.const_collection_->cend())
			throw std::out_of_range("Error: call operator*() on an iterator past the end of his internal container");
		return *itr_;
	}

	const T* operator->() const override //const arrow operator
	{
		if (!valid_)
			throw std::out_of_range("Error: call operator->() on an iterator in invalid state");
		if (itr_ == filter_reference_.const_collection_->cend())
			throw std::out_of_range("Error: call operator->() on an iterator past the end of his internal container");
		return itr_.operator->();
	}

	void operator++() override //pre-increment operator
	{
		if (!valid_)
			throw std::out_of_range("Error: call operator++() on an iterator in invalid state");
		while (itr_ != filter_reference_.const_collection_->cend())
		{
			++itr_;
			if (itr_ == filter_reference_.const_collection_->cend() || filter_reference_.filtre_(*itr_))
				return;
		}
		valid_ = false;
	}

	void operator--() override //pre-decrement operator
	{
		if (!valid_)
			throw std::out_of_range("Error: call operator--() on an iterator in invalid state");
		while (itr_ != filter_reference_.const_collection_->cbegin())
		{
			--itr_;
			if (filter_reference_.filtre_(*itr_))
				return;
		}
		valid_ = false;
	}

	ConstIteratorBase<T>* clone() const override
	{
		return new ConstFilteredIterator(filter_reference_, itr_, valid_);
	}

	bool equal(const ConstIteratorBase<T>& rhs) const override
	{
		auto& rhs_ref = dynamic_cast<const ConstFilteredIterator&>(rhs);
		if (!valid_ && !rhs_ref.valid_)
			return true;
		if (!valid_ || !rhs_ref.valid_)
			return false;
		return itr_ == rhs_ref.itr_;
	}

private:
	const FilteredCollection<T>& filter_reference_;
	ConstCollectionIterator<T> itr_;
	bool valid_;
};

#endif // !FILTERED_COLLECTION_HPP

